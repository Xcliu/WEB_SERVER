#!/bin/bash

cmake_dir_name="BUILD"

function clear_cache()
{
    for var in `ls $1`
        do
        if test -d "${var}";then
            if [[ "$var" == ${cmake_dir_name} ]];then
                rm -rf ${var}
            else 
                clear_cache "$1/${var}"
            fi
        fi
        done
}

function complie()
{
    cd ./$1
    if test -d "${cmake_dir_name}";then
        rm -rf ${cmake_dir_name}  
    fi
    mkdir ${cmake_dir_name}
    cd ./${cmake_dir_name}
    cmake ..
    make 
}

init_path=.

if [[ $1 == "clean_build" ]] ;then
        clear_cache $init_path
    else
        if [[ $1 == "complie" ]]; then
            complie $init_path
        fi
fi
