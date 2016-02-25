echo "---string pattern---"
file=usr/local/hello.xyz
filename=${file##*/}
echo $filename

echo "---string compare---"
str1="pre.abc"
str2="*.abc"
if [[ $str1 == $str2 ]];  then
    echo "yes"
fi
