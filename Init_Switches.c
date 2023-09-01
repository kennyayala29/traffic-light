#include <stdint.h>
#include "msp432p401r.h"

void Init_Swithes(void){
//Configuring Inputs P5.0-P5.2
//Car Sensors
   P5SEL0 &= ~0x03;
   P5SEL1 &= ~0x03;
   P5DIR  &= ~0x03;

//Configure Input P1.1
// Walk Sensor
   P1SEL0 &= ~0x02;
   P1SEL1 &= ~0x02;
   P1DIR  &= ~0x02;
   P1REN  |=  0x02;
   P1OUT  |=  0x02;
}
