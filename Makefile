CC      = gcc
CXX     = g++
CFLAGS  = -g -O3 -Wall -Wextra -I../include -std=c++14 -DHAVE_LOG_H `pkg-config --cflags --libs libmosquitto`
LIBS    = 
LDFLAGS = -g

OBJECTS = main.o mqtt.o

all:		main

main:	$(OBJECTS)
		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o mqtt

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<


