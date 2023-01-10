#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD4x

// Make instances for both sensors.
SCD4x Sensor1; // Input sensor
SCD4x Sensor2; // Output sensor

// Code for selecting the I2C bus.
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

// Add fan control stuff here (ints)
int co21; // variable for INput co2 for fan control.
int co22; // variable for OUTput co2 for fan control.

// Get values from the sensors.  Bus number must be the same as the sensor designated as input or output.
void printInput(int bus) {
  TCA9548A (bus);
  Serial.print("Sensor number on bus");
  Serial.println(bus);
  Serial.print(F("CO2(ppm):"));
  Serial.print(Sensor1.getCO2());
  Serial1.print("co2I.val=");
  Serial1.print(Sensor1.getCO2());
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  co21 = Sensor1.getCO2();

  Serial.print(F("\tTemperature(C):"));
  Serial.print(Sensor1.getTemperature(), 0);
  Serial1.print("temp.val=");
  Serial1.print(Sensor1.getTemperature(), 0);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  Serial.print(F("\tHumidity(%RH):"));
  Serial.print(Sensor1.getHumidity(), 0);
  Serial1.print("humid.val=");
  Serial1.print(Sensor1.getHumidity(), 0);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

    Serial.println();
}

void printOutput(int bus){
  TCA9548A (bus);
  Serial.print("Sensor number on bus");
  Serial.println(bus);
  Serial.print(F("CO2(ppm):"));
  Serial.print(Sensor2.getCO2());
  Serial1.print("co2O.val=");
  Serial1.print(Sensor2.getCO2());
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  co22 = Sensor2.getCO2();
  Serial.println();
}

void setup(){
  // Start serial and I2C.
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();

 // Add fan control setups here.


 // Init sensor on bus number 2
  TCA9548A(2);
  if (!Sensor2.begin(0x62)) {
    Serial.println("Could not find a sensor on bus 2, check wiring!");
    while (1);
  }
  Serial.println();

 // Init sensor on bus number 7
  TCA9548A(7);
  if (!Sensor1.begin(0x62)) {
    Serial.println("Could not find a sensor on bus 7, check wiring!");
    while (1);
  }
  Serial.println();
}

void loop(){
  //Print values for the sensors to the display.
  printInput(7);
  printOutput(2);
  delay(5000);
}
