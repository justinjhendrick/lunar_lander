default:
	clang++ -g -Weverything *.cpp -std=c++11 -lSDL2 -o lunar_lander

clean:
	rm lunar_lander
