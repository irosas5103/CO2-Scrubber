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
const int fan_control = 4; // PWM wire is connected to pin 4.
int fanc; // Variable to send speed to pwm pin.
int count=0; // counter for obtaining rpm of the fan.
unsigned long s_timer; //timer for obtaining rpm.
int rpm; // variable to store the rpm of the fans.

// Add variables for the battery charge controller
float batt; // stores battery voltage
float batt_low = 12.0; // stores lowest value (starts charge)
float batt_high = 13.5; // stores highest values (stops charge)
int batt_level; // variable for the display

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

 // Fan control setup.
  pinMode(fan_control, OUTPUT); // PWM pin set to an output.
  analogWrite(fan_control, 0); // Start PWM value to zero.
  pinMode(5, INPUT_PULLUP); // set internal pullup resistor for tachometer.
  attachInterrupt(digitalPinToInterrupt(5), counter, RISING); // Read the rising tach pulses;

 // Battery charge controller
  pinMode(3, OUTPUT); // Relay control pin for charging the battery.
  
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
  delay(500);

  //Set fan speed and send rpm to display.
  if (co21>5000){
    co21=5000; //Set the top end value to 5000 ppm so that the PWM cycle does not reset.
  }

  if (co21<500){
    analogWrite(fan_control, 0); //Any value under 500 ppm sets the fan speed to zero.
  }

  fanc = map(co21, 500, 2500, 0, 255); //set ppm value to PWM speed.
  analogWrite(fan_control, fanc); // Set the speed.

  //Calculate rpm and send to display.
  s_timer = millis();
  count=0;
  while ((millis()-s_timer)<1000){
    }

  rpm=count*60/2;

  Serial.print("RPM: ");
  Serial.println(rpm);
  Serial1.print("rpm.val=");
  Serial1.print(rpm);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);  

  // Battery Charge controller will start on batt_low and cut off at batt_high
  batt = (analogRead(A6)*(2.759/1023)*5.3);  // This equation converts the voltage seen by the analog pin to the actual battery voltage.
  batt_level = map(batt,11.8, 13.8, 0, 100); // set the value to 0-100% battery for the display

  Serial.print("Battery Voltage: ");
  Serial.println(batt);
  Serial1.print("batt.val=");
  Serial1.print(batt_level);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff); 

  if (batt<batt_low){ 
    digitalWrite(3, HIGH); // battery is below threshold, start charging by turning relay on
    Serial.println("Charging Battery");
  }

  if (batt>batt_high){
    digitalWrite(3, LOW); // battery is above high limit, stop charging by turning relay off
    Serial.print("Not Charging");
  }
}

void counter(){
  count++;
}