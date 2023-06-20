int BUTTON = 4;
int BUTTONstate = LOW;
int ledState = LOW;
int previousButtonState;
int presentButtonState;

//int SW = 12; //On/Off
//int SWstate;

int SWL = 4; //On/Off
int SWLstate;

int status = 0;

int LED1 = 13;
int LED2 = 15;
int LDR = 14;

int analogPin = 5;
int val = 0;
int timer = 100;

void Ldr() {

  val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog ขา5 ที่ต่อกับ LDR
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val

  if (val < 100) { // ค่า 100 สามารถกำหนดปรับได้ตามค่าแสงในห้องต่างๆ
    digitalWrite(LED2, HIGH); // สั่งให้ LED ติดสว่าง
  }
  else {
    digitalWrite(LED2, LOW); // สั่งให้ LED ดับ
  }
  delay(100);
}


void Lamp() {  // 1 Time on/off
  SWLstate = digitalRead(SWL);
  Serial.print(SWLstate);

  if (SWLstate == 1) {
    digitalWrite(LED2, HIGH);
  }
  else {
    digitalWrite(LED2, LOW);
  }
}

void SwitchLdr() {

  previousButtonState    = presentButtonState;
  presentButtonState = digitalRead(BUTTON);
  if (previousButtonState == HIGH && presentButtonState == LOW) {
    Serial.println("The button is pressed");
    BUTTONstate = !BUTTONstate;
    Serial.println(BUTTONstate);

    if (BUTTONstate == HIGH) {
      digitalWrite(LED1, HIGH);
      delay(200);
      digitalWrite(LED1, LOW);
      delay(200);
      Serial.println("SW is pressed & LDR AUTO");
    }
    else {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      Serial.println("SW is pressed & LDR MANUAL");
    }

  }
}
//void SwitchLdr() {  //1time on/off
//
//  SWstate = digitalRead(SW);
//  Serial.println(SWstate);
//
//  if (SWstate == 1) {
//    Ldr();
//    digitalWrite(LED1, HIGH);
//    delay(200);
//    digitalWrite(LED1, LOW);
//    delay(200);
//    Serial.println("SW is pressed & LDR AUTO");
//  }
//  else {
//    digitalWrite(LED1, HIGH);
//    digitalWrite(LED2, LOW);
//    Serial.println("SW is pressed & LDR MANUAL");
//    Lamp();
//  }
//}


void setup() {

  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);
//  pinMode(SW, INPUT);
  pinMode(SWL, INPUT);

}


void loop() {
  SwitchLdr();

  //  // Switch case
  //  Serial.println("Enter Your state"); //Toggle
  //  while (Serial.available() == 0) {
  //  }
  //  int Choice = Serial.parseInt();
  //
  //  switch (Choice) {
  //
  //    case 4:   //เปิด
  //      Serial.print("SW is pressed & LDR AUTO");
  //
  //      digitalWrite(LED1, HIGH);
  //      delay(200);
  //      digitalWrite(LED1, LOW);
  //      delay(200);
  //      Ldr;
  //
  //      break;
  //
  //    case 5: //ปิด
  //      digitalWrite(LED1, HIGH);
  //      Serial.print("SW is pressed & LDR MANUAL");
  //      Lamp();
  //      break;
  //
  //    default:
  //
  //      Serial.println("Please choose a valid selection");
  //  }


}
