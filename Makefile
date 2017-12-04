# Uncomment lines below if you have problems with $PATH
SHELL := /bin/bash
PATH := /usr/bin:$(PATH)
BIN := .pioenvs/wemos_d1_mini/firmware.bin
WEBNAME := wasserkerze.local
WEBPATH := firmware

all: run

webupdate: run
	curl -F "file=@$(BIN)" --user `cat user.cred` $(WEBNAME)/$(WEBPATH) > /dev/null

upload:
	platformio -f -c vim run --target upload;
	sleep 1;

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update: 
	platformio -f -c vim update

run: $(BIN)
	platformio -f -c vim run
