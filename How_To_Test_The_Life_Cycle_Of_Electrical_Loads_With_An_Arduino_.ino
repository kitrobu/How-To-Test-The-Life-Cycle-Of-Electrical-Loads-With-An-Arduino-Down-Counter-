#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Define the LCD screen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the relay pin
int relayPin = 13;

// Define the keypad layout
const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Define a custom character (Arrow)
byte arrow[8] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111,
  B01010,
  B01010,
};

void setup() {
  pinMode(relayPin, OUTPUT);
  lcd.begin();
  lcd.createChar(0, arrow);  // Define custom character at position 0
  
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Load Test");
  delay(2000);  // Display the welcome message for 2 seconds
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Enter Load Value");
}

void loop() {
  static int loadValue = 0; // Declare loadValue at the beginning of loop

  char key = keypad.getKey();

  if (key && (key >= '0' && key <= '9')) {
    if (loadValue < 100) {
      loadValue = loadValue * 10 + (key - '0');
      if (loadValue > 999) {
        loadValue = 999; // Limit maximum input value to 999
      }
      
      lcd.setCursor(0, 1);
      lcd.print("Load: ");
      lcd.print(loadValue);
      lcd.write(byte(0));  // Display custom character (Arrow)
      delay(500); // Small delay for better input readability
    }
  }
  else if (key == 'D') { // Change from '#' to 'D' for enter key
    if (loadValue > 0) {
      startLoadTest(loadValue);
      loadValue = 0; // Reset loadValue after the test
    }
  }
}

void startLoadTest(int initialLoad) {
  int stepSize = 1; // Set your step size
  for (int load = initialLoad; load >= 0; load -= stepSize) {
    lcd.clear();
    lcd.print("Load: ");
    lcd.print(load);
    lcd.write(byte(0));  // Display custom character (Arrow)
    delay(1000);

    activateRelay(); // Activate relay for load testing

    delay(2000); // Adjust the delay based on your load characteristics

    deactivateRelay(); // Deactivate relay

    delay(1000); // Add a delay between tests (adjust as needed)
  }

  lcd.clear();
  lcd.print("Test complete");
  delay(2000); // Display "Test complete" for 2 seconds

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Load Value");
}

void activateRelay() {
  digitalWrite(relayPin, HIGH);
}

void deactivateRelay() {
  digitalWrite(relayPin, LOW);
}
