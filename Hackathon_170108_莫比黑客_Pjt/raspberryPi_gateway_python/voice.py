#!/usr/bin/python
#--*--coding:utf-8 -*-
from voicetools import BaiduVoice
from gevent import monkey;monkey.patch_socket()
import sys
from write_color import write_color
from single_stm32_control import *
reload(sys)
sys.setdefaultencoding('utf8')
import pygame
import subprocess
# api key 及 secret key 请在百度语音官方网站注册获取
token = BaiduVoice.get_baidu_token('Hpycs4wIZtXGb8KcW1aypGP0', '9b0ed628113a732e2709170fe6400c05')  # 该方法返回百度 API 返回的完整 json
bv = BaiduVoice(token['access_token'])  # 在上述方法获取的 json 中得到 access_token
# 语音识别     
cRed = 255
cGreen = 255
cBlue = 255
colors = [
	['红色',255,0,0],
	['绿色',0,255,0],
	['蓝色',0,0,255],
	['橙色',256,165,0],
	['黄色',255,255,0],
	['青色',0,255,255],
	['紫色',128,0,128]
	]
def chk_rst(rst):
    global cRed, cGreen, cBlue
    print '开始检测....'
    if rst.encode('utf8').find(u'关灯')!=-1:
        cRed, cGreen, cBlue = 0,0,0 
	#sock = Connect_board("192.168.31.106",8080)  
        #Send_board_cmd(sock, 10,10,10)
	#Close_sock(sock) 
    if rst.encode('utf8').find(u'开灯')!=-1:
        cRed, cGreen, cBlue = 255,255,255 
    if rst.encode('utf8').find(u'白色')!=-1:
        cRed, cGreen, cBlue = 255,255,255 
    if rst.encode('utf8').find(u'粉红')!=-1:
        cRed, cGreen, cBlue = 255,192,203 
	#sock = Connect_board("192.168.31.106",8080)  
    if rst.encode('utf8').find(u'亮一点')!=-1:
        cRed += 50
        cGreen += 50
        cBlue += 50
    if rst.encode('utf8').find(u'橙色')!=-1:
        cRed += 255
        cGreen += 255
        cBlue += 0
    if rst.encode('utf8').find(u'紫色')!=-1:
        cRed += 255
        cGreen += 0
        cBlue += 255
    if rst.encode('utf8').find(u'青色')!=-1:
        cRed += 0
        cGreen += 255
        cBlue += 255
    if rst.encode('utf8').find(u'太亮了')!=-1:
        cRed -= 20
        cGreen -= 20
	#sock = Connect_board("192.168.31.106",8080)  
    if rst.encode('utf8').find(u'亮一点')!=-1:
        cRed += 50
        cGreen += 50
        cBlue += 50
    if rst.encode('utf8').find(u'橙色')!=-1:
        cRed += 255
        cGreen += 255
        cBlue += 0
    if rst.encode('utf8').find(u'紫色')!=-1:
        cRed += 255
        cGreen += 0
        cBlue += 255
    if rst.encode('utf8').find(u'青色')!=-1:
        cRed += 0
        cGreen += 255
        cBlue += 255
    if rst.encode('utf8').find(u'太亮了')!=-1:
        cRed -= 20
        cGreen -= 20
        cBlue -= 20
    if rst.encode('utf8').find(u'红色')!=-1:
        cRed = 255
        cGreen = 0
        cBlue == 0
    if rst.encode('utf8').find(u'绿色')!=-1:
        cRed = 0
        cGreen = 255
        cBlue = 0

    if rst.encode('utf8').find(u'蓝色')!=-1:
        cRed = 0 
        cGreen = 0
        cBlue = 255

    f1=open('/tmp/color','w')
    Green = '%03d' %cGreen
    Red = '%03d' %cRed
    Blue = '%03d' %cBlue
    Color = 'GRB'+Green+Red+Blue+'\r\n'


    f1.write(Color)
    f1.close()



from multiprocessing.dummy import Pool as ThreadPool

while 1:
    print '====开始录音====='
    subprocess.call('arecord -D "plughw:1,0" -d 2 -r 16000 -f S16_LE ./test.wav', shell=True)
    print '====录音结束====='
    print '====开始识别====='
    try:
        results = bv.asr('test.wav')  # 返回识别结果列表，可选参数见百度语音文档
        print '====识别结束=='
        if len(results)>0:
            rst =  results[0]
            chk_rst(rst)
            print "-->",rst
    except:
        print 'not reg....'

