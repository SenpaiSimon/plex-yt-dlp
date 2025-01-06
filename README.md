# plex-yt-dlp

[![CMake on multiple platforms](https://github.com/SenpaiSimon/plex-yt-dlp/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/SenpaiSimon/plex-yt-dlp/actions/workflows/cmake-multi-platform.yml)

Easy to use binary wrapper for using [yt-dlp](https://github.com/yt-dlp/yt-dlp).

# What it does
* This creates the Folder-Structures and also the File-Names exactly like it's needed for using in a Plex/Jellyfin Library.
* In the case of Jellyfin it creates the .nfo files from the info.json so you dont need to use the YouTube Metadata Plugin for Jellyfin.

* For Plex you have to use the [YouTube-Agent](https://github.com/ZeroQI/YouTube-Agent.bundle) alongside with the [Absolute-Series-Scanner](https://github.com/ZeroQI/Absolute-Series-Scanner) for your library to look beautiful.
* For Jellyfin you have to use the [YouTube Plugin](https://github.com/ankenyr/jellyfin-youtube-metadata-plugin)

# Features
* Embeds Video metadata into the video itself
* Embeds the video thumbnail into the video
* Embeds thumbnail as music cover
* Downloads metadata as info.json for plex
* Downloads and embeds english and german subtitles if available
* This can also download RSS-Feeds -- use "rss" option for that
* Also supports naming schemes for Jellyfin

# Requirements

Please install the following packages before using this script:

* [yt-dlp](https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp)
* [ffmpeg](https://ffmpeg.org/)
* [curl](https://curl.se/)
* [rsync (Linux only)](https://www.rsync.org/)
* [python3-mutagen](https://mutagen.readthedocs.io/en/latest/)

# Pre-Setup

## Config-File

Upon running the script for the first time you will be prompted with the message that a `config.txt`-File was created.

### Config-File Example

> NOTE: They can be relative paths to your binary or absolute paths

```json
{
    "tempPath": "./temp",
    "musicPath": "./music",
    "rssPath": "./rss",
    "videoPath": "./video"
}
```

> Why the temp path?

* The Temp-Path is where the files will be downloaded and also where the files will be modified
* If your other paths refer to a NAS-Drive for example this last step would be really slow
* The Files will get moved from temp to the final path after the converting process is finished

# Usage

Just use `./plex-yt-dlp -h` or `./plex-yt-dlp --help` for the help screen.

This will print something like:

```text
==========================================================================================================
==
== plex-yt-dlp [OPTIONS]
==
== -h, --help
==      Display this help screen
==
== -v, --version
==      Display current version
==
== -u, --url [urlHere]
==      Url to download
==
== -l, --links [pathToTxtFile]
==      File with URLs/Playlists to be download
==
== -t, --type [mediaType]
==      What output you want -- "video" or "music" or "rss"
==
== -r, --reverse
==      Reverse the order of the given playlist or rss feed
==
== -j, --jellyfin
==      Apply naming scheme for jellyfin instead of plex
==
== -io, --indexOverwrite [num] -- optional
==      Overwrite the playlist index
==
== -ido, --idOverwrite [num] -- optional
==      Overwrite the auto ID
==
==========================================================================================================
```

## Examples

```bash
# Download a video/playlist
./plex-yt-dlp -u urlHere -t video

# Download a single track or playlist
./plex-yt-dlp -u urlHere -t music

# Download a rss feed
./plex-yt-dlp -u urlHere -t rss

# Download a playlist starting from index 10
./plex-yt-dlp -u urlHere -t video -io 10

# Download a music playlist starting from index 10
./plex-yt-dlp -u urlHere -t music -io 10

# Download a rss feed starting from index 10
./plex-yt-dlp -u urlHere -t rss -io 10
```


# Building yourself

## Cloning the Repo and sub-modules

```bash
git clone https://github.com/SenpaiSimon/plex-yt-dlp.git
cd plex-yt-dlp
git submodule update --init --recursive
```

## Install Build-Tools

```bash
sudo apt update
sudo apt install cmake build-essential libcurl4-openssl-dev -y
```

## Building

Linux:
```bash
mkdir build
cd build
cmake ..
make
```

Windows:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```