# 


CC=gcc
AR=ar
LD=ld

COMMON:=$(wildcard src/*.c)
RUNNERFILES:=$(wildcard src/runner/*.c)
LIBFILES:=$(wildcard src/library/*.c)
LIBFILES+=$(wildcard src/library/C/*.c)
LIBOBJFILES = $(LIBFILES:.c=.o)

all: bin/ buildrunner 
	cp -r include/ bin/
	rm -r .god

bin/:
	mkdir -p .god
	mkdir -p bin

buildrunner: libgod.a $(RUNNERFILES) $(COMMON)
	$(CC) $(RUNNERFILES) $(COMMON) -g -o bin/god -Lbin -lgod -Iinclude

src/library/%.o: src/library/%.c
	$(CC) -c $< -g -o $@ -Iinclude

src/common.o:
	$(CC) -c src/common.c -g -o src/common.o -Iinclude

libgod.a: $(LIBOBJFILES) src/common.o
	$(AR) rcs bin/libgod.a $(LIBOBJFILES) src/common.o

clean:
	find . -name "*.o" | xargs rm