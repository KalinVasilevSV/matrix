# Matrix Makefile
#

# Select appropriate compiler.
CPP=g++
CPPFLAGS=-O3

ifdef DEBUG
CPPFLAGS=-g
endif

default: all

all: matrix

matrix: matrix.cpp Makefile
	$(CPP) $(CPPFLAGS) -o matrix matrix.cpp 

clean:
	-rm matrix

DISTNAME = matrix
DISTFILES = matrix.cpp Makefile

SRCS = $(CFILES) $(C++FILES) $(C++FILES:%.cxx=%.h)

others:
	$(CPP) $(CPPFLAGS)
