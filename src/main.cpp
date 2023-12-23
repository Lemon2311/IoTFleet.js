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

struct Input {
  String name; // Name of the output as it will appear in the URL.
  int pin;     // GPIO pin number associated with the output.
  bool state;  // Current state of the output (HIGH or LOW).
};

// Declare and initialize your outputs here.
Output outputs[] = {
  {"d2", 2, HIGH},
  {"d13", 13, HIGH} // Example for GPIO13, named 'd13' in the URL.
};

Input inputs[] = {
  {"d2", 2, HIGH},
  {"d13", 13, HIGH} // Example for GPIO13, named 'd13' in the URL.
};



const int outputsCount = sizeof(outputs) / sizeof(outputs[0]); // Number of outputs.
const int inputsCount = sizeof(inputs) / sizeof(inputs[0]);

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000; // Timeout for client inactivity.

// Function to connect to the WiFi network.
void connectToWiFi() {
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
}

// Function to initialize each output pin.
void initializeOutputs() {
  for (int i = 0; i < outputsCount; i++) {
    pinMode(outputs[i].pin, OUTPUT); // Set the GPIO as an output.
    digitalWrite(outputs[i].pin, outputs[i].state); // Initialize the output to its default state.
  }
}

void initializeInputs() {
  for (int i = 0; i < inputsCount; i++) {
    pinMode(inputs[i].pin, INPUT); // Set the GPIO as an output.
    digitalWrite(inputs[i].pin, inputs[i].state); // Initialize the output to its default state.
  }
}

// Function to send the HTTP response back to the client.
void sendHTTPResponse(WiFiClient &client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // HTML content for the client.
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // ... more HTML content can be added here ...
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line.
  client.println();
}

// Function to parse the HTTP request and control the outputs.
void parseHTTPRequest(String header) 
{
parseHTTPOutputRequest(header);
//commented for now, as it is not fully implemented
  //parseHTTPInputRequest(header);

}

void parseHTTPOutputRequest(String header) {
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
}

void parseHTTPInputRequest(String header) {
  for (int i = 0; i < inputsCount; i++) {
   
    String commandOff = "GET /" + inputs[i].name;
    digitalRead(inputs[i].pin);


    }
  }




// Function to handle incoming client connections.
void handleClient(WiFiClient client) {
  currentTime = millis();
  previousTime = currentTime;
  String header; // Variable to hold the HTTP request.
  
  // Loop while the client's connected.
  while (client.connected() && currentTime - previousTime <= timeoutTime) {
    currentTime = millis();
    if (client.available()) { // If there's bytes to read from the client,
      char c = client.read(); // Read a byte.
      header += c;
      if (c == '\n') { // If the byte is a newline character.
        // If the current line is blank, you got two newline characters in a row.
        // That's the end of the client HTTP request, so send a response.
        if (header.indexOf("\r\n\r\n") >= 0) {
          // Parse the HTTP request and control the outputs.
          parseHTTPRequest(header);
          // Send the HTTP response with HTML content.
          sendHTTPResponse(client);
          break;
        }
      }
    }
  }
  client.stop(); // Close the connection.
}

void setup() {
  Serial.begin(115200); // Start the serial communication.
  connectToWiFi();      // Connect to WiFi network.
  initializeOutputs();  // Initialize output pins.
  server.begin();       // Start the server.
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients.
  if (client) { // If a new client connects,
    handleClient(client); // Handle the client connection.
  }
}
