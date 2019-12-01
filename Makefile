TEST=test_dynamic_cast_adaptor

CXX ?= g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++11
CPPFLAGS= -D_GLIBCXX_DEBUG
LDFLAGS=

DEPS = cppunit
CXXFLAGS += $(shell pkg-config --cflags $(DEPS))
LIBS = -Wl,--as-needed $(shell pkg-config --libs $(DEPS))

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(TEST)

$(OBJ): $(SRC) dynamic-cast-adaptor.h

$(TEST): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -rf  $(TEST) $(OBJ)

run: $(TEST)
	./$(TEST)

.PHONY: all clean


