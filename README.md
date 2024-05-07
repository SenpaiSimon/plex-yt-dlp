# plex-yt-dlp

[![CMake on multiple platforms](https://github.com/SenpaiSimon/plex-yt-dlp/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/SenpaiSimon/plex-yt-dlp/actions/workflows/cmake-multi-platform.yml)

Easy to use binary wrapper for using [yt-dlp](https://github.com/yt-dlp/yt-dlp).

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

# Usage

## Creating a YouTube Api-Key

Feel free to refer to this guide
[How to Get a YouTube API Key](https://blog.hubspot.com/website/how-to-get-youtube-api-key)

## Config-File

Upon running the script for the first time you will be prompted with the message that a `config.txt`-File was created.
Please add your paths and your api-key to the `config.json`-File.

### Config-File Example

> NOTE: This is not a real API Key

```json
{
    "apiKey": "iwdowia2138DOAWIDHN",
    "tempPath": "./temp",
    "musicPath": "./music",
    "rssPath": "./rss",
    "videoPath": "./video"
}
```

# Usage

Just use `plex-yt-dlp -h` or `plex-yt-dl --help` for the help screen.

This will print something like:

```text
=================================================================
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
== -t, --type [mediaType]
==      What output you want -- "video" or "music" or "rss"
==
== -io, --indexOverwrite [num] -- optional
==      Overwrite the playlist index
==
== -ido, --idOverwrite [num] -- optional
==      Overwrite the auto ID
==
=================================================================
```

## Examples

```bash
# Download a video/playlist
ytdl.sh -u ulrHere -t video

# Download a single track or playlist
ytdl.sh -u urlHere -t music

# Download a rss feed
ytdl.sh -u urlHere -t rss

# Download a playlist starting from index 10
ytdl.sh -u ulrHere -t video -io 10

# Download a music playlist starting from index 10
ytdl.sh -u ulrHere -t music -io 10

# Download a rss feed starting from index 10
ytdl.sh -u ulrHere -t rss -io 10
```