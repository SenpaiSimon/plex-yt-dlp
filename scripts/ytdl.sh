#!/bin/sh

#############################################################
## Help Screen Print
#############################################################
RED='\033[0;31m'   #'0;31' is Red's ANSI color code
GREEN='\033[0;32m'   #'0;32' is Green's ANSI color code
THICK='\033[1;32m'   #'1;32' is Yellow's ANSI color code
BLUE='\033[0;34m'   #'0;34' is Blue's ANSI color code
NOCOLOR='\033[0m'

#############################################################
## Help Screen Print
#############################################################
if [ "$1" = "-h"  -o  "$1" = "--help"  -o  -z "$1" ]; then
  echo "##################################################"
  echo "#"
  echo "# ${THICK}ytdl.sh USAGE ${NOCOLOR}"
  echo "#"
  echo "##################################################"
  echo "#"
  echo "# ${THICK}ytdl.sh [url] [mediaType] ${BLUE}[playlistStartIndex] [forceID]${NOCOLOR}"
  echo "#"
  echo "# ${THICK}green args${NOCOLOR} are mandatory - ${BLUE}blue args${NOCOLOR} are optional ${NOCOLOR}"
  echo "#"
  echo "# ${THICK}[url]${NOCOLOR} = url to playlist or single video"
  echo "#"
  echo "# ${THICK}[mediaType]${NOCOLOR} = music, playlist, single"
  echo "#    single  -> Single Video"
  echo "#    plalist -> whole playlist"
  echo "#    music   -> only download sound - works with playlist or single file"
  echo "#    rss     -> Download whole RSS Feed"
  echo "#"
  echo "# ${BLUE}[playlistStartIndex]${NOCOLOR} = force a starting index"
  echo "#"
  echo "# ${BLUE}[forceID]${NOCOLOR} = overwrites the auto-id "
  echo "#"
  echo "##################################################"
  exit 0
fi


#############################################################
## OUTPUT Config File
#############################################################
confFile="config.txt"

if [ ! -f "$confFile" ]; then
  touch "$confFile"
  echo "# Put Video Path in the next line" >> "$confFile" 
  echo "" >> "$confFile"
  echo "# Put Music Path in the next line" >> "$confFile" 
  echo "" >> "$confFile"
  echo "# Put YouTube API Key in the next line" >> "$confFile"
  echo "${THICK}Created Config-File${NOCOLOR}"
  echo "Please Fill in paths like this: /this/is/a/path"
  echo "                       or this: ./path/to/folder"
  echo ""
  echo "Also please add your YouTube Key"
  echo "see: https://blog.hubspot.com/website/how-to-get-youtube-api-key"
  exit 0
fi


#############################################################
## Check if yt-dlp is even there
#############################################################
if [ ! $(command -v yt-dlp) ]; then
  echo "${RED}Did not find yt-dlp exectuable on path${NOCOLOR}" >&2
  echo "${RED}Please install it from here and add it to your path${NOCOLOR}" >&2
  echo "      https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp" >&2
  exit 1
fi


#############################################################
## Check if supermove is there
#############################################################
if [ ! -f "supermove.sh" ]; then
  echo "${RED}Did not find supermove.sh in this dir${NOCOLOR}" >&2
  echo "${RED}Please also get it from the repo${NOCOLOR}" >&2
  exit 1
fi


#############################################################
## PATHS
#############################################################
skriptPath=$(pwd)
tempFolder="./temp"
videoOutFolder=$(head -n 2 "$confFile" | tail -n 1)
musicOutFolder=$(head -n 4 "$confFile" | tail -n 1)
api_key=$(head -n 6 "$confFile" | tail -n 1)


#############################################################
## CHECK ARGS
#############################################################
if [ -z "$2" ]
  then
    echo "${RED}ERR No type given on argument 2${NOCOLOR}" >&2
    exit 1
fi


#############################################################
## SET ARGS
#############################################################
dlUrl=$1
mediaType=$2


#############################################################
## Create Folder Structure
#############################################################
mkdir -p ${tempFolder}
mkdir -p ${videoOutFolder}
mkdir -p ${musicOutFolder}


#############################################################
## Validate Paths
#############################################################
if [ ! -d "${videoOutFolder}" ]; then
  echo "${RED}ERR Video Out Path is invalid${NOCOLOR}" >&2
  exit 1
fi
if [ ! -d "${musicOutFolder}" ]; then
  echo "${RED}ERR Video Out Path is invalid${NOCOLOR}" >&2
  exit 1
fi


