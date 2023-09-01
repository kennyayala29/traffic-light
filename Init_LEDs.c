#include <stdint.h>
#include "msp432p401r.h"

void Init_LEDs(void){
//Traffic Light LED outputs P4.0-P4.5
   P4SEL0 &= ~0x3F;
   P4SEL1 &= ~0x3F;
   P4DIR  |=  0x3F;

//Walk LED output P2.0-P2.1
   P2SEL0 &= ~0x03;
   P2SEL1 &= ~0x03;
   P2DIR  |=  0x03;
}
