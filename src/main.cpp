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

void setDigitalOutputHttpEndpoint()
{
  // New endpoint for controlling pins using query parameters
  server.on("/digitalOutput", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    if (!request->hasParam("pin") || !request->hasParam("state"))
    {
      request->send(400, "text/plain", "Missing 'pin' or 'state' parameter.");
      return;
    }

    String pinNumber = request->getParam("pin")->value();
    String state = request->getParam("state")->value();

    if (!pinNumber.toInt())
    {
      request->send(400, "text/plain", "Invalid 'pin' parameter. Please use a valid integer.");
      return;
    }

    int pin = pinNumber.toInt();

    if (state == "high")
    {
      digitalWrite(pin, HIGH);
      request->send(200, "text/plain", "Pin nr."+String(pin)+" set to HIGH");
    }
    else if (state == "low")
    {
      digitalWrite(pin, LOW);
      request->send(200, "text/plain", "Pin nr."+String(pin)+" set to LOW");
    }
    else
    {
      request->send(400, "text/plain", "Invalid 'state' parameter. Use 'high' or 'low'.");
    }
  });
}

void setPinModeHttpEndpoint()
{
  // New endpoint for initializing digital pins using query parameters
  server.on("/initializeDigitalPin", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    if (!request->hasParam("pin") || !request->hasParam("mode"))
    {
      request->send(400, "text/plain", "Missing 'pin' or 'mode' parameter.");
      return;
    }

    String pinNumber = request->getParam("pin")->value();
    String mode = request->getParam("mode")->value();

    if (!pinNumber.toInt())
    {
      request->send(400, "text/plain", "Invalid 'pin' parameter. Please use a valid integer.");
      return;
    }

    int pin = pinNumber.toInt();

    if (mode == "input")
    {
      pinMode(pin, INPUT);
      request->send(200, "text/plain", "Digital pin nr."+String(pin)+" initialized as INPUT");
    }
    else if (mode == "output")
    {
      pinMode(pin, OUTPUT);
      request->send(200, "text/plain", "Digital pin nr."+String(pin)+" initialized as OUTPUT");
    }
    else
    {
      request->send(400, "text/plain", "Invalid 'mode' parameter. Use 'input' or 'output'.");
    }
  });
}

void getDigitalInputHttpEndpoint()//needs testing with hardware
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

        request->send(200, "text/plain", pinState == HIGH ? "HIGH" : "LOW");
    });
}

void setHttpEndpoints()
{
  setPinModeHttpEndpoint();
  setDigitalOutputHttpEndpoint();
  getDigitalInputHttpEndpoint();
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
