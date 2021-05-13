#!/bin/bash

myname="vx-cmake-template"
curdirr=$(pwd)
dirr=$(basename $curdirr)
if [ $dirr == "$myname" ] ; then
    echo Run this script inside a new folder to rename
    echo $myname to a current \'useful\' name
    exit 1
fi

[ -d build ] && rm -rf build
[ -d .vscode/ipch ] && rm -rf .vscode/ipch

echo Renaming \"$myname\" to $dirr

while read filer
do 
    if [[ $filer =~ ^\./\.git ]] ; then
            continue
    fi
    sed -i "s|$myname|$dirr|g" $filer
    if [[ $filer =~ ^.*vx-cmake-template.*$ ]] ; then
        newfiler=${filer/$myname/$dirr}
        mv $filer $newfiler
    fi
done < <(find . -type f)
