PWD := $(shell pwd)
SRC := $(PWD)/src
OUT := $(PWD)/out

CXX := g++
CC  := gcc
CXXFLAGS := 
CFLAGS   :=
CXXSRCS := $(wildcard $(SRC)/*.cpp)
CSRCS   := $(wildcard $(SRC)/*.c)

EXE := $(PWD)/porttorpedo

OBJS := $(CXXSRCS:$(SRC)/%.cpp=$(OUT)/%.cpp.o) $(CSRCS:$(SRC)/%.c=$(OUT)/%.c.o)

all: $(OUT) $(EXE)

clean:
	rm -rfv $(OUT) $(EXE)

$(OUT): 
	echo $(OBJS)
	mkdir -p $@

$(EXE): $(OBJS)
	$(CXX) -o $@ $^

$(OUT)/%.cpp.o: $(SRC)/%.cpp
	$(CXX) -c -o $@ $^

$(OUT)/%.c.o: $(SRC)/%.c
	$(CC) -c -o $@ $^

.PHONY: all clean
