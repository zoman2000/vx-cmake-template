#!/bin/bash

. prepare_dep.dat

demo=
shallow=0

while [ ! -z "$1" ] ; do
    case $1 in 
	deep)
	    shallow=0
	    ;;
	shallow)
	    shallow=1
	    ;;
	demo)
	    demo=echo
	    ;;
    esac
    shift
done

function run_prepare_dep() {
if [ -z "$modules" ] ; then
    echo \"modules\" parameter is not set in the configuration file. Using "external".
    echo -n 'Continue [y|N]:'
    read resp
    if [ "$resp" != "y" ] ; then
        exit
    fi
    modules="external"
fi

### Init submodules for current or parent project
case $modules in 
    parent)
        pushd .. > /dev/null
        git submodule update --init
        popd >/dev/null
        ;;

    submodules)
        git submodule update --init
        ;;
esac

### Iterate over all repos

if [ ${#repos[@]} -ne 0 ] ; then
    echo ---------------------------------------------
    echo Add dependent repositories
    echo ---------------------------------------------

    for key in "${!repos[@]}"; do
        repourls=(${repos[$key]/|/ })
        repourl=${repourls[0]}
        repoitem=${repourls[1]}
        ok=1

        echo ---- Repository for $key from $repourl\|$repoitem

        case $modules in
            parent|external)
                if [ ! -d ../$key ] || [ ! -e ../$key/.git ] ; then
                    ok=0
                fi
                ;;

            submodules)
                if [ ! -d $key ] ; then
                    ok=0
                fi
        esac

        if [ $ok -eq 0 ] ; then
            case $modules in
                parent)
                    pushd .. > /dev/null
                    $demo git submodule add --force $repourl
                    popd > /dev/null
                    ;;

                external)
                    pushd .. > /dev/null
                    $demo git clone $repourl
                    popd > /dev/null
                    ;;

                submodules)
                    $demo git submodule add --force $repourl
                    ;;
            esac
        fi

        case $modules in 
            parent|external)
                if [ ! -L $key ] ; then
                    echo link $key does not exist
                    $demo ln -s ../$key $key
                fi
                ;;

            submodules)
                ;;
        esac

        if [ -e $key ] ; then
    	pushd $key >/dev/null
    	    echo git fetch --tags -f
    	    $demo git fetch --tags -f
    	    echo git fetch --all -f
    	    $demo git fetch --all -f
    	    if [ ! -z "$repoitem" ] ; then
        		echo git checkout $repoitem
        		$demo git checkout $repoitem
    	    else
        		echo $demo git pull
        		$demo git pull
    	    fi
    	popd >/dev/null
        fi
    done
fi

if [ ${#links[@]} -ne 0 ] ; then
    echo ---------------------------------------------
    echo Configure links
    echo ---------------------------------------------

    for key in "${!links[@]}"; do
        if [ ! -L src/$key ] ; then
    	$demo ln -s ${links[$key]} src/$key
        fi
    done
fi

if [ ${#submodules[@]} -ne 0 ] ; then
    echo ---------------------------------------------
    echo Run submodule init on configured submodules
    echo ---------------------------------------------

    for key in "${submodules[@]}"; do
        pushd $key >/dev/null
        $demo git submodule update --init
        popd >/dev/null
    done
fi

if [ ${#repos[@]} -ne 0 ] && [ $shallow -eq 0 ] ; then
    echo ---------------------------------------------
    echo Run prepare_dep on all dependent repositories
    echo ---------------------------------------------

    for key in "${!repos[@]}"; do
        if [ -e $key/prepare_dep.sh ] ; then
    	pushd $key >/dev/null
        echo ---- Repository for $key from ${repos[$key]}
    	$demo ./prepare_dep.sh $branch
    	popd >/dev/null
        fi
    done
fi

if [ ${#folders[@]} -ne 0 ] ; then
    echo ---------------------------------------------
    echo Run prepare_dep on all configured subfolders
    echo ---------------------------------------------

    for key in "${!folders[@]}"; do
        echo ---- Folder for $key from ${repos[$key]}
        if [ -e $key/prepare_dep.sh ] ; then
    	pushd $key >/dev/null
    	$demo ./prepare_dep.sh $branch
    	popd >/dev/null
        fi
    done
fi
}

run_prepare_dep $1 $2 $3 $4 $5
