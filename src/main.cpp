#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const char *ssid = "DIGI-y6cQ";
const char *password = "D2E2PyEhkT";

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

void setHttpEndpoints()
{
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Hello");
    request->send(200, "text/plain", "Hello from ESP");
  });

  // New endpoint for controlling pins using query parameters
  server.on("/output", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if (request->hasParam("pin") && request->hasParam("state"))
    {
      String pinNumber = request->getParam("pin")->value();
      String state = request->getParam("state")->value();

      int pin = pinNumber.toInt();

      if (state == "high")
      {
        digitalWrite(pin, HIGH);
        request->send(200, "text/plain", "Pin set to HIGH");
      }
      else if (state == "low")
      {
        digitalWrite(pin, LOW);
        request->send(200, "text/plain", "Pin set to LOW");
      }
      else
      {
        request->send(400, "text/plain", "Invalid state. Use 'high' or 'low'.");
      }
    }
    else
    {
      request->send(400, "text/plain", "Missing pin or state parameter.");
    }
  });
}

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  initWifi();
  setHttpEndpoints();
  server.begin();
}

void loop()
{
  // Nothing here
}
