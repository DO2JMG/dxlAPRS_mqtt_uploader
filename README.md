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
-ma             mqtt server address
-mp             mqtt server port

-mp             mqtt server subscribe
```

