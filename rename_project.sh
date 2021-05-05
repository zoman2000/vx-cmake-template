#!/bin/bash

curdirr=$(pwd)
dirr=$(basename $curdirr)
if [ $dirr == "vx-cmake-template" ] ; then
    echo Run this script inside a new folder to rename
    echo vx-cmake-template to a current \'useful\' name
    exit 1
fi

[ -d build ] && rm -rf build
[ -d .vscode/ipch ] && rm -rf .vscode/ipch

echo Renaming \"vx-cmake-template\" to $dirr

while read filer
do 
    sed -i "s|vx-cmake-template|$dirr|g" $filer
    if [[ $filer =~ ^.*vx-cmake-template.*$ ]] ; then
        newfiler=${filer/vx-cmake-template/$dirr}
        mv $filer $newfiler
    fi
done < <(find . -type f)
