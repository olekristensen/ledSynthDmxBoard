#include "utils.h"
#include <Wire.h>
#include "Biquad.h"
#include <Conceptinetics.h>
#include "DmxUniverse.h"

#define DMX_MASTER_CHANNELS 200
#define RXEN_PIN            2

DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );

unsigned int intensityPromille = 0;
unsigned int temperatureKelvin = 2700;

float intensityPromilleOutputFiltered = intensityPromille;
float temperatureKelvinOutputFiltered = temperatureKelvin;

Biquad *intensityFilter = new Biquad(bq_type_lowpass, 2.5 / 20.0,  0.7071, 0.0);
Biquad *temperatureFilter = new Biquad(bq_type_lowpass, 2.5 / 20.0,  0.7071, 0.0);

DmxUniverse dmxUniverse(DMX_MASTER_CHANNELS);

void setup() {

  // WIRE

  Wire.setClock(400000L);
  //Wire.setClock(400);
  Wire.onRequest(requestEvent); // register event
  Wire.begin(100);              // join i2c bus with address #100 0x64

  pinMode(13, OUTPUT);
  digitalWrite (13, LOW);

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

  // 001
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 011
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 021
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 031
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 041
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 051
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 061
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 071
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 081
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 091
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());
  dmxUniverse.addFixture(new ElforskSpot());

  // 101
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());

  // 111
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());

  // 121
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());

  // 131
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());

  // 141
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());
  dmxUniverse.addFixture(new ElforskStrip());

  // DMX SHIELD

  dmx_master.enable ();  
  
  // Set channel 1 - 50 @ 50%
  dmx_master.setChannelRange ( 1, DMX_MASTER_CHANNELS, 0 );

}

void loop() {

  intensityPromille = round(mapFloat(analogRead(A0), 0.0, 1023.0, 0.0, 1000.0));
  temperatureKelvin = map(analogRead(A1), 0, 1023, dmxUniverse.getKelvinLow(), dmxUniverse.getKelvinHigh());

  // FILTER OUTPUT

  intensityPromilleOutputFiltered = intensityFilter->process(intensityPromille);
  temperatureKelvinOutputFiltered = temperatureFilter->process(temperatureKelvin);

  analogWrite(9, map(round(intensityPromille), 0, 1000, 0, 255));

  static int dimmer_val;

  intensityPromilleOutputFiltered = dimmer_val++ %1000;
  //temperatureKelvinOutputFiltered = temperatureKelvin;

  dmxUniverse.setIntensity(constrain(intensityPromilleOutputFiltered / 1000.0, 0.0, 1.0));
  dmxUniverse.setTemperatureKelvin(round(temperatureKelvinOutputFiltered));

  dmxUniverse.updateDmxMaster(dmx_master);
  //dmxUniverse.updateDmxSimple();
}

void requestEvent()
{
  byte tStartLow = lowByte(dmxUniverse.getKelvinLow());
  byte tStartHi = highByte(dmxUniverse.getKelvinLow());
  byte tEndLow = lowByte(dmxUniverse.getKelvinHigh());
  byte tEndHi = highByte(dmxUniverse.getKelvinHigh());
  byte data[] = {tStartLow, tStartHi, tEndLow, tEndHi};
  Wire.write(data, 4);
}
