#!/usr/bin/python

port = "/dev/ttyACM0" #default device under which TI Launchpad shows up
baud = 9600 #default baud used in uC firmware


import serial, re, datetime

datapattern = '([0-9A-F]{16})\s([+-][0-9]{3}\.[0-9]{4})'
therminterface = serial.Serial(port, baud, timeout=5)

while True:
	therminterface.write("x") #therminterface wont send us any data until we request it, by sending anything
	now = datetime.datetime.now()
	rawdata = therminterface.read(9999)
	sensors = re.findall(datapattern, rawdata)
	datestring = now.strftime("%Y-%m-%d %H:%M:%S")
	for sensor in sensors:
		serialnumber = sensor[0]
		temperature = sensor[1]
		print(datestring + ' ' + serialnumber + ' ' + temperature)
		sensorfile=open(serialnumber, 'a+')
		sensorfile.write(datestring + ', ' + temperature + '\n')
		sensorfile.close()
	print('\n')

therminterface.close()