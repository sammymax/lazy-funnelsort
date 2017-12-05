EXEC = test.x test_parameter.x
CXX := g++
CXXFLAGS := -std=c++11
LINK = ${CXX} $^ -o $@

ifeq ($(DEBUG),1)
	CXXFLAGS += -g -DNDEBUG
else
	CXXFLAGS += -O3 -march=native -g
endif

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(EXEC)

%.x: %.o
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) -MMD -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

-include *.d
