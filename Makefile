ALL_SRCS := $(wildcard src/*.cpp)
NON_MAIN_SRCS := $(filter-out src/main.cpp, $(ALL_SRCS))

default:
	clang++ -g -Weverything -Wno-c++98-compat -Wno-padded src/*.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

gcc:
	g++ src/*.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

test:
	clang++ -g tests/*.cpp $(NON_MAIN_SRCS) -std=c++11 -lSDL2 -lSDL2_ttf -o test.exe

nodebug:
	clang++ -O3 src/*.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

nowarn:
	clang++ -g src/*.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

windows:
	clang++ -target x86_64-intel-win32-gnu src/*.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

clean:
	rm lunar_lander
	rm test.exe
