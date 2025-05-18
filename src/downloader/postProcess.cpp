#include <downloader/postProcess.hpp>

postProcess::postProcess(settings setting) {
    this->setting = setting;
}

bool postProcess::isPlaylistMeta(fs::path path) {
    string filename = path.filename().string();
    return filename.rfind("E0 - ", 0) == 0 || 
           filename.rfind("E00 - ", 0) == 0 ||
           filename.rfind("E000 - ", 0) == 0;
}

void postProcess::convert() {
    vector<fs::path> images;
    vector<fs::path> infoJson;

    fs::path playlistInfo;
    string playlistName = setting.playlistPath.substr(setting.playlistPath.find_last_of("/\\") + 1); // Extract last folder

    for (const auto& entry : std::filesystem::directory_iterator(setting.playlistPath)) {
        // episode info json
        if(entry.path().extension() == ".json") {
            if(this->isPlaylistMeta(entry.path())) {
                playlistInfo = entry.path();
                cout << "== " << "Playlist Info: " << entry.path() << endl;
            } else {
                infoJson.push_back(entry.path());
            }
        }

        // all images
        if(entry.path().extension() == ".jpg" || \
           entry.path().extension() == ".webp" || \
           entry.path().extension() == ".png") {
            // we dont want E0 image since it has bad quality
            if(!this->isPlaylistMeta(entry.path())) {
                images.push_back(entry.path());
            }
        }
    }

    // sort infoJson by name
    std::sort(infoJson.begin(), infoJson.end(), [](const fs::path& a, const fs::path& b) {
        auto getNumber = [](const fs::path& p) -> int {
            std::string name = p.filename().string();
            size_t pos = name.find(" - ");
            if (pos != std::string::npos) {
                try {
                    return std::stoi(name.substr(0, pos));
                } catch (...) {}
            }
            return 0;
        };
        return getNumber(a) < getNumber(b);
    });

    for(const auto& path : infoJson) {
        cout << "== " << "Episode Info: " << path << endl;
    }

    // must be first
    this->processPlaylist(playlistInfo, images.front());
    // must be second
    this->processEpisodes(infoJson, images);
}

// all other stuff
void postProcess::processEpisodes(vector<fs::path> episodeInfos, vector<fs::path> images) {
    // rename every image to *-thumb
    for(fs::path image: images) {
        
        fs::path destination = image.parent_path() / image.stem();
        destination += "-thumb" + image.extension().string();
        fs::rename(image, destination);
    }

    for(fs::path info: episodeInfos) {
        nfoInfo_t nfo = this->scrapePath(info, true);
        this->createNfo(info, nfo, true);
    }
}

// these files will have the playlist name in the name itself
void postProcess::processPlaylist(fs::path info, fs::path image) {
    // cover image
    fs::path destination = setting.playlistPath + "/folder" + image.extension().string();
    fs::copy_file(image, destination, fs::copy_options::overwrite_existing);
    // tvshow.nfo 
    if(!info.empty()) {
        nfoInfo_t nfo = this->scrapePath(info, false);
        this->createNfo(info, nfo, false);
    }
}

void postProcess::createNfo(fs::path path, nfoInfo_t nfo, bool episode) {
    tinyxml2::XMLDocument doc;

    // Create a declaration node.
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration( "xml version=\"1.0\"" );
    doc.InsertFirstChild(decl); // Add it to the beginning of the document

    // Create root element
    tinyxml2::XMLElement* root = doc.NewElement("episodedetails");
    doc.InsertEndChild(root);

    // Add child elements
    tinyxml2::XMLElement* titleElement = doc.NewElement("title");
    titleElement->SetText(nfo.title.c_str());
    root->InsertEndChild(titleElement);

    tinyxml2::XMLElement* showtitleElement = doc.NewElement("showtitle");
    showtitleElement->SetText(nfo.showtitle.c_str());
    root->InsertEndChild(showtitleElement);

    tinyxml2::XMLElement* uniqueidElement = doc.NewElement("uniqueid");
    uniqueidElement->SetText(nfo.uniqueid.c_str());
    uniqueidElement->SetAttribute("type", "youtube");
    uniqueidElement->SetAttribute("default", "true");
    root->InsertEndChild(uniqueidElement);

    tinyxml2::XMLElement* plotElement = doc.NewElement("plot");
    plotElement->SetText(nfo.plot.c_str()); // Or leave empty if nfo.plot is empty
    root->InsertEndChild(plotElement);

    tinyxml2::XMLElement* premieredElement;
    if(episode) {
        premieredElement = doc.NewElement("aired");
    } else {
        premieredElement = doc.NewElement("premiered");
    }
    premieredElement->SetText(nfo.premiered.c_str());
    root->InsertEndChild(premieredElement);

    tinyxml2::XMLElement* directorElement = doc.NewElement("director");
    directorElement->SetText(nfo.director.c_str());
    root->InsertEndChild(directorElement);

    // ordering in display
    if(episode) {
        tinyxml2::XMLElement* displayorderElement = doc.NewElement("episode");
        displayorderElement->SetText(nfo.displayOrder);
        root->InsertEndChild(displayorderElement);
    }

    // season always 1
    tinyxml2::XMLElement* seasonElem;
    if(episode) {
        seasonElem = doc.NewElement("season");

    } else {
        seasonElem = doc.NewElement("seasonnumber");
    }
    seasonElem->SetText(1);
    root->InsertEndChild(seasonElem);

    // save it 
    fs::path dest;
    if(episode) {
        dest = path.replace_extension("").replace_extension("nfo");
    } else {
        dest = path.remove_filename().append("tvshow.nfo");
    }

    doc.SaveFile(dest.string().c_str());
}

nfoInfo_t postProcess::scrapePath(fs::path path, bool episode) {
    nfoInfo_t temp = {};
    json tempJson;

    std::ifstream i(path.string());
    i >> tempJson;

    temp.title = tempJson["title"];
    temp.showtitle = tempJson["uploader"];
    temp.uniqueid = tempJson["id"];
    if(tempJson.contains("description")) {
        temp.plot = tempJson["description"];
    } else {
        temp.plot = "";
    }
    temp.director = tempJson["channel"];

    if(episode) {
        // should be aired here
        temp.premiered = this->reformatDate(tempJson["upload_date"]);
        if(tempJson.contains("playlist_index")) {
            temp.displayOrder = tempJson["playlist_index"];
            this->curIndex = temp.displayOrder;
        } else {
            temp.displayOrder = this->curIndex;
            this->curIndex++;
        }
    } else {
        // here we can call it premiered
        temp.premiered = this->reformatDate(tempJson["modified_date"]);
    }

    return temp;
}

std::string postProcess::reformatDate(const std::string& date) {
    if (date.length() != 8) {
        return ""; // Or handle the error as appropriate
    }

    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(4, 2));
    int day = std::stoi(date.substr(6, 2));

    std::stringstream ss;
    ss << year << "-" << std::setw(2) << std::setfill('0') << month << "-" << std::setw(2) << std::setfill('0') << day;
    return ss.str();
}