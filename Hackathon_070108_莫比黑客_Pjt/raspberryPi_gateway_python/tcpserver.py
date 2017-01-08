#! /usr/bin/env python
import threading
from socket import *
from time import ctime 
import os
def create_tcp_server(port):
	HOST=''
	BUFSIZ = 1024
	ADDR = (HOST,port)
	tcpSerSock = socket(AF_INET, SOCK_STREAM)
	tcpSerSock.bind(ADDR)
	tcpSerSock.listen(5) 
	while True:
		print 'waiting for connection...'
		tcpCliSock, addr = tcpSerSock.accept()
		print '....connected from :',addr
		while True:
			data = tcpCliSock.recv(BUFSIZ)
			if not data:
				break
			#print data
			f1 = open('/tmp/color','w')
			f1.write(data)
			f1.close()
			#send_color_Buf_to_board("192.168.31.198", 8080,data)
			#print len(data)
		tcpCliSock.close()

if __name__ == "__main__":
	print "start control"
	create_tcp_server(8000)
