# 


CC=gcc
AR=ar
LD=ld

COMMON:=$(wildcard src/*.c)
RUNNERFILES:=$(wildcard src/runner/*.c)
LIBFILES:=$(wildcard src/library/*.c)
LIBOBJFILES = $(LIBFILES:.c=.o)

all: bin/ libgod.a buildrunner 
	cp -r include/ bin/
	rm -r .god

bin/:
	mkdir -p .god
	mkdir -p bin

buildrunner: $(RUNNERFILES) $(COMMON)
	$(CC) $(RUNNERFILES) $(COMMON) -g -o bin/god -Lbin -lgod

src/library/%.o: src/library/%.c
	$(CC) -c $< -g -o $@

src/common.o:
	$(CC) -c src/common.c -g -o src/common.o

libgod.a: $(LIBOBJFILES) src/common.o
	$(AR) rcs bin/libgod.a $(LIBOBJFILES) src/common.o

clean:
	rm -rf $(LIBOBJFILES) .god bin
	find . -name "*.o" | xargs rm