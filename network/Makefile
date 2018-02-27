
all: boolmat.o layers.o network.o

boolmat.o: boolmat.cpp
	g++ $(CXXFLAGS) -c -o boolmat.o boolmat.cpp

layers.o: layers.cpp
	g++ $(CXXFLAGS) -c -o layers.o layers.cpp

network.o: network.cpp
	g++ $(CXXFLAGS) -c -o network.o network.cpp

clean:
	-rm boolmat.o
	-rm layers.o
	-rm network.o
