#-*-coding:utf-8-*-
import serial
import requests

t = serial.Serial("/dev/cu.wchusbserial1410",115200)

d = ['']  ##device list
url = 'http://127.0.0.1:5000/updatedetail'
s = set()

while True:
	tmp = t.readline()
	mac = tmp[:17]
	for item in d:
		if not item in s and mac == item:
			print item
			data = {
				'macaddr':mac.replace(':', '-'),
				'latitude':'39',
				'longitude':'116'
			}
			res = requests.post(url, data=data)
			# print res.text
