//          Project: Lab 4 Part 2
//          Date: Oct 9, 2022
//          Author: Kenneth Ayala

#include <stdint.h>
#include "SysTick.h"
#include "Init_LEDs.h"
#include "Init_Switches.h"
#include "msp432p401r.h"


struct State {
    uint32_t Out;
    uint32_t Time;      //10ms units
    uint32_t Next[8];   //list of the next states
};
typedef const struct State STyp;

#define goN         0
#define waitN       1
#define goW         2
#define waitW       3
#define goWalk      4
#define reqN        5
#define confirmN    6
#define reqW        7
#define confirmW    8
#define waitN2      9
#define waitW2      10
#define slow        11
#define LEDred0     12
#define LEDoff1     13
#define LEDred2     14
#define LEDoff3     15
#define stopWalk    16


STyp FSM[17] = {              //  000,        001,           010,         011,       100,            101,            110,         111
               {0x4C, 300, {      goN,        waitN,         goN,         waitN,     reqN,           reqN,           reqN,        waitN      }},   //Go north
               {0x4A, 50,  {      goW,        goW,           goW,         goW,       goW,            goW,            goW,         goW        }},   //Wait North
               {0x61, 300, {      goW,        goW,           waitW,       waitW,     reqW,           reqW,           reqW,        reqW       }},   //Go West
               {0x51, 50,  {      goN,        goN,           goN,         goN,       goN,            goN,            goN,         goN        }},   //Wait West
               {0x89, 300, {      slow,       slow,          slow,        slow,      goWalk,         slow,           slow,        slow       }},   //Go Walk
               {0x4C, 200, {      goN,        goN,           goN,         goN,       confirmN,       confirmN,       confirmN,    confirmN   }},   //reqN
               {0x4C, 10,  {      waitN2,     waitN2,        waitN2,      waitN2,    waitN2,         waitN2,         waitN2,      waitN2     }},   //confirmN
               {0x61, 200, {      goW,        goW,           goW,         goW,       confirmW,       confirmW,       confirmW,    confirmW   }},   //reqW
               {0x61, 10,  {      waitW2,     waitW2,        waitW2,      waitW2,    waitW2,         waitW2,         waitW2,      waitW2     }},   //confirmW
               {0x4A, 50,  {      goWalk,     goWalk,        goWalk,      goWalk,    goWalk,         goWalk,         goWalk,      goWalk     }},   //Wait N2
               {0x51, 50,  {      goWalk,     goWalk,        goWalk,      goWalk,    goWalk,         goWalk,         goWalk,      goWalk     }},   //Wait W2
               {0x09, 50,  {      LEDred0,    LEDred0,       LEDred0,     LEDred0,   LEDred0,        LEDred0,        LEDred0,     LEDred0    }},   //slow
               {0x49, 50,  {      LEDoff1,    LEDoff1,       LEDoff1,     LEDoff1,   LEDoff1,        LEDoff1,        LEDoff1,     LEDoff1    }},   //LEDred0
               {0x09, 50,  {      LEDred2,    LEDred2,       LEDred2,     LEDred2,   LEDred2,        LEDred2,        LEDred2,     LEDred2    }},   //LEDoff1
               {0x49, 50,  {      LEDoff3,    LEDoff3,       LEDoff3,     LEDoff3,   LEDoff3,        LEDoff3,        LEDoff3,     LEDoff3    }},   //LEDred2
               {0x09, 50,  {      stopWalk,   stopWalk,      stopWalk,    stopWalk,  stopWalk,       stopWalk,       stopWalk,    stopWalk   }},   //LEDoff3
               {0x49, 50,  {      goN,        goW,           goN,         goN,       goN,            goW,            goN,         goN        }},   //StopWalk
};

void main(void){
    uint32_t ind;
    uint32_t Input;         //P5.0-P5.1
    uint32_t Walk_Input;    //P1.1
    //initialize ports and timer
    SysTick_Init();
    Init_LEDs();
    Init_Swithes();

    ind = goN;      //start state
    while(1){
        P4OUT  = (P4OUT&~0x3F)| FSM[ind].Out;
        P2OUT  = (P2OUT&~0x03)| FSM[ind].Out>>6;
        SysTick_Wait10ms(FSM[ind].Time);
        Walk_Input = (~P1IN&0x02);              //read walk sensor and invert bit since neg logic
                                                //shift bit by 1 to have walk sensor bit on bit 2
        Input  = (Walk_Input<<1)|(P5IN&0x03);   //Combine walk and car sensor with or
        ind = FSM[ind].Next[Input];
    }
}
