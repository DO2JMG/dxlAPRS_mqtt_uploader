# dxlAPRS_mqtt_uploader
Simple MQTT-Uploader for dxlAPRS


## Install the required dependencies :

```
sudo apt install libmosquitto-dev 
```

### Unpack and compile :

```
  git clone https://github.com/DO2JMG/dxlAPRS_mqtt_uploader.git
  cd dxlAPRS_mqtt_uploader
  make
```

### Use help :

```
  mqtt -h
```
```
Parameter       Description

-h              this help
-p              udp receive port
-lat            home location latitude
-lon            home location longitude
-call           callsign
-ma             mqtt server address
-mp             mqtt server port
-mb             mqtt server prefix station beacon
-ms             mqtt server prefix sondedata

```
Example :

```
./mqtt -p 4020 -ma mqttdomain.org -mp 1883 -ms radiosonde/sondedata
```
