import paho.mqtt.client as mqtt
import array
import ssl
import time
import json
import random

max = [20, 49, 69, 120, 50, 30, 82, 32, 10, 80, 110, ]
lon = [21.261622, 21.238355, 21.241406, 21.235444, 21.915625, 21.917625, 21.915762, 21.263740, 21.246264, 19.148915, 20.530470, 17.098192, 21.242796, 18.739219]
lat = [48.723837, 48.693893, 48.727751, 48.698400, 48.752358, 48.757016, 48.755385, 48.717763, 48.730168, 48.734608, 48.657666, 48.142614, 48.992575, 49.195457]
name = ["Mestská plaváreň Košice", "Optima", "Amfiteáter", "Študentský domov Jedlíkova 5", "Nemocnica Svet zdravia Michalovce", "Poliklinika Michalovce", "Sama Chalupku 1", "Aupark Košice", "Letná 9", "Parkovisko Múzea SNP", "Šafárikova 3826/50", "Zámocká 1/A", "Námestie legionárov 15267/1", "Prielohy 1"]

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
        
broker_address="192.168.0.99" 

client = mqtt.Client("PARKING_DEVICE")

TLS_CA = "client.crt"

client.tls_set(cert_reqs=ssl.CERT_NONE, tls_version=ssl.PROTOCOL_TLSv1_2)
client.tls_insecure_set(True)

client.username_pw_set(username="iotdevice", password="AwmXczY995UuKSj6")

client.connect(broker_address, port=8883, keepalive=60)
client.on_message=on_message
client.subscribe("data/parking")

print("Connected...")

for i in range (14):
	DEVICEID = i+1
	free = (random.randint(0,max[i]))

	datajson =  '{ "DeviceID": '+str(DEVICEID)+', "Max": '+str(max[i])+', "Name": "'+name[i]+'", "Free": '+str(free)+', "Lon": '+str(lon[i])+', "Lat": '+str(lat[i])+'}'
	client.publish("data/parking",datajson)

client.loop_forever()