void setup() {
  Serial.begin(115200);
  delay(200);
}

void loop() {
  if(Serial.available()){
    char c = Serial.read(); //อ่านค่าจาก Serial port โดยเก็บค่าเป็นตัวอักษร
    
    switch (c){
      case 'A':
        Serial.print("c = ");
        Serial.println(c);
        break; //การพักคำสั่ง เมื่อ case ตรงตามเงื่อนไขจะออกจาก switch ทันที
      case 'B':
        Serial.print("c = ");
        Serial.println(c);
        break;
      case '\n': //การกดปุ่ม Enter
        break;
      default: //เมื่อตัวอักษรที่เก็บในตัวแปร c ไม่ใช่ A หรือ B
        Serial.println("เงื่อนไขอื่นๆ ");
        Serial.print("c = ");
        Serial.println(c);
        break;
    }
  }
}
