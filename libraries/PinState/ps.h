#ifndef _PIN_STATE_H_
#define _PIN_STATE_H_

#include <stdint.h>

#define PS_ANALOG  0
#define PS_DIGITAL 1
#define PS_INPUT   0
#define PS_OUTPUT  1
#define PS_LOW     0
#define PS_HIGH    1

typedef struct PinState {
  uint8_t pin;
  uint8_t val;
  uint8_t ad; // can combine with 'io' into a single bitmask to save space if needed
  uint8_t io;
} ps_t, *ps_p;

void ps_init(ps_p pin);

void ps_write(ps_p pin);

uint32_t ps_read(ps_p pin);

#endif /* _PIN_STATE_H_ */
