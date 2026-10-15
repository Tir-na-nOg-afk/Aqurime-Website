#pragma once
#include <Arduino.h>
#define MAX_STRENGTH_LIMIT 100

enum Phase { FLOOD_TIDE, HIGH_SLACK, EBB_TIDE, LOW_SLACK };

extern float maxStrength;
extern float phaseTimes[4];
extern Phase phase;
extern float currentPhaseTime;
extern float pump1Strength, pump2Strength;
