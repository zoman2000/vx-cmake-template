#!/bin/bash

type=Debug
runbuild=0
runtest=0
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

calculate_jobs

while [ ! -z "$1" ] ; do
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

	-d|dep)
	    . ./prepare_dep.sh
	    ;;
	-c|clean)
	    . ./cleanup.sh
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
	-g|gtest)
	    runbuild=1
	    runtest=1
	    ;;
    esac
    shift
done

[ ! -d build ] && mkdir build

pushd build >/dev/null

    cmake -DCMAKE_BUILD_TYPE=$type -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
    if [ $runbuild -ne 0 ] ; then
	export TEMP=/run/user/$EUID
	make -j $BUILD_JOBS
    fi

    if [ $runtest -ne 0 ] ; then
    	BINARY_NAME=$(dirname `pwd`)
    	BINARY_NAME=$(basename $BINARY_NAME)
    	./${BINARY_NAME}-test
    fi

    if [ $rungtest -ne 0 ] ; then
    	BINARY_NAME=$(dirname `pwd`)
    	BINARY_NAME=$(basename $BINARY_NAME)
    	./${BINARY_NAME}-gtest
    fi

popd >/dev/null
