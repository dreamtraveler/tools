#!/usr/bin/env python
# coding=utf-8

import MySQLdb, datetime, time, collections
import os, os.path, logging
#gHost   = '192.168.254.114'
gHost   = '119.29.52.148'
gUser   = 'root'
gPasswd = 'root'
gLogDB  = 'bilog'
gGameDB = 'sgq'
gPort   = 3306
logger = logging.getLogger()

class Struct(object):
	def __init__(self, **data):
		self.__dict__.update(data)

#===========================
# 基本数据
# @dt 	日期
#===========================
def baseStat(dt):
    logger.info("-----base data-----")
    dtStart=dt
    dtEnd=dtStart+datetime.timedelta(1)
    startTime= time.mktime(dtStart.timetuple()) * 1000
    endTime= time.mktime(dtEnd.timetuple()) * 1000

    baseData = Struct(allRegister=0,newRegister=0)
    try:
        conn=MySQLdb.connect(host=gHost,user=gUser,passwd=gPasswd,db=gGameDB,port=gPort)
        cur=conn.cursor(cursorclass=MySQLdb.cursors.DictCursor)

        sqlStr='select count(id) as icount from account where create_time<='+str(endTime)
        cur.execute(sqlStr)
        for row in cur.fetchall():
            baseData.allRegister=row['icount']

        sqlStr='select count(id) as icount from account where create_time>='+str(startTime)+' and create_time<='+str(endTime)
        cur.execute(sqlStr)
        for row in cur.fetchall():
            baseData.newRegister=row['icount']


        cur.close()
        conn.close()
    except MySQLdb.Error,e:
        print "MySQL cur Error %d:%s"%(e.args[0],e.args[1])

    logger.info("%s\t%s\t%s" % ("date","allRegister","newRegister"))
    logger.info("%s\t%d\t%d" % (dt.strftime("%Y-%m-%d"),baseData.allRegister,baseData.newRegister))

#===========================
# 日存留率
# @dt 	日期
# @day 	几日存留
#===========================
def retention(dt):
    logger.info("-----retention----- from " + dt.strftime("%Y-%m-%d %H:%M:%S"))
    for dd in range(1,7):
        _retention(dt,dd)

def _retention(dt,day):
    try:
        conn = MySQLdb.connect(host=gHost,user=gUser,passwd=gPasswd,db=gLogDB,port=gPort)
        cur  = conn.cursor(cursorclass=MySQLdb.cursors.DictCursor)

        dtStart   = dt
        dtEnd     = dtStart+datetime.timedelta(1)
        startTime = time.mktime(dtStart.timetuple()) * 1000
        endTime   = time.mktime(dtEnd.timetuple()) * 1000

        beforePlayer = set()
        afterPlayer = set()
        sqlStr = 'select distinct uid from login where triggertime>=' + str(startTime) + ' and triggertime<=' + str(endTime)
        cur.execute(sqlStr)
        for row in cur.fetchall():
            rid=row['uid']
            beforePlayer.add(rid)
        if len(beforePlayer) == 0:
            return


        dtStart   = dt + datetime.timedelta(day)
        dtEnd     = dtStart+datetime.timedelta(1)
        startTime = time.mktime(dtStart.timetuple()) * 1000
        endTime   = time.mktime(dtEnd.timetuple()) * 1000
        sqlStr = 'select distinct uid from login where triggertime>=' + str(startTime) + ' and triggertime<=' + str(endTime)
        cur.execute(sqlStr)
        for row in cur.fetchall():
            rid = row['uid']
            afterPlayer.add(rid)
        cur.close()
        conn.close()
        if len(afterPlayer) == 0:
            return

        crossPlayer = beforePlayer&afterPlayer
        res =  "%.2f"%(float(len(crossPlayer)) * 100 /float(len(beforePlayer)))
        logger.info("%d day \t %s"%(day+1, res))

    except MySQLdb.Error,e:
        print "MySQL cur Error %d:%s"%(e.args[0],e.args[1])

def strByStamp(timeStamp):
    timeArray = time.localtime(timeStamp)
    otherStyleTime = time.strftime("%Y-%m-%d %H:%M:%S", timeArray)
    return otherStyleTime

