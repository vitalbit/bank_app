# c compiler
# CC=$(CROSS_COMPILE)clang
CC=$(CROSS_COMPILE)gcc

# Varibles
DB=Bank.sqlite
LIB=lib
SRC=$(wildcard include/*.c)

ifeq ($(OS), Windows_NT)
	CMD_RUN=bank.exe $(DB)
else
	CMD_RUN=./bank $(DB)
endif

# default make task
all: main.c $(SRC) $(LIB)/sqlite3.c
	$(CC) $^ -o bank -lpthread -ldl
# start application
run: bank
	$(CMD_RUN)
# delete executable files
clean: bank
	rm bank
