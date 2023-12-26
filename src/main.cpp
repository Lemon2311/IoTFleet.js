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
    } });
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
    } });
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

void getAnalogInputHttpEndpoint() // needs testing with hardware
{
  server.on("/analogInput", HTTP_GET, [](AsyncWebServerRequest *request)
      {
    if (!request->hasParam("pin")) {
      request->send(400, "text/plain", "Missing 'pin' parameter.");
      return;
    }

    String pinNumber = request->getParam("pin")->value();
    String type = request->hasParam("type") ? request->getParam("type")->value() : "value";
    String precisionParam = request->hasParam("precision") ? request->getParam("precision")->value() : "12";
    int pin = pinNumber.toInt();
    int precision = precisionParam.toInt();
    
    //set the resolution of analogRead return values
    analogReadResolution(precision);
    // Read the analog value
    int value = analogRead(pin);

    if (type == "voltage") {
      float maxAdcValue = pow(2, precision) - 1; // Calculate max ADC value
      float voltage = (value / maxAdcValue) * 3.3; // Convert to voltage
      String voltageStr = String(voltage, precision);
      request->send(200, "text/plain", voltageStr + "V");
    } else {
      request->send(200, "text/plain", String(value));
    } });
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
