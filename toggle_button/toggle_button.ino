int button = 4;
int led = 15;
int status = false;
//#define SW1  0
//int SW1State = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT_PULLUP);
//  pinMode(SW1,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button)==true){
    status=!status;
    Serial.println(status);
//    digitalWrite(led, status);
Serial.println("SW1 is Pressed ON");
    }
    while(digitalRead(button==true));
    delay(50);
}
//    if (status == 1) {
//    digitalWrite(led, HIGH);
////    delay(50);
//  }
//  else{
//    digitalWrite(led, LOW);
//    }
//    if (digitalRead(SW1) == LOW) {
//    Serial.println("SW1 is Pressed ON");
//    SW1State = 1;
//  }
//  else {
//    SW1State = 0;
//    Serial.println("SW1 is NOT pressed DISABLE");
//  }while(digitalRead(SW1State==1));
//    delay(50);
