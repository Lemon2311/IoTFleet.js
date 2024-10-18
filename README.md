## EspC++
*Suitable for C++ Esp devices*, works with IoTFleet.js<br>
*note: This is a platform.io project, and platform.io automatically installs the ESPAsyncWebServer*

Insert your wifi credentials in the `WiFiCredentials.h` file inside `src`

```cpp
// WiFiCredentials.h
const char *ssid = "YOUR-WIFI-SSID";
const char *password = "YOUR-WIFI-PASSWORD";
```

Upload the `main.cpp` file located inside `src` to the ESP using Platform.io and view the serial monitor. After the code has uploaded and the ESP connected to WiFi, the IP address of the ESP will appear in the serial monitor.

*Thank you me-no-dev for the EspAsyncWebServer library that was used to make the esp c++ async rest api: https://github.com/me-no-dev/ESPAsyncWebServer* <br>