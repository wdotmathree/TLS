CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -g -lpthread -lgmp
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS)
	$(CXX) $(OBJS) ec.o -o a.out $(CXXFLAGS)

clean:
	rm -f $(OBJS) a.out
