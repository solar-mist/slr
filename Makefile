CXXC=g++
LD=g++
CXXFLAGS=-g -Iinclude/slr -std=c++17
LDFLAGS=

CXXSOURCES:=$(shell find src -name '*.cxx')
OBJS:=${CXXSOURCES:.cxx=.o}
TARGET=slr

all: $(TARGET)

%.o: %.cxx
	$(CXXC) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)