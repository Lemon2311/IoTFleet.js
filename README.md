# IoTFleet.js
Javascript version of IoTFleet.

IoTFleet enables control over multiple microcontroller devices running micropython like the Esp32 or Raspberry Pi Pico from a PC remotely.

## Features
- Control multiple microcontroller devices from a PC using HTTP requests and JavaScript, but the functionality is abstracted so it works like magic almost...
- Supports devices that run MicroPython, and ESP c++ devices
- Non-blocking IO. Both the javascript library, and the microcontroller code work asynchronous.
- Easy integration with JS libraries like the ones from npm
- Actively supports digital & analog IO

## Slave Device Setup
To get started, insert your wifi credential into the file `config.py` then save `MicroAPIgRESTion.py`, `main.py` & `config.py` onto the slave device.
```
config.py
```
```python
SSID = "SSID" # Replace SSID with your wifi SSID
PASS = "PASS" # Replace PASS with PASSWORD
```
The ip of the device is being output in serial when the device connects to WI-Fi.

## Then
Then using the library is easy. Copy the IoTFleet.js file which contains the library to your project and import the output and/or input function from `script.js` and use it like so:

```js
//this example is taken from example.js

import { output, input } from "./IoTFleet.js"; //make sure to update your path as example.js is located
                                            //inside the same folder as IoTFleet.js

const { d13, a27 } = output("192.168.1.138", "d13", "a27");//declare and initialize output pins

d13.set("high");//set digital pin 13 to HIGH
a27.set(2);//set analog pin 27 to 2V

const { d3, a34 } = input("192.168.1.138", "d3", "a34");//declare and initialize input pins

await d3.get();//return digital pin 3 value
await a34.get();//return voltage of analog pin 34

```

*Note: Analog voltage has a margin of arround 0.1V sometimes even 0.15V, as the function of bit value driven analog voltage pins isn`t liniar.*
