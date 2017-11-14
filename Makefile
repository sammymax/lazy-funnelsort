EXEC = test
CPP := g++
CPPFLAGS := -std=c++11

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS)
	$(CPP) $(OBJS) -o $(EXEC)

%.o: $.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@
