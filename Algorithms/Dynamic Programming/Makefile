CXX = clang++
CXX_FLAGS = -std=c++14
CXX_FLAGS += -O2

all: task1

task1: task1.o
	@$(CXX) -o $@ $<

task1.o: task1.cpp
	@$(CXX) -c $< $(CXX_FLAGS)

clean:
	@rm -rf *.o
