
OUTPUT=bin/tower

CXXFLAGS=-Wall -O0 -g -MMD
LDLIBS=-lSDL2 -lm -lSDL2_ttf
SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(OUTPUT)

-include $(OBJS:.o=.d)

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

$(OUTPUT): $(OBJS)
	g++ $^ $(LDLIBS) -o $@

clean:
	rm -rf $(OBJS) $(OBJS:.o=.d) $(OUTPUT)

re : clean all
