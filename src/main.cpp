#include <WiFi.h>

// WiFi credentials.
const char* ssid = "";
const char* password = "";  

// Initialize the WiFi server on port 80.
WiFiServer server(80);

// Define a struct to represent each output.
struct Output {
  String name; // Name of the output as it will appear in the URL.
  int pin;     // GPIO pin number associated with the output.
  bool state;  // Current state of the output (HIGH or LOW).
};

// Declare and initialize your outputs here.
Output outputs[] = {
  {"d2", 2, HIGH},
  {"d13", 13, HIGH} // Example for GPIO3, named 'd3' in the URL.
  // Add more outputs as needed.
};
const int outputsCount = sizeof(outputs) / sizeof(outputs[0]); // Number of outputs.

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000; // Timeout for client inactivity.

void setup() {
  Serial.begin(115200); // Start the serial communication.

  // Initialize WiFi and connect to the network.
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup each output pin.
  for (int i = 0; i < outputsCount; i++) {
    pinMode(outputs[i].pin, OUTPUT); // Set the GPIO as an output.
    digitalWrite(outputs[i].pin, outputs[i].state); // Initialize the output to its default state.
  }

  server.begin(); // Start the server.
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients.

  if (client) { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    String header; // Variable to hold the HTTP request.
    String currentLine = ""; // A string to hold incoming data from the client.

    // Loop while the client's connected.
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) { // If there's bytes to read from the client,
        char c = client.read(); // Read a byte.
        header += c;
        if (c == '\n') { // If the byte is a newline character.
          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response.
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Check the HTTP request for commands to control the GPIOs.
            for (int i = 0; i < outputsCount; i++) {
              String commandOn = "GET /" + outputs[i].name + "/on";
              String commandOff = "GET /" + outputs[i].name + "/off";
              if (header.indexOf(commandOn) >= 0) {
                outputs[i].state = HIGH; // Turn the output on.
                digitalWrite(outputs[i].pin, HIGH);
              } else if (header.indexOf(commandOff) >= 0) {
                outputs[i].state = LOW; // Turn the output off.
                digitalWrite(outputs[i].pin, LOW);
              }
            }
            
            // Display the HTML web page.
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Add the rest of the HTML content here.
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line.
            client.println();
            break; // Break out of the while loop.
          } else { // If you got a newline, then clear currentLine.
            currentLine = "";
          }
        } else if (c != '\r') { // If you got anything else but a carriage return character,
          currentLine += c; // Add it to the end of the currentLine.
        }
      }
    }
    // Clear the header variable to free up memory.
    header = "";
    client.stop(); // Close the connection.
  }
}
