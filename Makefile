

CC=clang
AR=ar

COMMON:=$(wildcard src/*.c)
RUNNERFILES:=$(wildcard src/runner/*.c)
LIBFILES=src/library/c.c src/library/ld.c src/library/master.c \
src/library/c/clang.c
LIBOBJFILES = $(LIBFILES:.c=.o)

all:
	bin/god build
	mv bin/god_staging bin/god

help:
	@echo "make            compiles using god, requires minimal installtion"
	@echo "make full       full installation of build system"
	@echo "make minimal    minimal installation, contains only C and ld"
	@echo "make rebuild    rebuild full installation"
	@echo "make clean      cleans the project"
	@echo "make remove     removes the project"

full: minimal all


bin/:
	mkdir -p .god
	mkdir -p bin

compiledmodules:
	touch compiledmodules.txt
	echo "MODULE(c, compiler) SUB(c, clang) MODULE(ld, compiler)" > compiledmodules.txt

minimal: compiledmodules bin/ buildrunner
	cp -r include/ bin/

buildrunner: libgod.a $(RUNNERFILES) $(COMMON)
	$(CC) $(RUNNERFILES) $(COMMON) -g -o bin/god -Lbin -lgod -Iinclude \
	-Dminimal

src/library/%.o: src/library/%.c
	$(CC) -c $< -g -o $@ -Iinclude -Dminimalcc

src/common.o:
	$(CC) -c src/common.c -g -o src/common.o -Iinclude

libgod.a: $(LIBOBJFILES) src/common.o
	$(AR) rcs bin/libgod.a $(LIBOBJFILES) src/common.o

clean:
	-find . -name "*.o" | xargs rm

remove:
	-rm -rf .god
	-rm -rf bin
	-find . -name "*.o" | xargs rm


rebuild: remove full