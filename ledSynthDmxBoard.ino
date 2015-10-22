/*
    This file is part of LEDsynth
    LEDsynth - a dmx controller for control praradigm experiments with LED fixtures.
    Copyright (C) 2015  Ole Kristensen

    LEDsynth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LEDsynth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LEDsynth.  If not, see <http://www.gnu.org/licenses/>.

    ole@kristensen.name
    olek@itu.dk
*/

#include "utils.h"
#include <Conceptinetics.h>
#include "DmxUniverse.h"
#include <Wire.h>
#include "Biquad.h"

#define DMX_MASTER_CHANNELS   512
#define RXEN_PIN                2
#define DMXEN_PIN               7

DMX_FrameBuffer dmxFrameBuffer( DMX_MASTER_CHANNELS + 1 );
DMX_Master      dmx_master ( dmxFrameBuffer, RXEN_PIN );
DmxUniverse     dmxUniverse( &dmxFrameBuffer );

boolean shieldEnabled = false;

const int break_usec = 200;

const int wireTimeoutMillis = 10000;
unsigned long lastWireReceived = 0;

unsigned int intensityPromille = 0;
unsigned int temperatureKelvin = 2700;

float intensityPromilleOutputFiltered = intensityPromille;
float temperatureKelvinOutputFiltered = temperatureKelvin;

Biquad *intensityFilter = new Biquad(bq_type_lowpass, 1.5 / 20.0,  0.7071, 0.0);
Biquad *temperatureFilter = new Biquad(bq_type_lowpass, 1.5 / 20.0,  0.7071, 0.0);

// the setup routine runs once when you press reset:
void setup() {

  digitalWrite(DMXEN_PIN, LOW);
  pinMode (DMXEN_PIN, INPUT);

  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // WIRE
  Wire.setClock(400000L);
  //Wire.setClock(400);
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  Wire.begin(100);              // join i2c bus with address #100 0x64

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

  // Enable DMX master interface and start transmitting
  dmx_master.enable ();

  // This will turn the DMX Master into manual break mode
  // After doing this you have to check wheter a break is
  // expected and then invoke a Break to continue the next
  // frame to be sent.
  dmx_master.setManualBreakMode ();

  static int counter;

  // Set channel 1 - 50 @ 50%
  dmx_master.setChannelRange ( 1, DMX_MASTER_CHANNELS, 0 );

}

void loop()
{

  if (millis() > 1000 && !shieldEnabled) {
    // enable shield after 1 second
    pinMode (DMXEN_PIN, OUTPUT);
    digitalWrite (DMXEN_PIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    shieldEnabled = true;
  }

  // Check if the DMX master is waiting for a break
  // to happen
  if ( dmx_master.waitingBreak () )
  {

    // We do the wire stuff in the break to avoid dmx timing errors.
    // Make sure we read the latest 4 bytes.

    int numberCommands = Wire.available();

    if (numberCommands % 4 == 0 && numberCommands > 4) {
      for (int i = 0; i < (numberCommands - 4) / 4; i++) {
        Wire.read();
        Wire.read();
        Wire.read();
        Wire.read();
      }
    }

    if (Wire.available() >= 4) {
      byte iLow = Wire.read();
      byte iHigh = Wire.read();
      byte tLow = Wire.read();
      byte tHigh = Wire.read();
      intensityPromille = word(iHigh, iLow);
      temperatureKelvin = word(tHigh, tLow);
      lastWireReceived = millis();
    }

    // Loss of Wire connectivity shows as sinusoidal fades
    if (lastWireReceived + wireTimeoutMillis  < millis()) {
      dmx_master.setChannelRange ( 1, DMX_MASTER_CHANNELS, round((0.5 + (sin(millis() * 0.005) / 2.0)) * 255));
    }

    intensityPromilleOutputFiltered = intensityFilter->process(intensityPromille);
    temperatureKelvinOutputFiltered = temperatureFilter->process(temperatureKelvin);

    dmxUniverse.setIntensity(constrain(intensityPromilleOutputFiltered / 1000.0, 0.0, 1.0));
    dmxUniverse.setTemperatureKelvin(round(temperatureKelvinOutputFiltered));

    dmxUniverse.update();

    // Invoke the breakAndContinue to start generating
    // the break and then automaticly continue sending the
    // next frame.
    // Your application will block for a period
    // length of a break and mark after break
    dmx_master.breakAndContinue ( break_usec );
  }

  // TODO: Do your other operations part of your
  // program here...

}

void receiveEvent(int howMany) {
  // To avoid timing errors, this is empty and the reading is done in the loop between DMX frames.
}

void requestEvent()
{
  byte tStartLow = lowByte(dmxUniverse.getKelvinLow());
  byte tStartHi = highByte(dmxUniverse.getKelvinLow());
  byte tEndLow = lowByte(dmxUniverse.getKelvinHigh());
  byte tEndHi = highByte(dmxUniverse.getKelvinHigh());
  byte data[] = {tStartLow, tStartHi, tEndLow, tEndHi};
  lastWireReceived = millis();
  Wire.write(data, 4);
}
