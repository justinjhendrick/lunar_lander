default:
	clang++ -g -Weverything -Wno-c++98-compat *.cpp -std=c++11 -lSDL2 -o lunar_lander

clean:
	rm lunar_lander
