#include <lpc213x.h>

// Outputs
#define BUZZER (1<<15)

// LED indicators (P0.16–P0.19)
#define LED_EMPTY  (1<<16)
#define LED_LOW    (1<<17)
#define LED_MEDIUM (1<<18)
#define LED_FULL   (1<<19)

// Inputs (switches)
#define EMPTY  0x01
#define LOW    0x02
#define MEDIUM 0x04
#define FULL   0x08

void delay(unsigned int count) {
    unsigned int i,j;
    for(i=0;i<count;i++)
        for(j=0;j<3000;j++);
}

unsigned int read_level() {
    return IO0PIN & 0x0F;
}

void buzzer_on()  { IO0SET = BUZZER; }
void buzzer_off() { IO0CLR = BUZZER; }

int main() {
    unsigned int level;

    /* FIX: Setting IO0DIR to 0x000F8FF0 
       Bit 15 (Buzzer) is now '1' (Output)
       Bits 16-19 (LEDs) remain '1' (Output)
       Bits 0-3 remain '0' (Input)
    */
    IO0DIR = 0x000F8FF0; 
    
    buzzer_off();

    while(1) {
        level = read_level();

        // Turn OFF all LEDs first
        IO0CLR = LED_EMPTY | LED_LOW | LED_MEDIUM | LED_FULL;

        if(level & FULL) {
            IO0SET = LED_FULL;
            buzzer_off();
        }
        else if(level & MEDIUM) {
            IO0SET = LED_MEDIUM;
            buzzer_off();
        }
        else if(level & LOW) {
            IO0SET = LED_LOW;
            buzzer_off();
        }
        else if(level & EMPTY) {
            IO0SET = LED_EMPTY;
            buzzer_on(); // Buzzer turns on here when Empty LED is on
        }
        else {
            buzzer_off();
        }

        delay(10);
    }
}