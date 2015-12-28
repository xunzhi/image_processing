# Makefile for image processing
# Xunzhi Wang
# wxzpursue@gmail.com

# Define compiler and linker
CC=g++ -Wall -g
LN=g++ -Wl,--stack,30000000

all: resize

jpg:
	@echo Compiling jpge and jpgd
	$(CC) -c jpge.cpp jpgd.cpp
	@echo **********

util: jpg
	@echo Compiling utilities
	$(CC) -c util.cpp
	@echo **********

resize: util
	@echo Compiling resizing
	$(CC) -c resize.cpp
	$(LN) -o resize resize.o util.o jpge.o jpgd.o
	@echo **********

clean:
	del *.o *.exe