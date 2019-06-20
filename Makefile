src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

solver: $(obj)
	$(CXX) -std=c++11 -lm $(DEBUG) -o $@ $^

solver.o: solver.cpp
	$(CXX) -std=c++11 -lm $(DEBUG) -c -o solver.o solver.cpp

main.o: main.cpp
	$(CXX) -std=c++11 -lm $(DEBUG) -c -o main.o main.cpp
	
.PHONY: clean
clean:
	rm -f $(obj) solver

debug: DEBUG = -g
debug: solver
