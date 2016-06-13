ALL_SRCS := $(wildcard src/*.cpp)
NON_MAIN_SRCS := $(filter-out src/main.cpp, $(ALL_SRCS))
LIBS := -lSDL2 -lSDL2_ttf
TOPLEVEL_RELEASE_FILES := lunar_lander COPYING COPYING_FONTS README.md UbuntuMono-R.ttf

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
	clang++ -v -target x86_64-intel-win32-msvc src/*.cpp -std=c++11 $(LIBS) -o lunar_lander

linux64:
	clang++ -O3 src/*.cpp -std=c++11 $(LIBS) -o lunar_lander
	mkdir lunar_lander_linux64
	mkdir lunar_lander_linux64/imgs
	mkdir lunar_lander_linux64/sprites
	cp sprites/* lunar_lander_linux64/sprites
	cp imgs/logo.bmp lunar_lander_linux64/imgs
	cp $(TOPLEVEL_RELEASE_FILES) lunar_lander_linux64/
	zip -r lunar_lander_linux64.zip lunar_lander_linux64/
	rm -rf lunar_lander_linux64/

linux32:
	clang++ -O3 src/*.cpp -std=c++11 $(LIBS) -o lunar_lander -triple x86-linux-gnu
	mkdir lunar_lander_linux32
	mkdir lunar_lander_linux32/imgs
	mkdir lunar_lander_linux32/sprites
	cp sprites/* lunar_lander_linux32/sprites
	cp imgs/logo.bmp lunar_lander_linux32/imgs
	cp $(TOPLEVEL_RELEASE_FILES) lunar_lander_linux32/
	zip -r lunar_lander_linux32.zip lunar_lander_linux32/
	rm -rf lunar_lander_linux32/

clean:
	rm -f lunar_lander
	rm -f test.exe
	rm -f lunar_lander_linux64.zip
	rm -f lunar_lander_linux32.zip
