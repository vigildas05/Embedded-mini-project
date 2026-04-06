#include <reg51.h>

// -------- OUTPUTS --------
sbit LED_FULL   = P2^0;
sbit LED_MID    = P2^1;
sbit LED_HIGH   = P2^2;
sbit LED_LOW    = P2^3;
sbit BUZZER     = P2^4;

// -------- INPUTS --------
sbit LEVEL_EMPTY = P3^2;   // INT0 (LOW water - from comparator)
sbit LEVEL_FULL  = P3^3;   // INT1 (FULL water - from comparator)
sbit LEVEL_MID   = P1^0;   // optional mid switch
sbit LEVEL_HIGH  = P1^1;   // optional high switch

// -------- FLAGS --------
bit low_flag   = 0;
bit full_flag  = 0;


// -------- INTERRUPTS --------
void low_ISR(void)  interrupt 0 { low_flag  = 1; }
void full_ISR(void) interrupt 2 { full_flag = 1; }


// -------- DELAY --------
void delay(unsigned int msec) {
    unsigned int i, j;
    for(i = 0; i < msec; i++)
        for(j = 0; j < 120; j++);
}


// -------- MAIN --------
void main(void) {

    IT0 = 1;
    IT1 = 1;
    EX0 = 1;
    EX1 = 1;
    EA  = 1;

    P1 = 0xFF;   // inputs
    P2 = 0x00;   // outputs

    while(1) {

        // -------- EMPTY LEVEL --------
        if(LEVEL_EMPTY == 0) {   // active LOW from comparator
            LED_LOW = 1;
            BUZZER  = 1;
        } else {
            LED_LOW = 0;
            BUZZER  = 0;
        }

        // -------- FULL LEVEL --------
        if(LEVEL_FULL == 0) {
            LED_FULL = 1;
        } else {
            LED_FULL = 0;
        }

        // -------- MID LEVEL (Polling) --------
        if(LEVEL_MID == 1 && LEVEL_HIGH == 0)
            LED_MID = 1;
        else
            LED_MID = 0;

        // -------- HIGH LEVEL (Polling) --------
        if(LEVEL_HIGH == 1 && LEVEL_EMPTY == 1)
            LED_HIGH = 1;
        else
            LED_HIGH = 0;
    }
}