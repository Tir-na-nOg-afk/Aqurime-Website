#include <cmath>
#include <aquaServer.h>
#include <DisplayHandler.h>

enum Phase {
  FLOOD_TIDE,
  HIGH_SLACK,
  EBB_TIDE,
  LOW_SLACK
};

// Input Variables
// This time must be stored at seconds. Conversions must happen before it comes here
float maxStrength = 100.0f;
float floodTime = 5.0f;
float highSlackTime = 5.0f;
float ebbTime = 5.0f;
float lowSlackTime = 5.0f;

float updateDelay = 100.0f;  //how long between every single update. This is counted in milliseconds

//Both pumps work contra to each other. Both start at low slack, and work there way through the cycle
enum Phase phase = FLOOD_TIDE;
float pump1Strength = 0.0;
float pump2Strength = 0.0;
//milliseconds
float currentPhaseTime = 0.0f;

long lastUpdateTime = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  displaySetup();
  startServer();
}

void loop() {
  if ((lastUpdateTime + ((long)updateDelay)) < millis()) {
    calculatePumps();
    handleServer();
    displayLoop();
    lastUpdateTime = millis();
    printPumpInfo();
  }
  // put your main code here, to run repeatedly:
}

void calculatePumps() {
  currentPhaseTime += updateDelay;

  //Assing the timings to an array, in the same order that they appear in the Phase enum
  float timeArray[4] = { floodTime, highSlackTime, ebbTime, lowSlackTime };

  //If the current phase time is up, we move to a new phase, the remainder ensures we loop from 0-1-2-3-0, where each integer is a phase
  if (currentPhaseTime >= timeArray[phase] * 1000.0f) {
    currentPhaseTime = 0.0f;
    phase = (enum Phase)((phase + 1) % 4);
  }

  switch (phase) {
    case LOW_SLACK:
      pump1Strength = 0.0;
      pump2Strength = 0.0;
      break;
    case HIGH_SLACK:
      pump1Strength = 0.0;
      pump2Strength = 0.0;
      break;
    //formula is A*sin(wx) where A is amplitutde and w is radial frequency given by 2*pi*f or (2*pi)/t
    //the flood and ebb time are multiplied by 1000 to convert from seconds to milliseconds.
    //In this case we have halved w as we want one half of the sinuisodal signal, not both halves. As it doesnt make much sense to send 100% and -100% strength as our reading.
    case FLOOD_TIDE:
      pump1Strength = maxStrength * sinf((M_PI * currentPhaseTime) / (floodTime * 1000.0f));
      pump2Strength = 0;
      break;
    case EBB_TIDE:
      pump1Strength = 0;
      pump2Strength = maxStrength * sinf((M_PI * currentPhaseTime) / (ebbTime * 1000.0f));
      break;
  }
}

void printPumpInfo() {
  Serial.print(phase);
  Serial.print(", ");
  Serial.print(currentPhaseTime);
  Serial.print(", ");
  Serial.print(pump1Strength);
  Serial.print(", ");
  Serial.print(pump2Strength);
  Serial.print("\n");
}
