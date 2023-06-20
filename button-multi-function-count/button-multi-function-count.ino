#include <ezButton.h>

ezButton button(12);  // create ezButton object that attach to pin 7;
unsigned long lastCount = 0;
unsigned long count = 0;


void setup() {
  Serial.begin(115200);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.setCountMode(COUNT_FALLING);
}

void loop() {
  button.loop(); // MUST call the loop() function first

  count = button.getCount();

  if (count != lastCount) {
    Serial.println(count);

    int countIn3 = count % 3 + 1;

    switch (countIn3) {
      case 1:
        // TO DO TASK 1
//        Serial.print("1");
        break;

      case 2:
        // TO DO TASK 2
//        Serial.print("2");
        break;

      case 3:
        // TO DO TASK 3
//        Serial.print("3");
        break;
    }

    lastCount = count;
  }
}
