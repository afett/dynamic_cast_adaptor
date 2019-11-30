TARGET=test_dynamic_cast_adaptor

CXX ?= g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -std=c++11
CPPFLAGS=
LDFLAGS=

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(TARGET)

$(TARGET): $(OBJ) dynamic-cast-adaptor.h
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -rf  $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean


