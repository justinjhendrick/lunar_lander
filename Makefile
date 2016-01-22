default:
	clang++ -g -Weverything -Wno-c++98-compat -Wno-padded *.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

nodebug:
	clang++ -O3 *.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

nowarn:
	clang++ -g *.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

windows:
	clang++ -target x86_64-intel-win32-gnu *.cpp -std=c++11 -lSDL2 -lSDL2_ttf -o lunar_lander

clean:
	rm lunar_lander
