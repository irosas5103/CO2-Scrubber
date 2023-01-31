#include <Wire.h>

#include "SparkFun_SCD4x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD4x
SCD4x mySensor;

// Add fan control stuff here (ints)
int co21; // variable for INput co2 for fan control.
const int fan_control = 4; // PWM wire is connected to pin 4.
int fanc; // Variable to send speed to pwm pin.
int count=0; // counter for obtaining rpm of the fan.
unsigned long s_timer; //timer for obtaining rpm.
int rpm; // variable to store the rpm of the fans.

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println(F("SCD4x Example"));
  Wire.begin();

  // Fan control setup.
  pinMode(fan_control, OUTPUT); // PWM pin set to an output.
  analogWrite(fan_control, 0); // Start PWM value to zero.
  pinMode(5, INPUT_PULLUP); // set internal pullup resistor for tachometer.
  attachInterrupt(digitalPinToInterrupt(5), counter, RISING); // Read the rising tach pulses;

  //mySensor.enableDebugging(); // Uncomment this line to get helpful debug messages on Serial

  //.begin will start periodic measurements for us (see the later examples for details on how to override this)
  if (mySensor.begin() == false)
  {
    Serial.println(F("Sensor not detected. Please check wiring. Freezing..."));
    while (1)
      ;
  }

  //The SCD4x has data ready every five seconds
}

void loop()
{
  if (mySensor.readMeasurement()) // readMeasurement will return true when fresh data is available
  {
    
    Serial.println();

    Serial.print(F("CO2(ppm):"));
    Serial.print(mySensor.getCO2());
   
    Serial.print(F("\tTemperature(C):"));
    Serial.print(mySensor.getTemperature(), 0);
    
    Serial.print(F("\tHumidity(%RH):"));
    Serial.print(mySensor.getHumidity(), 0);
    
    Serial.println();
  }
  else
    Serial.print(F("."));

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
  delay(500);
}

void counter(){
  count++;
}