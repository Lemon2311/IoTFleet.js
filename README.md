# IoTFleet.js

## Overview
The IoTFleet.js library is a versatile tool that enables control over multiple microcontroller devices like the Esp32 or Raspberry Pi Pico from a PC remotely using JavaScript.

## Features
- Control multiple microcontroller devices from a PC using HTTP requests and JavaScript, but the functionality is abstracted so it works like magic almost...
- Supports ESP devices, and devices that run MicroPython
- Non-blocking IO. Both the javascript library, the Esp c++ code and MicroPython code work asynchronous.
- Easy integration with JS libraries like the ones from npm
- The Esp c++ async restfull API EspServer.cpp, and MicroPython async restfull API MicroPythonServer.py can be used with any language, so implementing this library in another language is easy
- There is also a ui located inside the IO.ui folder that ca be used to control digital/analog IO of multiple devices which is really usefull for testing
- Actively supports digital & analog IO

## Software Requirements
* Platform.io // to upload the main.cpp file to the Esp. <br> *Note*: Platform.io automatically downloads the EspAsyncWebServer library, when using something else to upload the server code to the Esp the library needs to be installed.
* Thonny IDE / or any suitable way of running and adding files to the MicroPython microcontroller

# How to use
First, download the project by pressing `<>Code` followed by `Download Zip`.

![Download Zip](https://github.com/Lemon2311/IoTFleet.js/assets/63803133/b0d0e1da-d7d6-4f7c-a26b-e707ec1ee5ee)

Extract the file (using a tool like WinRAR) and then:

To use the library a server needs to be run on the microcontroller device which can either be the EspServer.cpp or MicroPythonServer.py

## EspServer.cpp
*Suitable for Esp devices*

Create a `WiFiCredentials.h` file inside `Esp32Server.cpp/src` containing your wifi credentials like so:

```cpp
// WiFiCredentials.h
const char *ssid = "YOUR-WIFI-SSID";
const char *password = "YOUR-WIFI-PASSWORD";
```

Upload the `main.cpp` file located inside `Esp32Server.cpp/src` to the ESP32 using Platform.io and view the serial monitor. After the code has uploaded and the ESP32 connected to the WiFi, the IP address of the ESP32 will appear in the serial monitor. Copy it as you'll need it in a future step.

* Note: To make sure Platform.io recognises the project, make sure to open `Esp32Server.cpp` (the ESP32 project) in a separate window.

## MicroPythonServer.py
*Suitable for any device running MicroPython*

Create a WIFI_CREDENTIALS.py file inside MicroPythonServer.py containing wifi credentials like so
```python
# WIFI_CREDENTIALS.py
SSID = "SSID"
PASS = "PASS"
```
Save `WIFI_CREDENTIALS.py`, `MicroAPIgRESTion.py` to the MicroPython microcontroller device using Thonny IDE or any apropriate IDE.

Save `MicroPythonServer.py` as `main.py` on the MicroPython microcontroller device using Thonny IDE or any apropriate IDE.

Whatever `main.py` contains will be run at startup so now whenever the device powers up it will run the MicroPython server.
## Then
Then using the library is easy. Copy the IoTFleet.js folder which contains the library and an example to your project and import the output and/or input function from `script.js` and use it like so:

```js
//this example is taken from example.js

import { output, input } from "./IoTFleet.js"; //make sure to update your path as example.js is located
                                            //inside the same folder as script.js

const { d13, a27 } = output("192.168.1.138", "d13", "a27");//declare and initialize output pins

d13.set("high");//set digital pin 13 to HIGH
a27.set(2);//set analog pin 27 to 2V

const { d3, a34 } = input("192.168.1.138", "d3", "a34");//declare and initialize input pins

await d3.get();//return digital pin 3 value
await a34.get();//return voltage of analog pin 34

```

* Note: This example sets pin d13 to high, a27 to 2V, and gets the values from d3 and a34 (a=analog,d=digital). Analog voltage has a margin of arround 0.1V sometimes even 0.15V, as the function of bit value driven analog voltage pins isn`t liniar.

# Feature requests and contributions
I accept feature requests and contributions, so if you want a functionality that is not implemented already you can open an issue and if you want to contribute there are issues that need help and I`d be glad to guide you if you have any questions regarding them. Even more, is there a feature you want to add to this library and know how to implement it, open an issue about it and we can discuss details, then you can implement it and make a pull request.

# Links:
Thank you me-no-dev for the EspAsyncWebServer library that was used to make the esp c++ async rest api: https://github.com/me-no-dev/ESPAsyncWebServer <br>
Thanks to myself for the MicroAPIgRESTion library that was used to make the MicroPython async rest api: https://github.com/Lemon2311/MicroAPIgRESTion
