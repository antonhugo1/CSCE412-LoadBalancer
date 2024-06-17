CXX = g++
CXXFLAGS += -std=c++17 -g

loadbalancer: loadbalancer.cpp loadbalancer.h main.cpp webserver.cpp webserver.h
	g++ *.cpp -o loadbalancer

clean:
	rm -f *.txt loadbalancer