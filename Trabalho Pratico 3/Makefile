CC_OPTIONS := -O3
SEQUENTIAL_CC_OPTIONS := $(CC_OPTIONS) 
PARALLEL_CC_OPTIONS := $(CC_OPTIONS) -fopenmp
MIN := 100
MAX := 10000
NTHREADS := 2
BUILD_PATH := /tmp/programacao-concorrente-ufsc-2014-2
CC := gcc

.PHONY: execute
.PHONY: versao-sequencial
.PHONY: execute-versao-sequencial
.PHONY: versao-paralela
.PHONY: execute-versao-paralela
.PHONY: clean

all: compile execute

compile: versao-sequencial versao-paralela

versao-sequencial:
	mkdir -p $(BUILD_PATH)
	$(CC) -o $(BUILD_PATH)/versao-sequencial versao-sequencial.c $(SEQUENTIAL_CC_OPTIONS)

versao-paralela:
	mkdir -p $(BUILD_PATH)
	$(CC) -o $(BUILD_PATH)/versao-paralela versao-paralela.c $(PARALLEL_CC_OPTIONS)

execute-versao-sequencial:
	bash -c "time $(BUILD_PATH)/versao-sequencial $(MIN) $(MAX) > /dev/null"

execute-versao-paralela:
	bash -c "time $(BUILD_PATH)/versao-paralela $(MIN) $(MAX) $(NTHREADS) > /dev/null"

execute: execute-versao-sequencial execute-versao-paralela

clean: 
	rm $(BUILD_PATH)/versao-sequencial
	rm $(BUILD_PATH)/versao-paralela
