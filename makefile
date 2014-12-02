# CC=$(CROSS_COMPILE)clang
# c compiler
CC=$(CROSS_COMPILE)gcc
# Date Base varible
DB=Bank.sqlite

# default make task
all: main.c sqlite3.c
	$(CC) main.c -lsqlite3 -o bank
# start application
start: bank
	./bank $(DB)
# delete executable files
clean: bank
	rm bank