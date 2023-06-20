const int buttonPin = 4;     // the number of the pushbutton pin
const int ledPin =  15;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int count_value =0;
int prestate =0;
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, then the buttonState is HIGH:
  if (buttonState == HIGH && prestate == 0) {
    count_value++;
    Serial.println(count_value);
    // turn LED on
    digitalWrite(ledPin, HIGH);
    delay(100);
    // turn LED off
    digitalWrite(ledPin, LOW);

    prestate = 1;
  } else if(buttonState == LOW) {
    prestate = 0;
  }
}
