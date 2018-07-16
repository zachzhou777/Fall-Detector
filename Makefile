CC = gcc
CFLAGS = -g -Wall
	
all: FallDetector.c
	$(CC) $(CFLAGS) -o FallDetectorTest FallDetector_Tester.c FallDetector.c -lm
