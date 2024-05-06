# plex-yt-dlp

Easy to use Bash-Script based wrapper for using [yt-dlp](https://github.com/yt-dlp/yt-dlp).

# What it does
* This creates the Folder-Structures and also the File-Names exactly like it's needed for using in a Plex-Library.

* For Plex you have to use the [YouTube-Agent](https://github.com/ZeroQI/YouTube-Agent.bundle) alongside with the [Absolute-Series-Scanner](https://github.com/ZeroQI/Absolute-Series-Scanner) for your library to look beautiful.

# Features
* Embeds Video metadata into the video itself
* Embeds the video thumbnail into the video
* Embeds thumbnail as music cover
* Downloads metadata as info.json for plex
* Downloads and embeds english and german subtitles if available
* This can also download RSS-Feeds -- use "rss" option for that

# Requirements

Please install the following packages before using this script:

* [yt-dlp](https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp)
* [ffmpeg](https://ffmpeg.org/)
* [python3-mutagen](https://mutagen.readthedocs.io/en/latest/)
* [git-bash](https://git-scm.com/downloads) (windows only)

On Windows this script is meant to be run in the git-bash shell.

# Installation

You can install ffmpeg and python3-mutagen using the following commands:

**Windows**
```bash
winget install -e --id Gyan.FFmpeg
python3 -m pip install mutagen
```

**Linux**
```bash
sudo apt install ffmpeg
sudo apt install python3-mutagen
```

# Exeutable

Also make sure both scripts are executable. Just run following commands:
```bash
chmod +x ytdl.sh
chmod +x supermove.sh
```

# Usage

## Creating a YouTube Api-Key

Feel free to refer to this guide
[How to Get a YouTube API Key](https://blog.hubspot.com/website/how-to-get-youtube-api-key)

## Config-File

Upon running the script for the first time you will be prompted with the message that a `config.txt`-File was created.
Please add your paths and your api-key to the `config.txt`-File.

### Config-File Example

> NOTE: This is not a real API Key

```text
# Put Video Path in the next line
./video
# Put Music Path in the next line
./music
# Put YouTube API Key in the next line
WDdhjoaiwdhoiWADWAdoihDAWdoih
```

# Usage

Just use `ytdl.sh -h` or `ytdl.sh --help` for the help screen.

This will print something like:

```
ytdl.sh [url] [mediaType] [playlistStartIndex] [forceID]


[url] = url to playlist or single video

[mediaType] = music, playlist, single
    single  -> Single Video
    plalist -> whole playlist
    music   -> only download sound - works with playlist or single file
    rss     -> Download whole RSS Feed

[playlistStartIndex] = force a starting index

[forceID] = overwrites the auto-id
```

## Examples

```bash
# Download a single video
ytdl.sh urlToVideoHere single

# Download a playlist
ytdl.sh urlToPlaylistHere playlist

# Download a music playlist
ytdl.sh urlToPlaylistHere music

# Download a rss feed
ytdl.sh urlToPlaylistHere rss

# Download a playlist starting from video 10
ytdl.sh urlToPlaylistHere playlist 10

# Download a music playlist starting from video 10
ytdl.sh urlToPlaylistHere music 10

# Download a rss feed starting from video 10
ytdl.sh urlToPlaylistHere rss 10
```