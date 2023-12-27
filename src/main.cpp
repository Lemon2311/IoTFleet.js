#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "WiFiCredentials.h"

AsyncWebServer server(80);

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

void addCORSHeaders(AsyncWebServerResponse *response) {
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
    request->send(response);
  });
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
    request->send(response);
  });
}

void getDigitalInputHttpEndpoint() // needs testing with hardware
{
  server.on("/digitalInput", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        if (!request->hasParam("pin"))
        {
            request->send(400, "text/plain", "Missing 'pin' parameter.");
            return;
        }

        String pinNumber = request->getParam("pin")->value();

        if (!pinNumber.toInt())
        {
            request->send(400, "text/plain", "Invalid 'pin' parameter. Please use a valid integer.");
            return;
        }

        int pin = pinNumber.toInt();
        int pinState = digitalRead(pin);

        request->send(200, "text/plain", pinState == HIGH ? "HIGH" : "LOW"); });
}

void getAnalogInputHttpEndpoint() {
    server.on("/analogInput", HTTP_GET, [](AsyncWebServerRequest *request) {
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
        request->send(response);
    });
}

void setAnalogOutputHttpEndpoint() // needs testing with hardware
{
  server.on("/analogOutput", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        if (!request->hasParam("pin") || !request->hasParam("value")) {
            request->send(400, "text/plain", "Missing 'pin' or 'value' parameter.");
            return;
        }

        String pinNumber = request->getParam("pin")->value();
        String valueString = request->getParam("value")->value();
        String type = request->hasParam("type") ? request->getParam("type")->value() : "value";

        int pin = pinNumber.toInt();
        float value = valueString.toFloat();

        if (type == "voltage")//max voltage is 3.287V, voltage margin of arround 0.1V
         {
            value = (value / 3.3) * pow(2,8); // Convert from voltage to DAC value (8-bit resolution) 
        }

        if (value < 0 || value > 255) {
            request->send(400, "text/plain", "Invalid 'value' parameter. Use a value between 0 and 255.");
            return;
        }

        dacWrite(pin, static_cast<int>(value));
        request->send(200, "text/plain", "DAC Pin nr." + String(pin) + " set to " + String(value)); });
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
