import paho.mqtt.client as mqtt #import the client1
import array
import ssl
import time
import json
import random

lat = [48.724152, 48.722977, 48.721922, 48.734866, 48.720311, 48.720512, 48.758471, 48.754748, 48.761687, 48.314963, 48.150150, 48.328458, 48.990719, 48.736755]
lon = [21.264971, 21.262428, 21.264885, 21.236794, 21.237295, 21.229823, 21.916724, 21.920924, 21.917014, 18.038689, 17.107921, 19.649623, 21.225716, 19.137876]
name = ["Mestský park 1", "Mestský park 2", "Mestský park 3", "Mánesova 1889/23", "Ondavská 15", "Katkin park", "Park študentov", "Park Mieru", "26. Novembra", "Park v Kyneku", "Prezidentská záhrada", "Mestský park", "Kolmanova záhrada", "Janka Kráľa 4440"]

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)

broker_address="192.168.0.99" 

client = mqtt.Client("PARKS_DEVICE")

TLS_CA = "client.crt"

client.tls_set(cert_reqs=ssl.CERT_NONE, tls_version=ssl.PROTOCOL_TLSv1_2)
client.tls_insecure_set(True)

client.username_pw_set(username="iotdevice", password="AwmXczY995UuKSj6")

client.connect(broker_address, port=8883, keepalive=60)
client.on_message=on_message
client.subscribe("data/parks")
        

for i in range (14):
    DEVICEID = i+1

    print("Connected...")

    hum = (random.randint(0,100))       #humidity je vlhkost pody, nie vlhkost vzduchu
    temp = (random.randint(0,18))        

    datajson =  '{ "DeviceID": '+str(DEVICEID)+', "Name": "'+name[i]+'", "Humidity": '+str(hum)+', "Temperature": '+str(temp)+', "Lon": '+str(lon[i])+', "Lat": '+str(lat[i])+'}'
    client.publish("data/parks",datajson)

client.loop_forever()