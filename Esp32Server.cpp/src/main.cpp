#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "WiFiCredentials.h"

AsyncWebServer server(80);
// how to use digital/analog IO via http endpoints
// to set or initialize pins call a post request like bellow
// initialize the digital pin as an output example http://192.168.1.138/initializeDigitalPin?pin=13&mode=output //pin 13 is initialized as output
// set the digital pin to high example http://192.168.1.138/digitalOutput?pin=13&state=high //pin 13 is set to high
// set the analog pin output voltage example http://192.168.1.138/analogOutput?pin=25&value=2&type=voltage //pin 25 is set to 2V, type can either be value or voltage, max voltage = 3.287V, max value = 255
// to get pins call a get request like bellow
// get the digital pin input state example http://192.168.1.138/digitalInput?pin=5 //pin 5 is read as input
// get the analog pin input voltage http://192.168.1.138/analogInput?pin=34&type=voltage&precision=12 //pin 34 is read as voltage, precision is 12 bits, precision can be 10,11,12 bits, default is 12 bits

void initWifi()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void helloWorld()
{
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Hello");
    request->send(200, "text/plain", "Hello from ESP"); });
}

void addCORSHeaders(AsyncWebServerResponse *response)
{
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET, POST");
  response->addHeader("Access-Control-Allow-Headers", "*");
}

void setDigitalOutputHttpEndpoint()
{
  server.on("/digitalOutput", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int statusCode = 400;
    String responseContent;

    if (!request->hasParam("pin") || !request->hasParam("state"))
    {
      responseContent = "Missing 'pin' or 'state' parameter.";
    }
    else
    {
      String pinNumber = request->getParam("pin")->value();
      String state = request->getParam("state")->value();
      int pin = pinNumber.toInt();

      if (pin == 0 && pinNumber != "0") // Check if conversion failed and it wasn't zero
      {
        responseContent = "Invalid 'pin' parameter. Please use a valid integer.";
      }
      else
      {
        pinMode(pin, OUTPUT); // Ensure the pin mode is set to OUTPUT

        if (state == "high")
        {
          digitalWrite(pin, HIGH);
          statusCode = 200;
          responseContent = "Pin nr." + String(pin) + " set to HIGH";
        }
        else if (state == "low")
        {
          digitalWrite(pin, LOW);
          statusCode = 200;
          responseContent = "Pin nr." + String(pin) + " set to LOW";
        }
        else
        {
          responseContent = "Invalid 'state' parameter. Use 'high' or 'low'.";
        }
      }
    }

    // Create the response with the correct status code and body
    AsyncWebServerResponse *response = request->beginResponse(statusCode, "text/plain", responseContent);
    // Add CORS headers
    addCORSHeaders(response);
    // Send the response
    request->send(response); });
}

void setPinModeHttpEndpoint()
{
  // New endpoint for initializing digital pins using query parameters
  server.on("/initializeDigitalPin", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int statusCode = 400; // Default to bad request
    String responseContent;

    if (!request->hasParam("pin") || !request->hasParam("mode"))
    {
      responseContent = "Missing 'pin' or 'mode' parameter.";
    }
    else
    {
      String pinNumber = request->getParam("pin")->value();
      String mode = request->getParam("mode")->value();
      int pin = pinNumber.toInt();

      if (pin == 0 && pinNumber != "0") // Check if conversion failed and it wasn't zero
      {
        responseContent = "Invalid 'pin' parameter. Please use a valid integer.";
      }
      else
      {
        if (mode == "input")
        {
          pinMode(pin, INPUT);
          statusCode = 200;
          responseContent = "Digital pin nr." + String(pin) + " initialized as INPUT";
        }
        else if (mode == "output")
        {
          pinMode(pin, OUTPUT);
          statusCode = 200;
          responseContent = "Digital pin nr." + String(pin) + " initialized as OUTPUT";
        }
        else
        {
          responseContent = "Invalid 'mode' parameter. Use 'input' or 'output'.";
        }
      }
    }

    // Create the response with the correct status code and body
    AsyncWebServerResponse *response = request->beginResponse(statusCode, "text/plain", responseContent);
    // Add CORS headers to the response
    addCORSHeaders(response);
    // Send the response
    request->send(response); });
}

