CC=g++
CFLAGS= -Wall -O3 -std=c++11 -g
LIBS=-L/home/esteban/opt/usr/lib/
INCLUDES= -I/home/esteban/opt/usr/include/
BINARY_NAME=main

OBJS=	phase_detector.o \
		mapping_finder.o \
		linear_system.o \
		utils.o 

%.o : %.cpp %.hpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -c 

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -c

all : $(OBJS) main.o
	$(CC) $(LIBS) $^ -o $(BINARY_NAME) -lm -larmadillo

clean:
	rm -f *.o $(BINARY_NAME)
