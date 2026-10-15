#include <cmath>
#include <shared.h>
#include <aquaServer.h>
#include <DisplayHandler.h>

// Input Variables
// This time must be stored at seconds. Conversions must happen before it comes here
float maxStrength = 100.0f;
float phaseTimes [4] = {5.0f, 5.0f, 5.0f, 5.0f};
Phase phase = FLOOD_TIDE;
float pump1Strength = 0.0f, pump2Strength = 0.0f;
float currentPhaseTime = 0.0f;
float updateDelay = 100.0f;
unsigned long lastUpdateTime = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  displaySetup();
  startServer();
}

void loop() {
  handleServer();
  unsigned long now = millis();
  if (now - lastUpdateTime >= (unsigned long)updateDelay) {
    float elapsed = (float)(now - lastUpdateTime);
    lastUpdateTime = now;
    calculatePumps(elapsed);
    displayLoop();
    printPumpInfo();
  }
  // put your main code here, to run repeatedly:
}

void calculatePumps(float elapsedMS) {
  currentPhaseTime += elapsedMS;
  if (currentPhaseTime >= phaseTimes[phase] * 1000.0f){
    currentPhaseTime = 0.0f;
    phase = (Phase)((phase + 1) % 4);
  }
  switch (phase) {
    case FLOOD_TIDE:
      pump1Strength = maxStrength * sinf((M_PI * currentPhaseTime) / (phaseTimes[FLOOD_TIDE] * 1000.0f));
      pump2Strength = 0;
      break;  
    case EBB_TIDE:
      pump1Strength = 0;
      pump2Strength = maxStrength * sinf((M_PI * currentPhaseTime) / (phaseTimes[EBB_TIDE] * 1000.0f));;
      break;
    default:
      pump1Strength = 0;
      pump2Strength = 0;
      break;
    //formula is A*sin(wx) where A is amplitutde and w is radial frequency given by 2*pi*f or (2*pi)/t
    //the flood and ebb time are multiplied by 1000 to convert from seconds to milliseconds.
    //In this case we have halved w as we want one half of the sinuisodal signal, not both halves. As it doesnt make much sense to send 100% and -100% strength as our reading.
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
