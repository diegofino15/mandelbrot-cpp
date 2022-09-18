fractal: src/*.cpp
	g++ src/*.cpp -lraylib -std=c++11 -o fractal
run: fractal
	./fractal

