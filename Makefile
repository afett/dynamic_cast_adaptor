TARGET=test_dynamic_cast_adaptor

CXX ?= g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++11
CPPFLAGS= -D_GLIBCXX_DEBUG
LDFLAGS=

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(TARGET)

$(OBJ): $(SRC) dynamic-cast-adaptor.h

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -rf  $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean


