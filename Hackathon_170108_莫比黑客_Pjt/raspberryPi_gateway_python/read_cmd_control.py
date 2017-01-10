#! /usr/bin/python
# -*- coding: utf-8 -*-
BUFSIZE=1024
import socket
import time
import os

def send_color_to_board(ip, port,message):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server_address = (ip, port)
	#print 'Connecting to %s:%s.' % server_address
	sock.connect(server_address)
	sock.sendall(message)
	#print "close the socket"
	sock.close()
if __name__ == "__main__":
	message_bak='GRB000000000\r\n'
	while True:
		f1 = open('/tmp/color','r')
		message = f1.read()
		f1.close()
		if message_bak == message:
			continue
		for i in range(5) :
			try:
				send_color_to_board("192.168.31.198", 8080,message)
				send_color_to_board("192.168.31.140", 8080,message)
				send_color_to_board("192.168.31.141", 8080,message)
				send_color_to_board("192.168.31.168", 8080,message)
				send_color_to_board("192.168.31.106", 8080,message)
				send_color_to_board("192.168.31.112", 8080,message)
			except:
				pass
			time.sleep(0.2)
		message_bak=message
	