void getDigitalInputHttpEndpoint()
{
  server.on("/digitalInput", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    int statusCode = 200;
    String responseContent;

    if (!request->hasParam("pin")) {
      statusCode = 400;
      responseContent = "Missing 'pin' parameter.";
    } else {
      String pinNumber = request->getParam("pin")->value();

      if (!pinNumber.toInt()) {
        statusCode = 400;
        responseContent = "Invalid 'pin' parameter. Please use a valid integer.";
      } else {
        int pin = pinNumber.toInt();
        int pinState = digitalRead(pin);
        responseContent = pinState == HIGH ? "HIGH" : "LOW";
      }
    }

    // Create the response with the correct status code and body
    AsyncWebServerResponse *response = request->beginResponse(statusCode, "text/plain", responseContent);

    // Add CORS headers to the response
    addCORSHeaders(response);

    // Send the response
    request->send(response); });
}

void getAnalogInputHttpEndpoint()
{
  server.on("/analogInput", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        if (!request->hasParam("pin")) {
            AsyncWebServerResponse *response = request->beginResponse(400, "text/plain", "Missing 'pin' parameter.");
            addCORSHeaders(response);
            request->send(response);
            return;
        }

        String pinNumber = request->getParam("pin")->value();
        String type = request->hasParam("type") ? request->getParam("type")->value() : "value";
        String precisionParam = request->hasParam("precision") ? request->getParam("precision")->value() : "12";
        int pin = pinNumber.toInt();
        int precision = precisionParam.toInt();

        analogReadResolution(precision);
        int value = analogRead(pin);

        String responseContent;
        int statusCode = 200;

        if (type == "voltage") {
            float maxAdcValue = pow(2, precision) - 1;
            float voltage = (value / maxAdcValue) * 3.3;
            responseContent = String(voltage, precision) + "V";
        } else {
            responseContent = String(value);
        }

        AsyncWebServerResponse *response = request->beginResponse(statusCode, "text/plain", responseContent);
        addCORSHeaders(response);
        request->send(response); });
}

void setAnalogOutputHttpEndpoint()
{
  server.on("/analogOutput", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int statusCode = 200;
    String responseContent;

    if (!request->hasParam("pin") || !request->hasParam("value")) {
      statusCode = 400;
      responseContent = "Missing 'pin' or 'value' parameter.";
    } else {
      String pinNumber = request->getParam("pin")->value();
      String valueString = request->getParam("value")->value();
      String type = request->hasParam("type") ? request->getParam("type")->value() : "value";

      int pin = pinNumber.toInt();
      float value = valueString.toFloat();

      if (type == "voltage") {
        // Adjust the value conversion as per your hardware specification
        value = (value / 3.3) * pow(2, 8); // Example for 8-bit resolution
      }

      if (value < 0 || value > 255) {
        statusCode = 400;
        responseContent = "Invalid 'value' parameter. Use a value between 0 and 255.";
      } else {
        dacWrite(pin, static_cast<int>(value));
        responseContent = "DAC Pin nr." + String(pin) + " set to " + String(static_cast<int>(value));
      }
    }

    // Create the response with the correct status code and body
    AsyncWebServerResponse *response = request->beginResponse(statusCode, "text/plain", responseContent);

    // Add CORS headers to the response
    addCORSHeaders(response);

    // Send the response
    request->send(response); });
}

void setHttpEndpoints()
{
  setPinModeHttpEndpoint();
  setDigitalOutputHttpEndpoint();
  getDigitalInputHttpEndpoint();
  getAnalogInputHttpEndpoint();
  setAnalogOutputHttpEndpoint();
}

void setup()
{
  Serial.begin(115200);
  initWifi();
  setHttpEndpoints();
  server.begin();
}

void loop()
{
  // Nothing here
}
