#!/usr/bin/python
# This Python file uses the following encoding: utf-8
import subprocess
import time
import os

def monitor_process(key_word):
    print('monitor_process start')
    p1 = subprocess.Popen(['ps', '-ef'], stdout=subprocess.PIPE)
    p2 = subprocess.Popen(['grep', key_word], stdin=p1.stdout, stdout=subprocess.PIPE)
    p3 = subprocess.Popen(['grep', '-v', 'grep'], stdin=p2.stdout, stdout=subprocess.PIPE)
    lines = p3.stdout.readlines()
    if len(lines) > 0:
        return True
    print "process[%s] is lost, run [%s]\n" % (key_word,key_word)
    return False
    print('monitor_process end')

'''重启脚本bash'''
cmd='su root server_restart.sh'
if __name__ == '__main__':

	while 1:
		n=10
		if	not (monitor_process('worldserver') and monitor_process('gateway') and monitor_process('dbserver') and monitor_process('loginserver')):
			subprocess.call(cmd, shell=True)
			n=90
		time.sleep(n)
