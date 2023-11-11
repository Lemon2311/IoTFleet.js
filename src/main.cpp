#include <WiFi.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

String header;
String output2State = "off";

const int output2 = 2;
const int input5 = 5;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

const long debounceDelay = 50;
bool lastButtonState;
bool buttonState;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(output2, OUTPUT);
  digitalWrite(output2, LOW);
  pinMode(input5, INPUT_PULLUP);
  lastButtonState = digitalRead(input5);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // Sync the label with the initial state
  if(digitalRead(output2) == HIGH){
    output2State = "on";
  } else {
    output2State = "off";
  }
}

void loop(){
  WiFiClient client = server.available();
  bool reading = digitalRead(input5);
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
      lastButtonState = reading;
      if (lastButtonState == LOW) {
        output2State = output2State == "off" ? "on" : "off";
        digitalWrite(output2, output2State == "on" ? HIGH : LOW);
      }
    }
  }

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /2/on") >= 0) {
              output2State = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              output2State = "off";
              digitalWrite(output2, LOW);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Arial; display: flex; justify-content: center; align-items: center; height: 100vh; }");
            client.println(".button { border: none; color: white; padding: 16px 40px; text-align: center; display: inline-block; font-size: 30px; margin: 4px 2px; transition-duration: 0.4s; cursor: pointer; }");
            client.println(".button1 { background-color: white; color: black; border: 2px solid #4CAF50; }");
            client.println(".button1:hover { background-color: #4CAF50; color: white; }");
            client.println(".button2 { background-color: white; color: black; border: 2px solid #f44336; }");
            client.println(".button2:hover { background-color: #f44336; color: white; }</style></head>");
            client.println("<body><h1>Home Automation</h1>");
            
            if (output2State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button button1\">Light ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">Light OFF</button></a></p>");
            } 
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}
