//เครื่องควบคุมเมื่อมีการกด SW1 ให้หลอด BT LED "On" แสดงสถานะทำงานที่ 16*8 Matrix Display
//และเมื่อมีการกด SW2 ให้หลอด BT LED "Off" แสดงสถานะทำงานที่ 16*8 Matrix Display
//โดยแสดงสถานะ BT LED ทำงานบนเครือข่ายอินเตอร์เน็ต นายสุวิจักขณ์ พิมพา

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();

//#include "BluetoothSerial.h"
//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  //#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif
//BluetoothSerial SerialBT;

// Replace with your network credentials
const char* ssid = "SANGTONG 2.4G";
const char* password = "042781418";

const char* PARAM_INPUT_1 = "state";
String LED_BT_State = "on";
const int buttonPin1 = 16;
const int buttonPin2 = 14;
const int output_LED_BT = 17;

int ledState = HIGH;          // the current state of the output pin

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>KIDBRIGTH BT LED ON/OFF</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 180px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 8px; right: 0; bottom: 0; background-color: #2196F3; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 10px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #ccc}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>KIDBRIGHT BT LED ON/OFF BY 6240202512</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

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
      document.getElementById("output_LED_BT").checked = inputChecked;
      document.getElementById("output_LED_BTState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;
</script>
</body>
</html>
)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String output_LED_BTStateValue = output_LED_BTState();
    buttons+= "<h4>LED - BLUETOOTH -GPIO17 - State <span id=\"output_LED_BTState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output_LED_BT\" " + output_LED_BTStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String output_LED_BTState(){
  if(digitalRead(output_LED_BT)){
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
  pinMode(output_LED_BT, OUTPUT);
  digitalWrite(output_LED_BT, LOW);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  //BLUETOOTH
  //SerialBT.begin("TOP_BT"); //Bluetooth device name
  //Serial.println("now can pair with bluetooth!");
  
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
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output_LED_BT, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output_LED_BT)).c_str());
  });
  // Start server
  server.begin();
}

//void bt_test(){
  //if (Serial.available()) {
    //SerialBT.write(Serial.read());
  //}
  //if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
  //}
  //}
void loop() {
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(0, 0);
  matrix.writeDisplay();
  int sw1Val = digitalRead(buttonPin1);
  int sw2Val = digitalRead(buttonPin2);
    //active=low - unactive=high
    if (ledState == 1){
        matrix.clear();
        matrix.print("OFF");
        matrix.writeDisplay();
          }
    if (sw1Val == LOW) {
        Serial.println("1");
        delay(1000);
        ledState = !ledState;
    }
    if (ledState == 0){
          matrix.clear();
        matrix.print("ON!");
        matrix.writeDisplay();
          }
    if (sw2Val == LOW) {
        Serial.println("0");
        delay(1000);
        ledState = !ledState; 
    }
    
    digitalWrite(output_LED_BT, ledState);
}
