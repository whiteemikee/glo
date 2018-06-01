#include <arduino.h>
#include "ps.h"

void ps_init(ps_p self) {
  if (self->io == PS_INPUT) {
    pinMode(self->pin, PS_INPUT);
  } else if (self->io == PS_OUTPUT) {
    pinMode(self->pin, PS_OUTPUT);
  } else {
    // bad pin_state
  }
}

void ps_write(ps_p self) {
  if (self->io == PS_OUTPUT) {
    if (self->ad == PS_ANALOG) {
      analogWrite(self->pin, self->val);
    } else {
      digitalWrite(self->pin, self->val);
    }
  }
}

uint32_t ps_read(ps_p self) {
  if (self->io == PS_INPUT) {
    if (self->ad == PS_ANALOG) {
      return analogRead(self->pin);
    } else {
      return digitalRead(self->pin);
    }
  }
}

