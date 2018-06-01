#include <stdint.h>
#include <arduino.h>
#include "ps.h"
#include "steple.h"

#define NUM_PINS 4

// sequence steps
// use the lowest 4 bits to define the step
//     A,B,C,D,E,F,G,H
// 0 - 1,1,0,0,0,0,0,1
// 1 - 0,1,1,1,0,0,0,0
// 2 - 0,0,0,1,1,1,0,0
// 3 - 0,0,0,0,0,1,1,1
//#define A 0
//#define B 1
//#define C 2
//#define D 3
//#define E 4
//#define F 5
//#define G 6
//#define H 7
#define NUM_STEPS 8

const uint8_t seq_step[NUM_STEPS] = {
  0x01,
  0x03,
  0x02,
  0x06,
  0x04,
  0x0C,
  0x08,
  0x09
};

uint8_t STARTING_PIN;

uint8_t current_step = 0;

ps_t pins[NUM_PINS];

void step_init(uint8_t starting_pin) {
    STARTING_PIN = starting_pin;
    uint8_t i;
    for (i=0; i<NUM_PINS; i++) {
        pins[i].pin = STARTING_PIN + i;
        pins[i].val = PS_LOW;
        pins[i].ad = PS_DIGITAL;
        pins[i].io = PS_OUTPUT;
        
        ps_init(&pins[i]);
    }
}

void set_sequence_step(uint8_t sequence_step) {
    uint8_t i;
    for (i=0; i<NUM_PINS; i++) {
        if (seq_step[sequence_step] & (1 << i)) pins[i].val = PS_HIGH;
        else pins[i].val = PS_LOW;
        ps_write(&pins[i]);
    }
}

uint64_t accelerationDelay(uint64_t duration_ms, uint64_t steps, uint64_t current_step) {
    // min = 0 us
    // max = 1000 us
    // x = 1000 - [0-1000]
    uint64_t delay_us = (1000 - 0.978 * duration_ms);
    return delay_us;
}

#define NB_READY 0
#define NB_RUNNING 1
#define NB_DONE 2
#define NB_IDLE 3

int8_t nb_dir = 1;
uint32_t nb_total_steps = 0;
uint32_t nb_current_step = 0;
uint8_t nb_state = NB_IDLE; // 0 = ready; 1 = running; 2 = done; 3 = idle

void nbseq_init(int64_t steps) {
    if (nb_state == NB_IDLE) {
        nb_dir = (steps < 0 ? -1 : 1);
        nb_total_steps = steps;// * nb_dir;
        nb_current_step = 0;
        nb_state = NB_READY;
    }
}

void nbseq(uint32_t delay_ms) {
    switch (nb_state) {
        case NB_READY:
            nb_state = NB_RUNNING;
            break;
        case NB_RUNNING:
            if (nb_total_steps - nb_current_step != 0) {
                delayMicroseconds(
                    accelerationDelay(delay_ms, nb_total_steps, nb_current_step)
                );
                set_sequence_step(current_step);
                current_step = (uint8_t)(nb_current_step + nb_dir) % NUM_STEPS;
                nb_current_step += nb_dir;
            } else {
                nb_state = NB_DONE;
            }
        default:
            break;
    }
}

bool nbready() {
    return nb_state == NB_READY;
}

bool nbrunning() {
    return nb_state == NB_RUNNING;
}

bool nbdone() {
    if (nb_state == NB_DONE) {
        nb_state = NB_IDLE;
        return true;
    } else return false;
}

bool nbidle() {
    return nb_state == NB_IDLE;
}

void nbclear() {
    nb_total_steps = 0;
    nb_current_step = 0;
    nb_dir = 1;
    nb_state = NB_IDLE;
    step_clear();
}

void fseq(int64_t steps, uint32_t duration_ms) {
    // define direction
    int8_t dir = (steps < 0 ? -1 : 1);
    
    uint64_t i;
    for (i=0; i<steps*dir; i++) {
        // calculate the delay between each step to achieve
        // the desired duration timing.
        delayMicroseconds(accelerationDelay(duration_ms, steps, i));
        
        set_sequence_step(current_step);
        
        current_step = (uint8_t)(current_step + dir) % NUM_STEPS;
    }
}

/**
 * Reverse Sequence (CCW)
 */
void rseq(int32_t steps, uint32_t duration_ms) {
    fseq(-steps, duration_ms);
}

void step_clear() {
    uint8_t i;
    for (i=0; i<NUM_PINS; i++) {
        pins[i].val = LOW;
        ps_write(&pins[i]);
    }
}
