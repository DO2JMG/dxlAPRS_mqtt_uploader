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
### Changing parameters for sondemod

```
  -J <ip>:<port>
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
./mqtt -p 54321 -ma yourdomain.net -mp 1883 -ms rs/incoming -mb rs/beacon -lat 52.1234 -lon 8.1234 -call DO2JMG
```
