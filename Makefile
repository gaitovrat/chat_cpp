TARGETS = server client
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
OBJECT_CLIENT = build/cmain.o build/Client.o
OBJECT_SERVER = build/smain.o build/Server.o
OBJECTS_CLIENT = $(filter-out $(OBJECT_SERVER), $(OBJECTS:src/%.o=build/%.o))
OBJECTS_SERVER = $(filter-out $(OBJECT_CLIENT), $(OBJECTS:src/%.o=build/%.o))

CPPFLAGS = -g -std=c++20 -pthread
LDFLAGS = -lrt

all: prepare $(TARGETS)

prepare:
	mkdir -p build/bin

server: $(OBJECTS)
	g++ $(CPPFLAGS) $(OBJECTS_SERVER) -o build/bin/$@ $(LDFLAGS)

client: $(OBJECTS)
	g++ $(CPPFLAGS) $(OBJECTS_CLIENT) -o build/bin/$@ $(LDFLAGS)

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $(@:src/%=build/%) $^

clean:
	rm -rf build