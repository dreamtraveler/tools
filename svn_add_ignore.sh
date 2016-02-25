#!/bin/bash
pre=100
for file in $(svn status); do
    if [ "$file" == "?" ]; then
        pre=0
    fi
    unversion_file=${file##*/}
    pre=$[pre+1]
    if [ $pre -eq 2 ]; then
        is_find=0
        for ignore_file in $(cat .svnignore); do
            if [[ $unversion_file = $ignore_file ]]; then
                is_find=1
                break
            fi
        done

        if [ $is_find -eq 0 ]; then
            if [[ $1 == "-y" ]]; then
                svn add $file
            else
                echo $file
            fi
        fi
    fi
done
