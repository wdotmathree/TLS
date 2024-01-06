CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -g -lpthread -lgmp
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

debug: a.out

release: CXXFLAGS += -O3
release: a.out
	# strip -a a.out

a.out: $(OBJS)
	$(CXX) $(OBJS) ec.o -o a.out $(CXXFLAGS)

clean:
	rm -f $(OBJS) a.out
