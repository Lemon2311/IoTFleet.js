# IoTFleet.js

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Overview
The IoTFleet.js library is a versatile tool that enables you to control multiple ESP32 devices from a PC remotely using JavaScript. This library utilizes the EspAsyncWebServer library for seamless communication between ESP32 devices and a PC onto which the JS library is used.

## Features
- Control multiple ESP32 devices from a PC using HTTP requests and JavaScript, but the functionality is abstracted so it works like magic almost...
- Non-blocking IO. Both the javascript library and the c++ code on the esp32 work asynchronous.
- Easy integration with JS libraries like the ones from npm
- The esp32 c++ async restfull API can be used with any language, and actively supports digital/analog IO
- There is also a ui located inside the IO.ui folder that ca be used to control digital/analog IO
- The js library itself activelly supports digital & analog IO

## Software Requirements
* Platform.io // to upload the main.cpp file to the esp32. * Note: Platform.io automatically downloads the EspAsyncWebServer library, when using something else to upload the server code to the esp32 the library needs to be installed.

## How to use
First, download the project by pressing `<>Code` followed by `Download Zip`.

![Download Zip](https://github.com/Lemon2311/IoTFleet.js/assets/63803133/b0d0e1da-d7d6-4f7c-a26b-e707ec1ee5ee)

Extract the file (using a tool like WinRAR) and then:

Create a `WiFiCredentials.h` file inside `Esp32Server.cpp/src` containing your wifi credentials like so:

```cpp
// WiFiCredentials.h
const char *ssid = "YOUR-WIFI-SSID";
const char *password = "YOUR-WIFI-PASSWORD";
```

Upload the `main.cpp` file located inside `Esp32Server.cpp/src` to the ESP32 using Platform.io and view the serial monitor. After the code has uploaded and the ESP32 connected to the WiFi, the IP address of the ESP32 will appear in the serial monitor. Copy it as you'll need it in a future step.

* Note: To make sure Platform.io recognises the project, make sure to open `Esp32Server.cpp` (the ESP32 project) in a separate window.

Then using the library is easy. Copy the lib.js folder which contains the library and an example to your project and import the output and/or input function from `script.js` and use it like so:

```js
//this example is taken from example.js

import { output, input } from "./index.js"; //make sure to update your path as example.js is located
                                            //inside the same folder as script.js

const { d13, a27 } = output("192.168.1.138", "d13", "a27");//declare and initialize output pins

d13.set("high");//set digital pin 13 to HIGH
a27.set(2);//set analog pin 27 to 2V

const { d3, a34 } = input("192.168.1.138", "d3", "a34");//declare and initialize input pins

d3.get();//return digital pin 3 value
a34.get();//return voltage of analog pin 34

```

* Note: This example sets pin d13 to high, a27 to 2V, and gets the values from d3 and a34 (a=analog,d=digital). Analog voltage has a margin of arround 0.1V sometimes even 0.15V, as the function of bit value driven analog voltage pins isn`t lin#iar.

# Links:
Thank you me-no-dev for the EspAsyncWebServer library : https://github.com/me-no-dev/ESPAsyncWebServer <br>
My personal website : https://lemon2311.github.io/Portofolio/
