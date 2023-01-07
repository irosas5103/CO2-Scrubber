#include <Wire.h>

#include "SparkFun_SCD4x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD4x
SCD4x mySensor;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println(F("SCD4x Example"));
  Wire.begin();

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
    Serial1.print("co2I.val=");
    Serial1.print(mySensor.getCO2());
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

    Serial.print(F("\tTemperature(C):"));
    Serial.print(mySensor.getTemperature(), 0);
    Serial1.print("temp.val=");
    Serial1.print(mySensor.getTemperature(), 0);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

    Serial.print(F("\tHumidity(%RH):"));
    Serial.print(mySensor.getHumidity(), 0);
    Serial1.print("humid.val=");
    Serial1.print(mySensor.getHumidity(), 0);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

    Serial.println();
  }
  else
    Serial.print(F("."));

  delay(500);
}