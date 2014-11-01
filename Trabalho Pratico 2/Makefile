LDLIBS=-lpthread
BUILD_PATH := /tmp/programacao-concorrente-ufsc-2014-2
CC := gcc
.PHONY: trabalho2
all: execute
trabalho2:
	$(CC) -o $(BUILD_PATH)/trabalho2 trabalho2.c queue.c $(LDLIBS)

execute: trabalho2
	$(BUILD_PATH)/trabalho2
