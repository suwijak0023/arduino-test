//เครื่องควบคุมให้หลอด WIFI LED ทำงาน On เมื่อมีค่า Temperature Sensor เท่ากับหรือมากกว่า 28 C และหลอด
//WIFI LED ทำงาน Off เมื่อมีค่า Temperature Sensor เท่ากับหรือต่ำกว่ากว่า 26 C โดยแสดงค่า Temperature Sensor กับ
//สถานะ  WIFI LED ทำงานบนเครือข่ายอินเตอร์เน็ตพร้อมแสดงค่า
//Temperature Sensor ที่ 16*8 Matrix Display
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_GFX.h>
#define LM73_ADDR 0x4D
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();

// Replace with your network credentials
const char* ssid = "SANGTONG 2.4G";
const char* password = "042781418";

int analog_value = 0;
double temp = 0;
String LED_WIFI_State = "off";
const int buttonPin1 = 16;
const int buttonPin2 = 14;
const int output_LED_WIFI = 2;
int ledState = LOW;          // the current state of the output pin

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>KIDBRIGTH WIFI LED ON/OFF</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>KIDBRIGHT WIFI LED ON/OFF</h2>
  %BUTTONPLACEHOLDER%
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
<script>


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "Off";
      }
      else { 
        inputChecked = false;
        outputStateM = "On";
      }
      document.getElementById("output_LED_WIFI").checked = inputChecked;
      document.getElementById("output_LED_WIFIState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

</script>
</body>
</html>
)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String output_LED_WIFIStateValue = output_LED_WIFIState();
    buttons+= "<h4>LED - WIFI -GPIO2 - State <span id=\"output_LED_WIFIState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output_LED_WIFI\" " + output_LED_WIFIStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String output_LED_WIFIState(){
  if(digitalRead(output_LED_WIFI)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Wire1.begin(4, 5);
  matrix.begin(0x70);
  pinMode(output_LED_WIFI, OUTPUT);
  digitalWrite(output_LED_WIFI, LOW);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;


    //เปลื่ยนเป็น float หรือ float เป็น str
  //server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readTemperature().c_str());
  //});
    
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output_LED_WIFI)).c_str());
  });
  // Start server
  server.begin();
}
float readTemperature() {
  Wire1.beginTransmission(LM73_ADDR);
  Wire1.write(0x00); // Temperature Data Register
  Wire1.endTransmission();
  uint8_t count = Wire1.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire1.read();
    buff[1] = Wire1.read();
    temp += (int)(buff[0]<<1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[0]&0b10000000) temp *= -1.0;
  }
  return temp;
}
void loop() {
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(0, 0);
  matrix.writeDisplay();
  int temp = readTemperature();
  Serial.print("Temp:");Serial.println(temp);
  delay(0.1);    
    if (temp <= 26 ) {
        matrix.clear();
        matrix.print("ON");
        matrix.writeDisplay();
        Serial.println("1");
        delay(1000);
        ledState = 0;
    }
    else {
      matrix.clear();
        matrix.print("OFF!");
        matrix.writeDisplay();
        Serial.println("0");
        delay(1000);
        ledState = 1; 
    }
    digitalWrite(output_LED_WIFI, ledState);
}
