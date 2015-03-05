CFLAGS := -O2 -Wall --shared -fPIC

CC = gcc -std=gnu99

LUA_VERSION = 5.3.0
LUAINC = -Ilua-$(LUA_VERSION)/src

INC = $(LUAINC)

xlsread.so: lxlsread.c
	$(CC) $(CFLAGS) -o $@ $^ $(INC) -lxlsreader

clean:
	rm xlsread.so

