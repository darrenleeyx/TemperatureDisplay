#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ModbusMaster.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set up LCD (I2C address, 16 columns, 2 rows)
ModbusMaster node;

// Registers for temperature and humidity
#define TEMP_REGISTER 0x0000
#define HUM_REGISTER  0x0001

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);

  // Start RS485 communication (Modbus)
  Serial.begin(9600);  // Use Serial1 for RS485 communication
  node.begin(1, Serial);  // Slave ID = 1

  delay(1000);
}

void loop() {
  uint8_t result;
  float temperature = 0;
  float humidity = 0;

  // Read temperature and humidity values
  result = node.readHoldingRegisters(TEMP_REGISTER, 2);
  
  if (result == node.ku8MBSuccess) {
    // Read temperature and humidity data
    temperature = node.getResponseBuffer(0) / 10.0; // Convert value as necessary
    humidity = node.getResponseBuffer(1) / 10.0;    // Convert value as necessary

    // Update the LCD display with new values
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);  // Print temperature with one decimal place
    lcd.print("C");

    lcd.setCursor(0, 1);  // Move to the second row
    lcd.print("Hum: ");
    lcd.print(humidity, 1);  // Print humidity with one decimal place
    lcd.print("%");
  } else {
    // If the Modbus reading fails, display an error
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    lcd.setCursor(0, 1);
    lcd.print("Try again");
  }

  delay(2000); // Wait 2 seconds before next reading
}
