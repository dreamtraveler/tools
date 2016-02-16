#################################################################################
#platform linux	SHELL								#
#brief 	  shut down for server group						#
#date  	  2016-1-13								#
#################################################################################

#!/bin/sh

function close_server()
{
    svr=`ps -e | grep $1 | awk '{print $1}'`
    if [ -n "$svr"  ] ; then
        kill -9 $svr
        echo "kiil $1 process-${svr}"
    else
        echo "$1 is not running!"
    fi
}

close_server gamesvr
close_server loginsvr
close_server agentsvr
close_server worldsvr
close_server idsvr
close_server fightsvr
close_server mapsvr
close_server dbsvr
#close_server logsvr
