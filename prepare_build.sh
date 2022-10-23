#!/bin/bash

type=Debug
runbuild=0
runtest=0
runinstall=0
dosudo=
cmakeoopt=
democmd=
rungtest=0

BUILD_JOBS=2

function calculate_jobs() {
    local newjobs=0
    local cpus=$(cat /proc/cpuinfo | grep processor | wc -l)
    local memfree=($(cat /proc/meminfo | grep MemAvailable))
    let newjobs=${memfree[1]}/800000+1
    if [ $newjobs -gt $cpus ] ; then
        newjobs=$cpus
    fi
    if [ $newjobs -gt $BUILD_JOBS ] ; then
        BUILD_JOBS=$newjobs
    fi
}

myloc='.'
if [ ! -e prepare_dep.sh ] ; then
    myloc='/opt/vxpro/vx'
fi

calculate_jobs

while [ ! -z "$1" ] ; do
    cont=1
    case $1 in
        Debug|Release|MinSizeRel)
            type=$1
        ;;
        dev|debug)
            type=Debug
        ;;
        prod|release)
            type=MinSizeRel
        ;;
        cov|coverage)
            export VXPRO_COVERAGE=1
        ;;
        -d|dep)
            . $myloc/prepare_dep.sh
        ;;
        -c|clean)
            . $myloc/cleanup.sh clean
        ;;
        -b|build|run)
            runbuild=1
        ;;
        -j|jobs)
            shift
            if [ ! -z "$1" ] ; then
                BUILD_JOBS=$1
            fi
            runbuild=1
        ;;
        -t|test)
            runbuild=1
            runtest=1
        ;;
        in|ins|install)
            runbuild=1
            runinstall=1
        ;;
        su|sudo)
            dosudo='sudo'
        ;;
        -o)
            shift
            cmakeoopt="-D$1 $cmakeopt"
        ;;
        --demo)
            democmd=echo
        ;;
        -g|gtest)
            runbuild=1
            rungtest=1
        ;;
        --help|-h|help)
    	    cat <<HHH
Prepare cmake build.
Command:
    vx prepare build [options]
Options:
    Debug|Release|MinSizeRel    - set exact cmake target (default is Debug)
    dev|debug                   - set "Debug" target
    prod|release                - set MinSizeRel target
    cov                         - enable coverage compile/linker flags
    -d|dep                      - run vx prepare dep first
    -c|clean                    - run ./cleanup.sh script
    -b|build|run                - run make after executing cmake command
    -j|jobs num                 - set "num" make jobs
    -t|test                     - run tests using a default debug binary name as {project name}-test
    in|ins|install              - run sudo make install after build is complete
    su|sudo                     - use sudo when running make install
    -o option                   - add cmake define as -Doption
    --help|-h|help              - this hellp
    --demo                      - investigate all the commands to be executed
    -g|gtest                    - run gtest suite
HHH
            return 0 2> /dev/null || exit 0
        ;;
        *)
            cont=0
        ;;
    esac
    if [ $cont -eq 0 ] ; then
        break
    fi
    shift
done

[ ! -d build ] && mkdir build

if [ ! -z "$BUILD_DEBUG" ] ; then
    echo ">>>> prepare build: using version: $PROJECT_TAG_MAJOR.$PROJECT_TAG_MINOR.$PROJECT_TAG_PATCH"
fi

pushd build >/dev/null
$democmd cmake -DCMAKE_BUILD_TYPE=$type -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $cmakeoopt ..
if [ $runbuild -ne 0 ] ; then
    $democmd export TEMP=/run/user/$EUID
    $democmd make -j $BUILD_JOBS
fi

# load config parameters from build.dat or build.`hostname`

hos=$(hostname)
buildconfig=build.dat
if [ -e ../build.$hos ] ; then
    buildconfig="build.$hos"
fi
if [ -e ../$buildconfig ] ; then
    . ../$buildconfig
fi

# binary name could be provided in the config file or matches the folder name

if [ -z "$BINARY_NAME" ] ; then
    BINARY_NAME=$(dirname `pwd`)
    BINARY_NAME=$(basename $BINARY_NAME)
fi

if [ $runtest -ne 0 ] ; then
    $democmd ./${BINARY_NAME}-test $@
fi

if [ $runinstall -ne 0 ] ; then
    $democmd $dosudo make install
fi

if [ $rungtest -ne 0 ] ; then
    ./${BINARY_NAME}-gtest
fi

popd >/dev/null
