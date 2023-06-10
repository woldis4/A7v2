C_FLAGS=-std=c++11
SRCS=$(wildcard src/*.cpp)
HEADERS=$(wildcard src/*.hpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: A7

A7: $(OBJS)
	g++ $(OBJS) -o futballFantasy.out

%.o: %.cpp $(HEADERS)
	g++ $(C_FLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS)