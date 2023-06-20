//void SwitchLdr() {  //1time on/off
//  {
//
//    SWstate = digitalRead(SW);
//    Serial.println(SWstate);
//    
////    while (digitalRead(SW == true));
////    delay(50);
//    
//    if (SWstate == 1) {
//      ldr();
//      digitalWrite(LED1, HIGH);
//      delay(200);
//      digitalWrite(LED1, LOW);
//      delay(200);
//      Serial.println("SW is pressed & LDR AUTO");
//    }
//    else {
//      digitalWrite(LED1, HIGH);
//      digitalWrite(LED2, LOW);
//      Serial.println("SW is pressed & LDR MANUAL");
//      SWLstate = digitalRead(SWL);
//      if (digitalRead(SWLstate) == 1) {
//        digitalWrite(LED2, HIGH);
//      }
//      else {
//        digitalWrite(LED2, LOW);
//      }
//
//    }
//  }
//}
