TARGETS = server client
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
OBJECTS_CLIENT = build/cmain.o build/Client.o
OBJECTS_SERVER = $(filter-out $(OBJECTS_CLIENT), $(OBJECTS:src/%.o=build/%.o))

CPPFLAGS = -g -std=c++20 -pthread
LDFLAGS = -lrt

all: prepare $(TARGETS)

prepare:
	mkdir -p build/bin

server: $(OBJECTS)
	g++ $(CPPFLAGS) $(OBJECTS_SERVER) -o build/bin/$@ $(LDFLAGS)

client: $(OBJECTS)
	g++ $(CPPFLAGS) $(OBJECTS_CLIENT) -o build/bin/$@

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $(@:src/%=build/%) $^

clean:
	rm -rf build