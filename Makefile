EXEC = test
CPP := g++
CPPFLAGS := -std=c++11

ifeq ($(DEBUG),1)
	CPPFLAGS += -g -DNDEBUG
else
	CPPFLAGS += -O3 -march=native -g
endif

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS)
	$(CPP) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CPP) -MMD -c $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

-include *.d
