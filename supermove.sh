#!/bin/bash

######################################################
## PARSE ARGS
######################################################
src=$1
dest=$2

######################################################
## Create full path
######################################################
src=$(readlink -f "$src")
dest=$(readlink -f "$dest")

######################################################
## i dont know why
######################################################
cd "$src"

######################################################
## Windows rsync is dogshit, so dont use it
######################################################
if [[ "$OSTYPE" == "msys" ]]; then
    find * -type d -exec mkdir -p "$dest"/{} \;
    find * -type f -exec mv {} "$dest"/{} \;
else
    ######################################################
    ## Copy hidden files too
    ######################################################
    shopt -s dotglob

    ######################################################
    ## actual move
    ######################################################
    rsync -avzh --remove-source-files --info=name1 --info=progress2 --no-inc-recursive "$src"/ "$dest"/

    # also copy permissions, acls, xattrs
    rsync -aAX "$src"/ "$dest"/
fi
