WFLAGS = -W -Wall -Wextra
IFLAGS = -iquote $(shell pwd)/include
OPTFLAGS = -march=native -ffast-math -O3 -funroll-loops # -msse
export CFLAGS = $(IFLAGS) $(WFLAGS) $(OPTFLAGS) -g
export CXXFLAGS = $(CFLAGS) -std=c++11
export LDFLAGS = -lm
NETWORKLIB_F = network/bool_mat.o

libbnn.a: networklib

networklib:
	$(MAKE) -C network

clean:
	$(MAKE) -C network clean