def onlineCount(dt):
    logger.info("-----online num-----")
    try:
        conn = MySQLdb.connect(host=gHost,user=gUser,passwd=gPasswd,db=gLogDB,port=gPort)
        cur  = conn.cursor(cursorclass=MySQLdb.cursors.DictCursor)

        dtStart   = dt
        dtEnd     = dtStart+datetime.timedelta(1)
        startTime = time.mktime(dtStart.timetuple()) * 1000
        endTime   = time.mktime(dtEnd.timetuple()) * 1000

        sqlStr='select num, triggertime from online where triggertime>='+str(startTime)+' and triggertime<='+str(endTime)
        cur.execute(sqlStr)

        arrOnline=[]
        for row in cur.fetchall():
            record = Struct(triggertime=0,num=0)
            record.triggertime = row['triggertime']
            record.num = row['num']
            arrOnline.append(record)

        cur.close()
        conn.close()

        for v in arrOnline:
            logger.info("%s\t%d"%(strByStamp(v.triggertime/1000),v.num))

    except MySQLdb.Error,e:
        print "MySQL cur Error %d:%s"%(e.args[0],e.args[1])

#===========================
# 一天内打点按组计数
#===========================
def _dailyCount(dt, sqlStr):
    try:
        conn = MySQLdb.connect(host=gHost,user=gUser,passwd=gPasswd,db=gLogDB,port=gPort)
        cur  = conn.cursor(cursorclass=MySQLdb.cursors.DictCursor)

        idCount={}
        cur.execute(sqlStr)
        for row in cur.fetchall():
            id = row['id']
            cnt = row['cnt']
            idCount[id] = cnt;

        if len(idCount) == 0:
            return

        logger.info("%s\t%s\t%s" % ("date","id","count"))
        stageList = sorted(idCount.iteritems(), key=lambda asd:asd[0])
        for k,v in stageList:
            logger.info("%s\t%d\t%d"%(dt.strftime("%Y-%m-%d"),k,v))

    except MySQLdb.Error,e:
        print "MySQL cur Error %d:%s"%(e.args[0],e.args[1])

#===========================
# 关卡进度
#===========================
def stageProgress(dt):
    logger.info("-----stage progress-----")
    dtStart=dt
    dtEnd=dtStart+datetime.timedelta(1)
    startTime= time.mktime(dtStart.timetuple()) * 1000
    endTime= time.mktime(dtEnd.timetuple()) * 1000
    sqlStr = 'select count(*) as cnt, stageid as id from stage where triggertime>=' + str(startTime) + ' and triggertime<' + str(endTime) + ' group by stageid'
    _dailyCount(dtStart,sqlStr);

#===========================
# 在线礼包进度
#===========================
def onlineGiftProgress(dt):
    logger.info("-----online gift progress-----")
    dtStart=dt
    dtEnd=dtStart+datetime.timedelta(1)
    startTime= time.mktime(dtStart.timetuple()) * 1000
    endTime= time.mktime(dtEnd.timetuple()) * 1000
    sqlStr = 'select count(*) as cnt, giftid as id from onlinegift where triggertime>=' + str(startTime) + ' and triggertime<' + str(endTime) + ' group by giftid'
    _dailyCount(dtStart,sqlStr);

#===========================
# 新手引导
#===========================
def guideProgress(dt):
    logger.info("-----guide progress-----")
    dtStart=dt
    dtEnd=dtStart+datetime.timedelta(1)
    startTime= time.mktime(dtStart.timetuple()) * 1000
    endTime= time.mktime(dtEnd.timetuple()) * 1000
    sqlStr = 'select count(*) as cnt, guideid as id from guide where triggertime>=' + str(startTime) + ' and triggertime<' + str(endTime) + ' group by guideid'
    _dailyCount(dtStart,sqlStr);

def allProgress(dt):
    stageProgress(dt)
    onlineGiftProgress(dt)
    guideProgress(dt)


def clearLog(logPath):
    for root, dirs, files in os.walk(logPath):
        for filepath in files:
            if filepath[-4:] == ".txt":
                os.remove(os.path.join(root,filepath))

handlers = []
def setLog(logPath, dt):
    dt.strftime("%Y-%m-%d")
    logfile = logPath + dt.strftime("%Y-%m-%d") + "_log.txt"
    for v in handlers:
        logger.removeHandler(v)

    hdlr = logging.FileHandler(logfile)
    logger.addHandler(hdlr)
    handlers.append(hdlr)
    logger.setLevel(logging.NOTSET)

def main(dt,day):
    logPath = "./bi/"
    if not os.path.exists(logPath):
        os.makedirs(logPath)

    clearLog(logPath)
    for dd in range(0,day):
        dtStart = dt+datetime.timedelta(dd)
        setLog(logPath, dtStart)
        baseStat(dtStart)
        retention(dtStart)
        allProgress(dtStart)
        onlineCount(dtStart)

main(datetime.datetime(2016,6,20),4)
