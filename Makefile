CC      = gcc
CXX     = g++
CFLAGS  = -g -O3 -Wall -Wextra -pthread -I../include -std=c++14 -DHAVE_LOG_H `pkg-config --cflags --libs libmosquitto`
LIBS    = -lpthread 
LDFLAGS = -g

OBJECTS = main.o mqtt.o

all:		main

main:	$(OBJECTS)
		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o mqtt

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<


