
all: boolmat.o layers.o

boolmat.o: boolmat.cpp
	g++ $(CXXFLAGS) -c -o boolmat.o boolmat.cpp

layers.o: layers.cpp
	g++ $(CXXFLAGS) -c -o layers.o layers.cpp

clean:
	-rm boolmat.o
	-rm layers.o
