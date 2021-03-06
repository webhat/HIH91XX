#include <Wire.h>
#include "HIH91XX.h"
#include "HIH91XXCommander.h"



//  Create an HIH91XXCommander with I2C address 0x27, powered by digital pin 3
//  If you want to use Command Mode you MUST use the powerPin!
HIH91XXCommander hih(0x27, 3);

#define NEWADDR 0x2d



void setup()
{
  Serial.begin(9600);
  Wire.begin();
}



void loop()
{
  //  start the sensor, eeprom data is automatically read.
  hih.start();

  //  print out the whole EEPROM
  Serial.print("EEPROM:                   ");
  for(int i = 0; i < 8; ++i) {
    if(i) {
      Serial.print(' ');
    }
    Serial.print(hih.eeprom()[i]);
  }
  Serial.println();
  Serial.println();

  //  print out all properties. the changes to EEPROM have been commented out,
  //  so you don't accidentally screw up your chip.

  Serial.print("StartupMode:              ");
  Serial.println(hih.startupMode() ? "FastStartup" : "SlowStartup");
  //  set the fast startup mode (3ms)
  //  hih.setStartupMode(HIH91XXCommander::FastStartup);
  //  set the slow startup mode (10ms)
  //  hih.setStartupMode(HIH91XXCommander::SlowStartup);

  Serial.print("High alarm on (raw):      ");
  Serial.println(hih.highAlarmOn_Raw());
  //  hih.setHighAlarmOn_Raw(12345);

  Serial.print("High alarm off (raw):     ");
  Serial.println(hih.highAlarmOff_Raw());
  //  hih.setHighAlarmOff_Raw(12345);

  Serial.print("High alarm on:            ");
  Serial.println(hih.highAlarmOn());
  //  hih.setHighAlarmOn(0.5);

  Serial.print("High alarm off:           ");
  Serial.println(hih.highAlarmOff());
  //  hih.setHighAlarmOff(0.5);

  Serial.print("High alarm polarity:      ");
  Serial.println(hih.highAlarmPolarity() ? "ActiveLowPolarity" : "ActiveHighPolarity");
  //  hih.setHighAlarmPolarity(HIH91XXCommander::ActiveHighPolarity);
  //  hih.setHighAlarmPolarity(HIH91XXCommander::ActiveLowPolarity);

  Serial.print("High alarm output config: ");
  Serial.println(hih.highAlarmOutputConfig() ? "OpenDrainOuputConfig" : "PushPullOuputConfig");
  //  hih.setHighAlarmOutputConfig(HIH91XXCommander::PushPullOuputConfig);
  //  hih.setHighAlarmOutputConfig(HIH91XXCommander::OpenDrainOuputConfig);

  //  same as above for the low alarm... (not included)

  Serial.print("Customer ID:              ");
  Serial.println(hih.customerId());
  hih.setCustomerId(0xABCDEF);
  Serial.print("Customer ID:              ");
  Serial.println(hih.customerId());

  Serial.println();

  //  To write the changes to the chip use:
  //  hih.writeEEPROM();
  //  But then you still have to cycle the power for the changes to take effect, so it's easier to do:
  hih.restart();
  //  which will automatically commit all changes made.
  //  If you don't want to save your changes, use:
  //  hih.resetEEPROM();


  //  This is how you change the I2C address to 0x28:
  //  hih.setAddress(NEWADDR);
  //  hih.restart();      

  //  Make sure we're not in Command Mode any more:
  hih.leaveCommandMode();

  //  Update the sensor readings, you must call this to have current readings
  hih.update();

  Serial.print("Humidity: ");
  Serial.print(hih.humidity(), 5);
  Serial.println(" RH");

  Serial.print("Temperature: ");
  Serial.print(hih.temperature(), 5);
  Serial.println(" C");

  while(1) {
    while(Serial.available()) {
      hih.commandRequest(Serial);
    }
    delay(100);
  }
}
 
