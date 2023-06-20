#include <WiFi.h>
#include <WiFiManager.h>
#include <Ticker.h>
Ticker ticker;

#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define LED  2
#define SW1  26

const char* serverName = "http://192.168.1.131/SENSOR/esp-post-data.php";
//"http://sangtongshop.000webhostapp.com/post-esp-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "DHT";
String sensorLocation = "HOME";

#define DHTPIN 27     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}


// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") {
    return readDHTHumidity();
  }
  return String();
}

void tick(){
  digitalWrite(LED,!digitalRead(LED));
}

void configModeCallback(WiFiManager *myWiFiManager){
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void Wifi_Reset_begin(){
  if(digitalRead(SW1) == LOW) {
    Serial.println("Wifi Reset? Pls. waiting 3S..");
    delay(3000);
    if (digitalRead(SW1) == LOW) {
      delay(10);
      while (digitalRead(SW1) == LOW){
        digitalWrite(LED, HIGH);
        delay(10);
      }
      Serial.println("WiFi Reset Settings....OK!.");
      WiFiManager wm;
      wm.resetSettings(); 
      ESP.restart();  
    }
  }
  
  
}

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  Serial.begin(115200);
  dht.begin();
  ticker.attach(1,tick);
  // Connect to Wi-Fi
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if( !wm.autoConnect("WiFI_ABC_AP") ){
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);   
  }
  
  ticker.detach();
  digitalWrite(LED, LOW);
  
  delay(100);
  Serial.println("");
  Serial.println("connected...already..WiFi :)");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Wifi_Reset_begin();
  delay(1000);
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
                             + "&location=" + sensorLocation + "&value1=" + String(dht.readTemperature())
                             + "&value2=" + String(dht.readHumidity()) + "&value3=" + "75";
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
