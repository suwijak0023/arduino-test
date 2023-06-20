#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
const char* ssid = "Sangtong";
const char* password = "042781418";
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.1.131/SENSOR/esp-post-data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "DHT";
String sensorLocation = "HOME";

int LED1State = 0;
int LED1 = 12;

int SW1 = 26;
int SW1Old = 1;
int SW1New;


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(SW1, INPUT);
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void swti(){
  {
  SW1New = digitalRead(SW1);
  if (SW1Old == 0 && SW1New == 1)
  {
    if (LED1State == 0) {
      digitalWrite(LED1, HIGH);
      delay(200);
      LED1State = 1;
      Serial.println("SW1 is pressed & LED1 ON & SENDING");
      
    }
    else {
      digitalWrite(LED1, LOW);
      LED1State = 0;
      Serial.println("SW1 is pressed & LED1 OFF & DISABLE");
    }
  }
  SW1Old = SW1New;
  
}
  }

void loop() {
  swti();
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                             + "&location=" + sensorLocation + "&value1=" + String(LED1State)
                             + "&value2=" + String(LED1State) + "&value3=" + (LED1State);
                             delay(5000);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
}
  
