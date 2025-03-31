# Critter Connect V2

Modified for PlatformIO

## Board Definition

Use the following board definition parameters take from the [Arduino Pico doc](https://arduino-pico.readthedocs.io/en/latest/platformio.html) in `platformio.ini`

```ini
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
```

## Library Build Issues

Modify `.pio/libdeps/pico/DFRobot_DF1201S/src/DFRobot_DF1201S.cpp` as decribed in this [GitHub Issue](https://github.com/DFRobot/DFRobot_DF1201S/issues/11).

```c++
// Line 65 breaks compilation
// playMode = str[10]; 

// Replace with this
playMode = (const char*)str[10];
```
