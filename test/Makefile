WFLAGS = -W -Wall -Wextra
IFLAGS = -iquote $(shell pwd)/../include
OPTFLAGS = # -march=native -ffast-math -O3 -funroll-loops # -msse
export CFLAGS = $(IFLAGS) $(WFLAGS) $(OPTFLAGS) -g
export CXXFLAGS = $(CFLAGS) -std=c++11
export LDFLAGS = -lm
BNN_LIB = ../libboolnn.a

boolnn_test.elf: networklib boolnn_test.cpp
	g++ $(CXXFLAGS) $(LDFLAGS) -o boolnn_test.elf boolnn_test.cpp $(BNN_LIB)

networklib:
	$(MAKE) -C ../ libboolnn.a

clean:
	-rm boolnn_test.elf

