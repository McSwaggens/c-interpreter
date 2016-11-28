CC=g++
CFLAGS=-Wall -w -fpermissive -g -DUNIX_HOST -DVER=\"`svnversion -n`\"
LIBS=-lm -lreadline -lsfml-graphics -lsfml-window -lsfml-system

TARGET	= cint
SRCS	= picoc.cpp table.cpp lex.cpp parse.cpp expression.cpp heap.cpp type.cpp system_gpu.cpp \
	variable.cpp clibrary.cpp platform.cpp include.cpp debug.cpp \
	platform/platform_unix.c platform/library_unix.c \
	cstdlib/standard.c cstdlib/math.c cstdlib/gpu.c
OBJS	:= $(SRCS:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

test:	all
	(cd tests; make test)

clean:
	rm -f $(TARGET) *.o


picoc.o: picoc.cpp picoc.h system_gpu.h
system_gpu.cpp: system_gpu.cpp system_gpu.h
table.o: table.cpp interpreter.h platform.h
lex.o: lex.cpp interpreter.h platform.h
parse.o: parse.cpp picoc.h interpreter.h platform.h
expression.o: expression.cpp interpreter.h platform.h
heap.o: heap.cpp interpreter.h platform.h
type.o: type.cpp interpreter.h platform.h
variable.o: variable.cpp interpreter.h platform.h
clibrary.o: clibrary.cpp picoc.h interpreter.h platform.h
platform.o: platform.cpp picoc.h interpreter.h platform.h
include.o: include.cpp picoc.h interpreter.h platform.h
debug.o: debug.cpp interpreter.h platform.h
platform/platform_unix.o: platform/platform_unix.c picoc.h interpreter.h platform.h
platform/library_unix.o: platform/library_unix.c interpreter.h platform.h
cstdlib/standard.o: cstdlib/standard.c interpreter.h platform.h
cstdlib/gpu.o: cstdlib/gpu.c interpreter.h platform.h system_gpu.h
cstdlib/math.o: cstdlib/math.c interpreter.h platform.h
