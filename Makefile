# Uncomment lines below if you have problems with $PATH
SHELL := /bin/bash
PATH := /usr/bin:$(PATH)

all: run

upload:
	platformio -f -c vim run --target upload;
	sleep 1;
	screen /dev/ttyUSB0 115200;

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update

run:
	platformio -f -c vim run
