PARALLEL_CC_OPTIONS := -fopenmp -O3
SERIAL_CC_OPTIONS := -O3
MIN := 1000
MAX := 100000
NTHREADS := 4
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
	$(CC) -o $(BUILD_PATH)/versao-sequencial versao-sequencial.c $(SERIAL_CC_OPTIONS)

versao-paralela:
	mkdir -p $(BUILD_PATH)
	$(CC) -o $(BUILD_PATH)/versao-paralela versao-paralela.c $(PARALLEL_CC_OPTIONS)

execute-versao-sequencial:
	bash -c "time $(BUILD_PATH)/versao-sequencial $(MIN) $(MAX)"

execute-versao-paralela:
	bash -c "time $(BUILD_PATH)/versao-paralela $(MIN) $(MAX) $(NTHREADS)"

execute: execute-versao-sequencial execute-versao-paralela

compare: compile
	bash -c "$(BUILD_PATH)/versao-sequencial $(MIN) $(MAX) | tee $(BUILD_PATH)/versao-sequencial.log"; \
	bash -c "$(BUILD_PATH)/versao-paralela $(MIN) $(MAX) $(NTHREADS) | tee $(BUILD_PATH)/versao-paralela.log"; \
	echo "---------------- Comparando ----------------";\
	diff $(BUILD_PATH)/versao-sequencial.log $(BUILD_PATH)/versao-paralela.log;\
	echo "---------------- Terminou de comparar ----------------"

clean: 
	rm $(BUILD_PATH)/versao-sequencial
	rm $(BUILD_PATH)/versao-paralela