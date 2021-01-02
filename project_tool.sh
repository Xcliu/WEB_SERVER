#!/bin/bash

function clear_cached_cmakefiles()
{
    for var in `ls $1`
        do
        if test -d "$1/${var}";then
            if [[ "$var" == "cmake_build" ]];then
                rm -rf ${1}/${var}
            else 
                clear_cached_cmakefiles "$1/${var}"
            fi
        fi
        done
}

function complie()
{
    cd ./$1
    mkdir cmake_build
    cd ./cmake_build
    cmake ..
    make 
}

init_path=.

if [[ $1 == "clean_cache" ]] ;then
        clear_cached_cmakefiles $init_path
    else
        if [[ $1 == "complie" ]]; then
            complie $2
        fi
fi