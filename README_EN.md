This is an automatic translation, may be incorrect in some places. See sources and examples!

# Gyvermotor
Library for convenient management of collector engines through the driver
- speed control and direction of rotation
- Work with ShIM of any resolution
- smooth start and speed change
- Active brake
- threshold of minimum shim
- Deadtime
- Support 5 types of drivers

> The library "consists" of two libraries: Gyvermotor and Gyvermotor2.The second version is more new, easier, better optimized and easier to use!

## compatibility
Compatible with all arduino platforms (used arduino functions)
- For ESP8266 (to SDK v2.7.4), do not forget to switch the control of 10 bits!

### Documentation
There is [expanded documentation] to the library (https://alexgyver.ru/gyvermotor/)

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** gyvermotor ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/gyvermotor/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
Types of drivers:
- ** driver2wire ** - two -wire driver with inversion shim (Dir + PWM)
- ** driver2wire_no_invert ** - two -wire driver without inversion shim (DIR + PWM)
- ** driver2wire_pwm ** - two -wire driver with two shim (PWM + PWM)
- ** driver3wire ** - three -wire driver (Dir + Dir + PWM)
- ** Relay2wire ** - Relay as a driver (Dir + Dir)

For two -wire bridge drivers, it is worth giving preference to the type ** driver2wire_pwm **.He requires two Shim Pin,
But the driver works more correctly and less loaded, and the speed will be the same in both directions.Unlike simple ** driver2wire **.
    
<details>
<summary> gyvermotor </summary>

`` `CPP
// Prevention options depending on the type of driver:
Gmotor Motor (Driver2wire, Dig_pin, Pwm_pin, (Low / High));
Gmotor Motor (Driver2Wire_No_invert, Dig_pin, Pwm_pin, (Low / High));
Gmotor Motor (Driver3wire, Dig_pin_a, Dig_pin_b, Pwm_pin, (Low/High));
Gmotor Motor (Relay2wire,dig_pin_a, dig_pin_b, (low/high));
/*
  dig_pin, dig_pin_a, dig_pin_b - any digital PIN MK
  Pwm_pin - any shim pin mk
  Low / High - Driver level.If, with an increase in speed, the motor, on the contrary, slows down, change the level
*/
`` `
</details>

<details>
<summary> gyvermotor2 </summary>

`` `CPP
Gmotor2 <Type> Motor (PIN1, PIN2, PIN3);// discharge shim 8 bits (0 .. 255)
Gmotor2 <Type, discharge> Motor (PIN1, PIN2, PIN3);// General case, discharge of shim in bits

// Types and quantity of pins depending on the driver
Gmotor2 <driver2wire> Motor (GPIO, PWM);
Gmotor2 <driver2wire_no_invert> motor (GPIO, PWM);
Gmotor2 <driver2wire_pwm> Motor (PWM, PWM);
Gmotor2 <driver3wire> Motor (GPIO, GPIO, PWM);
GMOTOR2 <RELAY2Wire> MOTOR (GPIO, GPIO);
`` `
</details>

<a id="usage"> </a>
## Usage
<details>
<summary> gyvermotor </summary>

`` `CPP
Gmotor (GM_DRIVERTYPE TYPE, Int8_T PARAM1 = _GM_NC, Int8_T PARAM2 = _GM_NC, Int8_T PARAM3 = _GM_NC, Int8_T PARAM4 = _GM_NC);
// Three options for creating an object depending on the driver:
// Gmotor Motor (Driver2wire, Dig_pin, Pwm_pin, (Low/High))
// Gmotor Motor (Driver3wire, Dig_pin_a, Dig_pin_b, Pwm_pin, (Low/High))
// Gmotor Motor (Relay2wire, Dig_pin_a, Dig_pin_b, (Low/High))

// speed installation -255..255 (8 bits) and -1023..1023 (10 bits)
VOID SetSpeed (Int16_T Duty);

// Change the mode of operation of the motor:
// Forward - forward
// backward - back
// Stop - Stop
// Brake - active brake
// auto - subordinates setspeed (-255 .. 255)
VOID setmode (gm_workmode mode);

// Direction of rotation
// norm - ordinary
// Reverse - reverse
VOID Setdirection (Bool Direction);

// Install minimal well (at which the motor begins to spin)
VOID setminduty (intlety);

// Set the PWM resolution in bits
VOID Setresolution (Byte Bit);

// Install Deadtime (in microseconds).By the silence 0
VOID Setdeadtime (Uint16_t Deadtime);

// set the driver's level (by the silence high)
VOID Setlevel (Int8_T LEVEL);

// smooth change to the indicated speed (to the value of the PWM)
VOID smoothtick (int16_t duty);

// speed change speed
VOID setsmoothSpeed (Uint8_t Speed);

// Returns -1 during rotation of Backward, 1 at Forward and 0 when stopping and braking
int Getstate ();

// internal variable well for debugging
int16_t _duty = 0;

// Curvature with old versions
// Install an output of 8 bits
VOID Set8BITMODE ();

// Set a 10 bite output
VOID set10BITMODE ();
`` `

### Logic of Work
In Setminduty () you can set the minimum speed (0..255), in which the motor begins rotation.
Further speed settings will be automatically scaled, taking into account the minimum.
Setdirection () sets the global direction of the motor, which automatically affects all speed functions.

### Original mode
Setmode (Forward) for moving forward, setmode (backward) is launched.
The speed is set in SetSpeed () or Run (Forward/Backward, speed).You can stop setmode (Stop).

#### Auto mode
Setmode (auto) is launched, the speed is set in SetSpeed (), negative values for rotation are maintained in the other direction.
You can stop setmode (Stop).

#### Smooth mode
For starting, you need to install setmode (Auto).In smooth mode, you need to call Smoothtick more often with the target speed.With a value of 0, the motor itself will stop smoothly.
For a sharp stop, you can use setmode (Stop).

</details>

<details>
<summary> gyvermotor2 </summary>

`` `CPP
VOID setminduty (uint16_t mduty);// Install the minimum PWM (silence 0)
VOID setmindutyperc (Uint16_T MDUTY);// Install the minimum PWM in % (silence 0)
VOID Setdeadtime (Uint16_T US);// Install Deadtime in microseconds (silence 0)
VOID Reverse (Bool R);// Reverses of the direction (silence. FALSE)

VOID Stop ();// Stop.If a smooth mode is turned on, then smooth
VOID Brake ();// Active brake
VOID SetSpeed (Int16_T S);// set speed (-max ..Max)
VOID SetSpeDPERC (Int16_T S);// set the speed in percent (-100 .. 100%)

int8_t gettate ();// Get status: the motor is spinning (1 and -1), the motor is stopped (0)
Int16_T GetSpeed ();// get the current motor speed

VOID SMOOTHMODE (Bool Mode);// Set a smooth speed change regime (silence. FALSE)
VOID Tick ();// smooth change to the specified speed, call in the cycle
VOID setsmoothSpeed (uint8_t s);// set the speed of speed change (silence 20)
VOID setsmoothSpeedPERC (UINT8_T S);// set the speed of speed change as a percentage
`` `

### Disgusting Shim
The default Avr Arduino uses 8 bit shim (0 .. 255).The ESP8266 uses 10-bit (0 .. 1023).
When initializing the library, you can configure the desired bit, it can be any.

### Speed
The speed is set in `setspeded (-max shim .. max shim)` in the size of the PWM signal, or in `setSpeedPERC (-100 .. 100)` as a percentage.The speed can be negative
Then the motor will spin in the opposite direction.With a value of 0, the motor will stop and the driver will be disconnected.

### operating modes
Calling `Stop ()` equivalent `setspeed (0)`.With direct control, the motor will be immediately stopped, with smooth - it will stop smoothly.The driver will turn off, the motor shaft will be released.
The call `Brake ()` will stop the motor and switch the driver to the active braking mode (the motor will be closed through itself).The shaft of the motor will resist rotation.
Call `Reverse (True)` inverts the direction of rotation of the motor for all functions.

## Minimum shim
In `setminduty (-max shim .. max shim)` you can set the minimum speed at which the motor starts rotation, this is convenient in most applications.
The speed installed in `setSpeed ()` speed will be automatically scale taking into account the minimum.
You can also set the minimum percentage `setmindutyperc (-100 .. 100)`.

### smooth mode
In smooth mode, the speed set in `setSpeed ()` The speed is not used immediately, but smoothly over time.For inclusion
The smooth mode must be called `smoothmode (True)` and place in the main cycle of the program, the function-tick `tick ()`.
Inside this function, the skill will smoothly change along the built -in timer (period - 50ms).
You can configure the speed of speed change - `setsmoothSpeed ()` in the values of Shim and `setsmoothspeedperc ()` percent.
</details>

<a id="EXAMPLE"> </a>
## Example
The rest of the examples look at ** Examples **!
`` `CPP
#include <gyvermotor2.h>
Gmotor2 <driver2wire> Motor (5, 6);
// gmotor2 <driver2wire_no_invert> motor (5, 6);
// gmotor2 <driver2wire_pwm> motor (5, 6);
// Gmotor2 <driver3wire> Motor (5, 6, 11);
// Gmotor2 <Relay2Wire> Motor (5, 6);

VOID setup () {
  Motor.Setminduty (70);// min.Shim
  //motor.reverse(1);// reverse
  //motor.Setdeadtime(5);// Deadtime
}

VOID loop () {
  Motor.SetSpeed (10);
  DELAY (1000);
  Motor.SetSpeed (-100);
  DELAY (1000);
  Motor.SetSpeed (50);
  DELAY (1000);
  Motor.SetSpeed (255);
  DELAY (1000);
  Motor.brake ();
  Delay (3000);
}
`` `

<a id="versions"> </a>
## versions
- V1.1 - Defaine removed
- v1.2 - Defaines returned
- V2.0:
    - software Deadtime
    - negative speeds
    - Support for two types of drivers and relay
    - smooth start and speed change
- v2.1: small fixes and adds
- V2.2: Optimization
- V2.3: Added ESP support (errors corrected)
- V2.4: Compatibility with other biblors
- V2.5: Added type Driver2Wire_NO_invert
- v3.0: Minduty logic is converted, a mode for PWM of any bitterness has been added
- V3.1: Small corrections
- V3.2: Improved smooth stability
- v3.2.1: Returned Run () to Public
- V4.0: Fixed a bug in Gyvermotor.Added gyvermotor2

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting bugs or incorrectoh work of the library must be indicated:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code