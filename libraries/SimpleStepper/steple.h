#ifndef _STEPLE_H_
#define _STEPLE_H_

#include <stdint.h>
#include "ps.h"

typedef struct SimpleStepper { // so far unused
    uint8_t current_step;
    ps_p pin_array;
} steple_t, *steple_p;

void step_init(uint8_t starting_pin);

void fseq(int64_t steps, uint32_t duration_ms);

void rseq(int32_t steps, uint32_t duration_ms);

void nbseq_init(int64_t steps);

void nbseq(uint32_t delay_ms);

bool nbready();

bool nbrunning();

bool nbdone();

bool nbidle();

void nbclear();

void step_clear();

#endif /* _STEPLE_H_ */
