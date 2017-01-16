#-*-coding:utf-8-*-

from flask import Flask, request, jsonify
import json
import os

app = Flask(__name__)
app.debug = True

@app.route('/')
def index():
	pass

@app.route('/register', methods=['POST'])
def register():
	form = request.form
	username = form['username']
	password = form['password']

	return jsonify(res='00000')

@app.route('/querydevice', methods=['POST'])
def querydevice():
	
	form = request.form
	username = form['username']

	if os.path.exists('%s.txt'%username):
		f = open('%s.txt'%username)
		devices = []
		for line in f:
			tmp = line.strip().split('\t')
			device = {
				'name':tmp[0],
				'macaddr':tmp[1],
				'type':tmp[2]
			}
			devices.append(device)
		f.close()
		res = {
			'res':'00000',
			'devices':devices
		}
		return json.dumps(res)
	else:
		return jsonify(res='10001')

@app.route('/adddevice', methods=['POST'])
def adddevice():
	form = request.form
	username = form['username']
	macaddr = form['macaddr'].upper().replace('-', ':')
	name = form['name']
	n_type = form['type']

	print name

	f = open('%s.txt'%username, 'a')
	f.write('%s\t%s\t%s\n'%(name, macaddr, n_type))
	f.close()
	return jsonify(res='00000')


@app.route('/querydetail', methods=['POST'])
def querytetail():
	form = request.form
	username = form['username']
	macaddr = form['macaddr']
	print macaddr
	if os.path.exists('%s.txt'%macaddr.replace(':', '-')):
		f = open('%s.txt'%macaddr.replace(':', '-'))
		tmp = f.read().split()
		latitude = float(tmp[0])
		longitude = float(tmp[1])
		res = {
			'res':'00000',
			'latitude':latitude,
			'longitude':longitude
		}
		return json.dumps(res)
	else:
		return jsonify(res='10001')

@app.route('/updatedetail', methods=['POST'])
def updatedetail():
	form = request.form
	macaddr = form['macaddr']
	latitude = form['latitude']
	longitude = form['longitude']
	f = open('%s.txt'%macaddr, 'w')
	f.write('%s %s\n'%(latitude, longitude))
	f.close()
	return jsonify(res='00000')

@app.route('/login', methods=['POST'])
def login():
	form = request.form
	username = form['username']
	password = form['password']
	print username, password
	if username == 'zengzhaoyang' and password == 'aaaa':
		return '1'
	return '0'

@app.route('/deletedevice', methods=['POST'])
def deletedevice():
	form = request.form
	username = form['username']
	macaddr = form['macaddr'].replace('-', ':')

	f = open('%s.txt'%username, 'r')
	tmp = []
	for line in f:
 		if macaddr in line:
			pass
		else:
			tmp.append(line.strip())
	f.close()
	f = open('%s.txt'%username, 'w')
	for item in tmp:
		f.write('%s\n'%item)
	f.close()
	return jsonify(res='00000')

if __name__ == '__main__':
	app.run('0.0.0.0', 5000)