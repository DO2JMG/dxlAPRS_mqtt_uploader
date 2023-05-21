#include "mqtt.h"

#include <chrono>
#include <thread>
#include <iostream>

#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#define MAXLINE 1024

using namespace std::chrono_literals;
using namespace std;

struct uploader_config {
  int udpport;
  int mqttport;
  double latitude;
  double longitude;
  string mqttaddress;
  string mqtt_prefix_sondedata;
  string mqtt_prefix_beacon;
  string call;
};

int sockfd;
struct sockaddr_in servaddr, cliaddr;

uploader_config config;

class mqttclient: public MQTT
{
public:
    mqttclient() {
        connect(config.mqttaddress, config.mqttport); 
    }

private: // MQTT
    virtual void onConnected(int /*rc*/) override {
        std::cout << "Connected to MQTT broker" << std::endl;

        subscribe(config.mqtt_prefix_sondedata, MQTT::QoS::QoS0);
    }
    
    virtual void onMessageReceived(const struct mosquitto_message& msg) override {
        std::string message(static_cast<char*>(msg.payload));
        std::string topic(static_cast<char*>(msg.topic));
    }
};

void readandsend() {
    mqttclient client; 

    while (true) {
        char buffer[MAXLINE];

        socklen_t len;
        int n;
   
        len = sizeof(cliaddr);  
   
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';

        client.subscribe(config.mqtt_prefix_sondedata, MQTT::QoS::QoS0);
        client.publish(config.mqtt_prefix_sondedata, buffer, MQTT::QoS::QoS0);
    }
}

void uploadStationInfos()
{   
  while (true) {
    mqttclient client;  
    
    string strOut;

    strOut += "{";
    strOut += "\"call\": \"" + config.call + "\",";
    strOut += "\"latitude\": " + to_string(config.latitude) + ",";
    strOut += "\"longitude\": " + to_string(config.longitude) + "";
    strOut += "}";

    client.subscribe(config.mqtt_prefix_beacon, MQTT::QoS::QoS0);
    client.publish(config.mqtt_prefix_beacon, strOut.c_str(), MQTT::QoS::QoS0);

    sleep(60);
    
  }
}

int main(int argc, char *argv[]) {
    for (int i = 0; i<argc; i++) {
        if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"-help") == 0) {
            cout << "\n----------------------------------------------------\n";
            cout << "dxlAPRS mqtt uploader\n";
            cout  << "Copyright (c) 2023 by Jean-Michael Grobel (Do2JMG)\n";
            cout << "----------------------------------------------------\n\n";
            cout << "Parameter\tDescription\n\n";
            cout << "-h\t\tthis help\n";
            cout << "-p\t\tudp receive port\n";
            cout << "-lat\t\thome location latitude\n";
            cout << "-lon\t\thome location longitude\n";
            cout << "-call\t\tcallsign\n";
            cout << "-ma\t\tmqtt server address\n";
            cout << "-mp\t\tmqtt server port\n";
            cout << "-mb\t\tmqtt server prefix station beacon\n";
            cout << "-ms\t\tmqtt server prefix sondedata\n\n";
            return 0;
        }
        if (strcmp(argv[i],"-p") == 0) {
            if(i+1 < argc) {
                config.udpport = stoi(argv[i+1]);
            } else {
                cout << "Error : port\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-mp") == 0) {
            if(i+1 < argc) {
                config.mqttport = stoi(argv[i+1]);
            } else {
                cout << "Error : mqtt port\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-ma") == 0) {
            if(i+1 < argc) {
                config.mqttaddress = argv[i+1];
            } else {
                cout << "Error : mqtt address\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-ms") == 0) {
            if(i+1 < argc) {
                config.mqtt_prefix_sondedata = argv[i+1];
            } else {
                cout << "Error : mqtt prefix\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-lat") == 0) {
            if(i+1 < argc) {
                config.latitude = stod(argv[i+1]);
            } else {
                cout << "Error : latitude\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-lon") == 0) {
            if(i+1 < argc) {
                config.longitude = stod(argv[i+1]);
            } else {
                cout << "Error : longitude\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-call") == 0) {
            if(i+1 < argc) {
                config.call = argv[i+1];
            } else {
                cout << "Error : call\n"; 
                return 0;
            }
        } 
        if (strcmp(argv[i],"-mb") == 0) {
            if(i+1 < argc) {
                config.mqtt_prefix_beacon = argv[i+1];
            } else {
                cout << "Error : station info\n"; 
                return 0;
            }
        } 
    }
       
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Error : socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(config.udpport);
       
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  sizeof(servaddr)) < 0 ) {
        perror("Error : bind failed");
        exit(EXIT_FAILURE);
    }
    
   
    thread thread_readwrite (readandsend); 
    thread thread_beacon (uploadStationInfos); 
    thread_readwrite.join();
    thread_beacon.join();
    
    return 0;
}
