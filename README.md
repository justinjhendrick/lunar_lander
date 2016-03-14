<p align="center">
  <img
  src="https://github.com/justinjhendrick/lunar_lander/blob/master/imgs/logo.bmp"
  />
</div>

Can you land on the moon in one piece?

## Dependencies
clang 3.3 or later (or a version of gcc that supports c++11), libsdl2-dev, libsdl2-ttf-dev

## How to run
```
$ make
$ ./lunar_lander
```

## How to play
```
left  or a     apply torque counterclockwise
right or d     apply torque clockwise
up    or w     increase thrust
down  or s     decrease thrust
Spacebar       turn on your thruster
```

## Computer Pilot
`$ ./lunar_lander -c` enables the computer controlled pilot.
You don't play, just watch the flight computer do its work.

![flight computer lands](https://github.com/justinjhendrick/lunar_lander/blob/master/imgs/landing.gif)

### Command line arguments
```
-c           enable computer pilot
-s <seed>    seed the random number generator
```

## Testing
```
$ make test
$ ./test.exe 
```

`test.exe` takes an optional argument,
the number of trials to run the auto-pilot. Default is 1.

## Attribution
All files except `UbuntuMono-R.ttf` are under Creative Commons Attribution (CC BY).
See `COPYING` for more details

`UbuntuMono-R.ttf` is under the Ubuntu Font Licence.
See `COPYING_FONTS` for more details
