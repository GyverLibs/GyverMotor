This is an automatic translation, may be incorrect in some places. See sources and examples!

# GyverMotor
Library for convenient control of collector motors through the driver
- Control of speed and direction of rotation
- Work with PWM of any resolution
- Soft start and speed change
- Active brake
- Minimum PWM threshold
- dead time
- Support 5 types of drivers

> The library "consists" of two libraries: GyverMotor and GyverMotor2. The second version is newer, lighter, better optimized and easier to use!

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)
- For esp8266 (before SDK v2.7.4) don't forget to switch the control to 10 bits!

### Documentation
The library has [extended documentation](https://alexgyver.ru/GyverMotor/)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **GyverMotor** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download Library](https://github.com/GyverLibs/GyverMotor/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
Driver types:
- **DRIVER2WIRE** - two-wire driver with inverted PWM (dir + PWM)
- **DRIVER2WIRE_NO_INVERT** - two-wire driver without PWM inversion (dir + PWM)
- **DRIVER2WIRE_PWM** - two-wire driver with two PWM (PWM + PWM)
- **DRIVER3WIRE** - three-wire driver (dir + dir + PWM)
- **RELAY2WIRE** - relay as driver (dir + dir)

For 2-wire bridge drivers, the **DRIVER2WIRE_PWM** type should be preferred. It requires two PWM pins,
but the driver works more correctly and is less loaded, and the speed will be the same in both directions. Unlike a simple **DRIVER2WIRE**.
    
<details>
<summary>GyverMotor</summary>

```cpp
// initialization options depending on the driver type:
GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW / HIGH) );
GMotor motor(DRIVER2WIRE_NO_INVERT, dig_pin, PWM_pin, (LOW / HIGH) );
GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) );
GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) );
/*
  dig_pin, dig_pin_A, dig_pin_B - any MK digital pin
  PWM_pin - any PWM pin of the MK
  LOW / HIGH - driver level. If the motor slows down when the speed increases, change the level.
*/
```
</details>

<details>
<summary>GyverMotor2</summary>

```cpp
GMotor2<type> motor(pin1, pin2, pin3); // PWM width 8 bits (0.. 255)
GMotor2<type, bit> motor(pin1, pin2, pin3); // general case, PWM width in bits

// types and number of pins depending on the driver
GMotor2<DRIVER2WIRE> motor(GPIO, PWM);
GMotor2<DRIVER2WIRE_NO_INVERT> motor(GPIO, PWM);
GMotor2<DRIVER2WIRE_PWM> motor(PWM, PWM);
GMotor2<DRIVER3WIRE> motor(GPIO, GPIO, PWM);
GMotor2<RELAY2WIRE> motor(GPIO, GPIO);
```
</details>

<a id="usage"></a>
## Usage
<details>
<summary>GyverMotor</summary>

```cpp
GMotor(GM_driverType type, int8_t param1 = _GM_NC, int8_t param2 = _GM_NC, int8_t param3 = _GM_NC, int8_t param4 = _GM_NC);
// three options for creating an object depending on the driver:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )

// set speed -255..255 (8 bits) and -1023..1023 (10 bits)
void setSpeed(int16_t duty);

// change the motor mode:
// FORWARD - forward
// BACKWARD - back
// STOP - stop
// BRAKE - active brake
// AUTO - obeys setSpeed ​​(-255.. 255)
void setMode(GM_workMode);

// rotation direction
// NORM - normal
// REVERSE - reverse
void setDirection(bool direction);

// set the minimum duty cycle (at which the motor starts to spin)
void setMinDuty(int duty);

// set PWM resolution in bits
void setResolution(byte bit);

// set deadtime (in microseconds). Default 0
void setDeadtime(uint16_t deadtime);

// set driver level (default HIGH)
void setLevel(int8_t level);

// smooth change to the specified speed (to the PWM value)
void smoothTick(int16_t duty);

// rate of change of speed
void setSmoothSpeed(uint8_t speed);

// returns -1 on BACKWARD rotation, 1 on FORWARD and 0 on stop and brake
int getState();

// internal duty cycle variable for debugging
int16_t_duty = 0;

// compatibility with old versions
// set output to 8 bits
void set8bitMode();

// set output to 10 bits
void set10bitMode();
```

### Operation logic
In setMinDuty() you can set the minimum speed (0..255) at which the motor starts rotating.
Further speed settings will be automatically scaled based on the minimum speed.
setDirection() sets globallyomitted motor direction, which automatically affects all speed functions.

#### Normal mode
Run setMode(FORWARD) to move forward, setMode(BACKWARD) to move backward.
The speed is set in setSpeed() or run(FORWARD/BACKWARD, speed). You can stop by setMode(STOP).

#### Auto mode
SetMode(AUTO) is run, speed is set in setSpeed(), negative values ​​are supported for rotation in the opposite direction.
You can stop by setMode(STOP).

#### Smooth Mode
To run, you need to set setMode(AUTO). In smooth mode, you need to call smoothTick more often with an indication of the target speed. With a value of 0, the motor will smoothly stop itself.
For an abrupt stop, you can use setMode(STOP).

</details>

<details>
<summary>GyverMotor2</summary>

```cpp
void setMinDuty(uint16_tmduty); // set minimum PWM (default 0)
void setMinDutyPerc(uint16_t mduty); // set minimum PWM in % (default 0)
void setDeadtime(uint16_tus); // set deadtime in microseconds (default 0)
void reverse(bool r); // reverse direction (default false)

void stop(); // stop. If smooth mode is enabled, then smooth
void brake(); // active brake
void setSpeed(int16_t s); // set speed (-max.. max)
void setSpeedPerc(int16_t s); // set the speed as a percentage (-100.. 100%)

int8_t getState(); // get status: motor spinning (1 and -1), motor stopped (0)
int16_t getSpeed(); // get current motor speed

void smoothMode(bool mode); // set the smooth speed change mode (default false)
void tick(); // smooth change to the specified speed, call in a loop
void setSmoothSpeed(uint8_t s); // set rate of change of rate (default 20)
void setSmoothSpeedPerc(uint8_t s); // set rate of change in percentage
```Cranberry
### PWM bits
The AVR Arduino defaults to 8-bit PWM (0..255). The esp8266 uses 10 bits (0..1023).
When initializing the library, you can set the desired bit depth, it can be any.

### Speed
The speed is set in `setSpeed(-max PWM.. max PWM)` as a PWM value, or in `setSpeedPerc(-100.. 100)` as a percentage. Speed ​​can be negative
then the motor will spin in the opposite direction. With a value of 0, the motor will stop and the driver will be disabled.

### Operating modes
Calling `stop()` is equivalent to `setSpeed(0)`. With direct control, the motor will stop immediately, with smooth control, it will stop smoothly. The driver will turn off, the motor shaft will be released.
Calling `brake()` will stop the motor and switch the driver to active braking mode (closes the motor through itself). The motor shaft will resist rotation.
Calling `reverse(true)` reverses the direction of rotation of the motor for all functions.

### Minimum PWM
In `setMinDuty(-max PWM.. max PWM)` you can set the minimum speed at which the motor starts to rotate, this is useful in most applications.
The speed set in `setSpeed()` will be automatically scaled to the minimum speed.
You can also set the minimum speed as a percentage `setMinDutyPerc(-100.. 100)`.

### Smooth mode
In smooth mode, the speed set in `setSpeed()` is not applied immediately, but smoothly over time. To enable
smooth mode, you need to call `smoothMode(true)` and place the ticker function `tick()` in the main program loop.
Inside this function, the speed will smoothly change according to the built-in timer (period - 50ms).
You can set the rate of change of speed - `setSmoothSpeed()` in PWM values ​​and `setSmoothSpeedPerc()` in percent.
</details>

<a id="example"></a>
## Example
See **examples** for other examples!
```cpp
#include <GyverMotor2.h>
GMotor2<DRIVER2WIRE> motor(5, 6);
//GMotor2<DRIVER2WIRE_NO_INVERT> motor(5, 6);
//GMotor2<DRIVER2WIRE_PWM> motor(5, 6);
//GMotor2<DRIVER3WIRE> motor(5, 6, 11);
//GMotor2<RELAY2WIRE> motor(5, 6);

void setup() {
  motor.setMinDuty(70); // min. PWM
  //motor.reverse(1); // reverse
  //motor.setDeadtime(5); // deadtime
}

void loop() {
  motor.setSpeed(10);
  delay(1000);
  motor setSpeed(-100);
  delay(1000);
  motor.setSpeed(50);
  delay(1000);
  motor.setSpeed(255);
  delay(1000);
  motor brake();
  delay(3000);
}
```

<a id="versions"></a>
## Versions
- v1.1 - defines removed
- v1.2 - defaults returned
- v2.0:
    - Software deadtime
    - Negative speeds
    - Support for two types of drivers and relays
    - Soft start and speed change
- v2.1: small fixes and additions
- v2.2: optimization
- v2.3: esp support added (bugs fixed)
- v2.4: compatibility with other bibles
- v2.5: added type DRIVER2WIRE_NO_INVERT
- v3.0: redesigned minDuty logic, added mode for PWM of any bitness
- v3.1: minor fixes
- v3.2: improved smooth mode stability
- v3.2.1: returned run() to public
- v4.0: fixed bug in GyverMotor. Added GyverMotor2

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!