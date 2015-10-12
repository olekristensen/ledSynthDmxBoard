#include "utils.h"
#include <Wire.h>
#include "Biquad.h"
#include "DmxUniverse.h"
#include <DmxSimple.h>

#define DMX_MASTER_CHANNELS 200

int intensityPromille = 0;
int temperatureKelvin = 2700;

Biquad *intensityFilter = new Biquad(bq_type_lowpass, 2.5 / 20,  0.7071, 0.0);
Biquad *temperatureFilter = new Biquad(bq_type_lowpass, 2.5 / 20,  0.7071, 0.0);

DmxUniverse dmxUniverse(DMX_MASTER_CHANNELS);

void setup() {

  pinMode(13, OUTPUT);

  // WIRE

  Wire.setClock(400);
  Wire.begin(100);              // join i2c bus with address #100 0x64
  Wire.onReceive(receiveEvent); // register event


  // DMX SHIELD

  DmxSimple.usePin(4);

  DmxSimple.maxChannel(DMX_MASTER_CHANNELS);

  // Set shield to output mode
  pinMode (2, OUTPUT);
  digitalWrite (2, HIGH);

  // DMX FIXTURES

  /*

  20 Runde Elforsk Spots starter paa 20
  001- int
  002- temp
  - 41

  12 Strips starter paa 101
  101-vw 1
  102-cw 1
  103-vw 2
  104-cw 2

  */

  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
}

void loop() {

  // FILTER OUTPUT

//  float intensityPromilleOutputFiltered = intensityFilter->process(intensityPromille);
//  float temperatureKelvinOutputFiltered = temperatureFilter->process(temperatureKelvin);

  float intensityPromilleOutputFiltered = intensityPromille;
  float temperatureKelvinOutputFiltered = temperatureKelvin;

  dmxUniverse.setIntensity(constrain(intensityPromilleOutputFiltered / 1000.0, 0.0, 1.0));
  dmxUniverse.setTemperatureKelvin(round(temperatureKelvinOutputFiltered));

  //  dmxUniverse.updateDmxMaster(dmx_master);
  dmxUniverse.updateDmxSimple();

}

void receiveEvent(int howMany)
{
  digitalWrite(13, HIGH);   // turn the LED on

  byte hb;
  byte lb;

  if (Wire.available() == 4)   // if four bytes were received
  {
    lb = Wire.read();
    hb = Wire.read();
    intensityPromille = word(hb, lb);
    lb = Wire.read();
    hb = Wire.read();
    temperatureKelvin = word(hb, lb);
     digitalWrite(13, LOW);
  }
}
