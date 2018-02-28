WFLAGS = -W -Wall -Wextra
IFLAGS = -iquote $(shell pwd)/include
OPTFLAGS = -march=native -ffast-math -O3 -funroll-loops # -msse
export CFLAGS = $(IFLAGS) $(WFLAGS) $(OPTFLAGS) -g
export CXXFLAGS = $(CFLAGS) -std=c++11
export LDFLAGS = -lm
NETWORKLIB_F = network/boolmat.o network/layers.o network/network.o

libboolnn.a: networklib
	ar rcs libboolnn.a $(NETWORKLIB_F)

networklib:
	$(MAKE) -C network

clean:
	$(MAKE) -C network clean
	-rm libboolnn.a
