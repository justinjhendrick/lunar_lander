ALL_SRCS := $(wildcard src/*.cpp)
NON_MAIN_SRCS := $(filter-out src/main.cpp, $(ALL_SRCS))
LIBS := -lSDL2 -lSDL2_ttf

default:
	clang++ -g -Weverything -Wno-c++98-compat -Wno-padded src/*.cpp -std=c++11 $(LIBS) -o lunar_lander

gcc:
	g++ src/*.cpp -std=c++11 $(LIBS) -o lunar_lander

test:
	clang++ -g tests/*.cpp $(NON_MAIN_SRCS) -std=c++11 $(LIBS) -o test.exe

prod:
	clang++ -O3 src/*.cpp -std=c++11 $(LIBS) -o lunar_lander

nowarn:
	clang++ -g src/*.cpp -std=c++11 $(LIBS) -o lunar_lander

windows:
	clang++ --gcc-toolchain=/usr/include -v -target x86_64-intel-win32-msvc src/*.cpp -std=c++11 $(LIBS) -o lunar_lander -Xlinker -static

clean:
	rm lunar_lander
	rm test.exe
