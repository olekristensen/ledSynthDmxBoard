#include <DmxMaster.h>
#include <Wire.h>

int dmxChannels = 0;
byte dmx[512];

void setup() {
  DmxMaster.usePin(3);

  for(int i = 0; i < 512; i++){
    dmx[i] = 0;
  }

  Wire.begin(100);                // join i2c bus with address #100 0x64
  Wire.onReceive(receiveEvent); // register event

  Serial.begin(9600);

}

void loop() {
  for (int i = 0; i < dmxChannels; i++) {
        DmxMaster.write(i+1, dmx[i]);
  }
  
}

void receiveEvent(int howMany)
{
  int channel = 0;
  while (Wire.available()) // loop through all
  {
    dmx[channel++] = Wire.read();
  }
  dmxChannels = howMany;
}


