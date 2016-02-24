#!/bin/bash
for name in $(cat .gitignore); do
	find ./ -name $name | xargs rm -rf;
done