#############################################################
## Generate Unique ID
#############################################################
if [ "$2" = "playlist" ]; then
  # Get video ID from playlist URL
  playlist_id=$(echo "$dlUrl" | grep -oP 'PL[\w-]*')
  # Use curl to get channel information based on playlist ID
  api_response=$(curl -s "https://www.googleapis.com/youtube/v3/playlists?part=snippet&id=$playlist_id&key=$api_key") 
  # Parse the JSON response to extract channel title (requires tools like jq)
  channelName=$(echo "$api_response" | grep -oP '"channelTitle": "([^"]*)"' | sed 's/"channelTitle": "\(.*\)"/\1/')
  plTitle=$(echo "$api_response" | grep -oP -m 1 '"title": "([^"]*)"' | sed 's/"title": "\(.*\)"/\1/')
  plTitle="${plTitle} [youtube-${playlist_id}]"

  # check if this playlist has already been downloaded at some point
  if [ -d "${videoOutFolder}/${channelName}/${plTitle}" ]; then
    # get id as the season num
    uniqueID=$(ls -l "${videoOutFolder}/${channelName}/${plTitle}" | grep -oP -m 1 'S([0-9]+)' | head -n 1 | sed 's/^S//')
    # get the highest episode
    startIndex=$(ls -l "${videoOutFolder}/${channelName}/${plTitle}" | grep -oP 'E([0-9]+)' | sed 's/^E0*//' | sort -nr | head -n 1)
    # increment by one
    startIndex=$((startIndex + 1))
  else # playlist is totally new --> generate new id
    # get how many folders are already there
    folderCount=$(ls -l "${videoOutFolder}/${channelName}" | grep -c '^d')
    uniqueID=$((folderCount + 1))
    startIndex=1
  fi
fi


#############################################################
## Force overwrites
#############################################################
if [ ! -z "$3" ]; then
  startIndex=$3
fi

if [ ! -z "$4" ]; then
  uniqueID=$4
fi


#############################################################
## Check for Updates
#############################################################
yt-dlp -U


#############################################################
## differentiate media types
#############################################################
if [ "${mediaType}" = "playlist" ]; then
  outFolder=${videoOutFolder}
  outFile="${tempFolder}/%(uploader)s/%(playlist_title)s [youtube-%(playlist_id)s]/%(title)s - S${uniqueID}E%(playlist_index)s [%(id)s].%(ext)s"
elif [ "${mediaType}" = "single" ]; then
  read -p "Enter Folder Name: " folderName
  outFolder=${videoOutFolder}
  outFile="${tempFolder}/%(uploader)s/${folderName}/%(title)s [%(id)s].%(ext)s"
elif [ "${mediaType}" = "music" -o "${mediaType}" = "rss" ]; then
  outFolder=${musicOutFolder}
  outFile="${tempFolder}/%(uploader)s/%(playlist_title)s/%(title)s.%(ext)s"
else
  echo "${RED}ERR Unknown Type${NOCOLOR}" >&2
  exit 1
fi


#############################################################
## post script hook call system dependent
#############################################################
if [ "$OSTYPE" = "msys" ]; then
  postHook="bash supermove.sh ${tempFolder}/ ${outFolder}/"  
else
  postHook="./supermove.sh ${tempFolder}/ ${outFolder}/"
fi


#############################################################
## actual command call
#############################################################
if [ "${mediaType}" = "music" ]; then
  yt-dlp ${dlUrl} \
    --embed-thumbnail --embed-metadata \
    --parse-metadata "playlist_autonumber:%(track_number)s" \
    --add-metadata --embed-chapters \
    -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' \
    -o "${outFile}" \
    -I ${startIndex}::1 \
    --exec "${postHook}"
elif [ "${mediaType}" = "rss" ]; then
  yt-dlp ${dlUrl} \
    --embed-thumbnail --embed-metadata \
    --parse-metadata "playlist_autonumber:%(track_number)s" \
    --add-metadata --embed-chapters --playlist-reverse \
    -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' \
    -o "${outFile}" \
    -I ${startIndex}::1 \
    --exec "${postHook}"
else
  yt-dlp ${dlUrl} \
    --sub-format best --sub-langs de.*,ger.*,en.* \
    --embed-subs --embed-thumbnail --embed-metadata --embed-chapters \
    --write-info-json \
    -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4' \
    -o "${outFile}" \
    -I ${startIndex}::1 \
    --exec "${postHook}"
fi


#############################################################
## CLEAN UP
#############################################################
echo "Cleaning up..."
rm -r ${tempFolder}
echo "Done."
