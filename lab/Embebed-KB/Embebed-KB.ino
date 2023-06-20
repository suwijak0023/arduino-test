//เครื่องควบคุมเมื่อมีการกด SW1 ให้หลอด BT LED "On" แสดงสถานะทำงานที่ 16*8 Matrix Display
//และเมื่อมีการกด SW2 ให้หลอด BT LED "Off" แสดงสถานะทำงานที่ 16*8 Matrix Display
//โดยแสดงสถานะ BT LED ทำงานบนเครือข่ายอินเตอร์เน็ต

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#define SEND_DELAY 200
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
// Replace with your network credentials
const char* ssid = "SANGTONG 2.4G";
const char* password = "042781418";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String LED_BT_State = "off";
const int sw1 = 16;
const int sw2 = 14;
const int LED_BT = 17;
// Assign output variables to GPIO pins
//const int output26 = 17;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  Wire1.begin(4, 5);
  matrix.begin(0x70);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);

  pinMode(LED_BT, OUTPUT);
  digitalWrite(LED_BT, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  WiFiClient client = server.available();   // Listen for incoming clients

  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(0, 0);
  matrix.writeDisplay();
  int sw1Val = digitalRead(sw1);
  int sw2Val = digitalRead(sw2);

  if (sw1Val == LOW) {
    matrix.clear();
    matrix.print("ON!");
    matrix.writeDisplay();
    Serial.println("ON");
    delay(1000);
    digitalWrite(LED_BT, LOW);
    client.println("<p><a href=\"/17/on\"><button class=\"button\">BLUETOOTH-ON</button></a></p>");
  }
  else if (sw2Val == LOW) {
    matrix.clear();
    matrix.print("OFF!");
    matrix.writeDisplay();
    Serial.println("OFF");
    delay(1000);
    digitalWrite(LED_BT, HIGH);

    client.println("<p><a href=\"/17/off\"><button class=\"button button2\">BLUETOOTH-OFF</button></a></p>");
  }

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /17/on") >= 0) {
              Serial.println("ON");
              LED_BT_State = "on";

            } else if (header.indexOf("GET /17/off") >= 0) {
              Serial.println("OFF");
              LED_BT_State = "off";
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>BLUETOOTH ON/OFF</h1>");
            client.println("<p>LED_BT - State " + LED_BT_State + "</p>");
            if (LED_BT_State == "off") {
              matrix.clear();
              matrix.print("OFF!");
              matrix.writeDisplay();
              digitalWrite(LED_BT, HIGH);
              client.println("<p><a href=\"/17/on\"><button class=\"button\">BLUETOOTH-ON</button></a></p>");
            } else {
              matrix.clear();
              matrix.print("ON!");
              matrix.writeDisplay();
              digitalWrite(LED_BT, LOW);
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">BLUETOOTH-OFF</button></a></p>");
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";

    Serial.println("");

  }
}
