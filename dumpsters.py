import paho.mqtt.client as mqtt #import the client1
import array
import ssl
import time
import json
import random

lon = [21.245747, 21.235286, 21.935248, 21.925008, 21.287130, 19.162700, 17.140321, 21.267170, 21.252949, 19.308381, 18.750834, 21.897280, 20.300528, 18.114146]
lat = [48.730378, 48.698507, 48.759624, 48.759571, 48.728504, 48.735416, 48.145809, 48.722399, 48.996299, 49.068310, 49.220796, 48.935660, 49.048762, 48.303434]
name = ["Letná 9", "Študentský domov Jedlíkova 5", "Konečná 3557/11", "Partizánska 1", "Povstania českého ľudu 739", "železničná stanica Banská Bystrica", "Prievozská 4", "železničná stanica Košice", "Fakultná nemocnica J.A. Reimana Prešov", "Bystrická cesta 5561", "Moyzesova 69", "Mierová 58", "Dostojevského 2805/23", "Dlhá 92"]

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
        

broker_address="192.168.0.99" 

client = mqtt.Client("DUMPSTER_DEVICE")

TLS_CA = "client.crt"

client.tls_set(cert_reqs=ssl.CERT_NONE, tls_version=ssl.PROTOCOL_TLSv1_2)
client.tls_insecure_set(True)

client.username_pw_set(username="iotdevice", password="AwmXczY995UuKSj6")

client.connect(broker_address, port=8883, keepalive=60)
client.on_message=on_message
client.subscribe("data/dumpster")
    
for i in range (14):

	DEVICEID = i+1

	print("Connected...")

	fill = (random.randint(0,100))		#fill urcuje percento naplnenia
	weight = fill * 10 + 100			#maximalne by mal kontajner uniest 1100kg cize na kolko percent je naplneny vynasobime carovnym cisielkom a pripocitame 100 :D
	if (random.randint(0,1) == 0):
		air = (random.randint(50,290))
	else:
		air = (random.randint(50,1000))
	smell = air - 13

	datajson =  '{ "DeviceID": '+str(DEVICEID)+', "Name": "'+name[i]+'", "Fill": '+str(fill)+', "Air": '+str(air)+', "Smell": '+str(smell)+', "Weight": '+str(weight)+', "Lon": '+str(lon[i])+', "Lat": '+str(lat[i])+'}'
	client.publish("data/dumpster",datajson)

client.loop_forever()
