# c compiler
# CC=$(CROSS_COMPILE)clang
CC=$(CROSS_COMPILE)gcc

# Varibles
DB=Bank.sqlite
LIB=lib
SRC=$(wildcard include/*.c)

CMD_ALL=$(CC) $^ -o bank

ifeq ($(OS), Windows_NT)
  CMD_RUN=bank.exe $(DB)
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
     CMD_ALL += -lpthread -ldl
  endif
  CMD_RUN=./bank $(DB)
endif

# default make task
all: main.c $(SRC) $(LIB)/sqlite3.c
	$(CMD_ALL)
# start application
run: bank
	$(CMD_RUN)
# delete executable files
clean: bank
	rm bank
