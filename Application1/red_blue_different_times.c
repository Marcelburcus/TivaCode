#include <tiva_header_file.h>

/*
 * Using Interrupts and Timers:
 * Blink the RED LED at an interval of 1 second continuously.
 * When a loop of blinking RED 10 times is completed,
 * blink the BLUE LED 2 times at an interval of 1.5 seconds.
 */

int counter_red =0;
int counter_blue =0;


void main()
{

       enable_PORTF();

       set_up_LEDs(RED);
       set_up_LEDs(BLUE);

       //we use for RED colour timer0A(here set-up & load)
       timer0A_set_up_with_interrupt_enable();
       load_timer0A_GPTMTAILR(16119000);//1 second

       //we use for Blue colour timer1A(here set-up & load)
       timer1A_set_up_with_interrupt_enable();
       load_timer1A_GPTMTAILR1(24178500);//=1.5 seconds

       //we start counting only timer0A
      timer0A_start();
      turn_on_off_LED(RED, ON);

       while(1){
           //do something
       }
}

/* Interrupt Service Routine for Timer0A */
void timer0A_ISR() {
    /* Increments counter for RED LED, toggles RED LED state,
     * and handles the transition to BLUE LED blinking.
     * Clears Timer0A interrupt flag and restarts Timer0A.
     */

    counter_red++;
    PORTF_GPIODATA ^= RED_LED_PIN;

    if (counter_red == 20) {
        turn_on_off_LED(BLUE, ON);
        timer1A_start();
        counter_red = 0;
    }

    timer0A_clear_ris();
    timer0A_start();
}

/* Interrupt Service Routine for Timer1A */
void timer1A_ISR() {
    /* Increments counter for BLUE LED and toggles BLUE LED state.
     * Manages the transition to turning off BLUE LED and resetting the counter.
     * Clears Timer1A interrupt flag and restarts Timer1A if needed.
     */

    counter_blue++;

    if (counter_blue < 4) {
        PORTF_GPIODATA ^= BLUE_LED_PIN;
        timer1A_clear_ris();
        timer1A_start();
    } else if (counter_blue == 4) {
        turn_on_off_LED(BLUE, OFF);
        timer1A_stop();
        timer1A_clear_ris();
        counter_blue = 0;
    }
}
