#include <stdint.h>
#include "ps.h"
#include "steple.h"

#define _LOGD_

#define POTPIN 2
#define STEP_A 8
#define STEP_B 9
#define STEP_C 10
#define STEP_D 11

// special inputs
#define FORWARD 0x7FFFFFFF
#define REVERSE 0x80000000
#define CLEAR 0x80000001

ps_t potpin;

uint32_t target = 0;
uint32_t current = target;

/////////////////////////////////////////////////////////////////

long read_input() {
  if (Serial.available()) {
    char buf[8] { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    Serial.readBytes(buf, 8);
    Serial.flush();

    #ifdef _LOGD_
    uint8_t i;
    for (i=0; i<8; i++) {
      Serial.print(buf[i]);
    }
    Serial.println();
    #endif

    long input = atol(buf);

    if (input == 0) {
      switch (buf[0]) {
        case 'f':
          Serial.print("Run forward indefinitely.\n");
          input = FORWARD; // may not be forever, but for all intents and purposes this should suffice
          break;
        case 'r':
          Serial.print("Run backward indefinitely.\n");
          input = REVERSE; // may not be forever, but for all intents and purposes this should suffice
          break;
        case 'c':
          Serial.print("Clearing steps.\n");
          input = CLEAR;
          break;
        default:
          Serial.print("Unrecognized input.\n");
          break;
      }
    }

    #ifdef _LOGD_
    Serial.print("input = ");
    Serial.print((long) input);
    Serial.println();
    #endif

    return input;
  }
  return 0;
}

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);

  potpin.pin = POTPIN;
  potpin.ad = PS_ANALOG;
  potpin.io = PS_INPUT;

  ps_init(&potpin);
  step_init(STEP_A);
  
  Serial.print("Pot-Step Ready.");
  Serial.println();
}

typedef struct InputStorage {
  long steps;
  long start;
  long finish;
} InputStorage;

InputStorage input;

uint32_t previousTarget = 0;
void loop() {
  target = ps_read(&potpin) / 5 * 5; // prevents jitter in steps of 5

//  if (target != previousTarget) {
//    uint32_t expected_delay = 500 + 0.978 * target;
//    Serial.print("target: "); Serial.print(target); Serial.println();
//    Serial.print("expected delay: "); Serial.print(expected_delay); Serial.println();
//  }

  long steps = read_input();
  if (steps == CLEAR) {
    nbclear();
  } else if (steps != 0) {
    input.steps = steps;
    input.start = millis();
    nbseq_init(input.steps);
    if (nbready()) nbseq(target);
  } else if (nbrunning()) {
    nbseq(target);
  } else if (nbdone()) {
    input.finish = millis();
    Serial.print("Completed ");
    Serial.print(input.steps);
    Serial.print(" steps in ");
    Serial.print(input.finish - input.start);
    Serial.print(" ms");
    Serial.println();
    step_clear();
  } else {
    delay(1000);
  }

  previousTarget = target;
}

