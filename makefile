# c compiler
# CC=$(CROSS_COMPILE)clang
CC=$(CROSS_COMPILE)gcc

# Varibles
DB=Bank.sqlite
LIB=lib
SRC=$(wildcard include/*.c)

# default make task
all: main.c $(SRC) $(LIB)/sqlite3.c
	$(CC) main.c $(SRC) $(LIB)/sqlite3.c -o bank
# start application
run: bank
	./bank $(DB)
# delete executable files
clean: bank
	rm bank