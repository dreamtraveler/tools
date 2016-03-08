#!/bin/bash
pre=100
commit_file=" "
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
                commit_file="$commit_file $file"
            else
                echo $file
                commit_file="$commit_file $file"
            fi
        fi
    fi
done

if [[ $2 == "-c" ]]; then
    svn commit $commit_file -m "auto commit"
fi
