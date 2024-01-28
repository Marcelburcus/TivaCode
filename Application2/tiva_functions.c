#include "tiva_header_file.h"
#include "uart_header_file.h"


//parameter values :
/*
 * Fn           : enable_PORTF
 * Brief        : enable the PORTF

 */
void enable_PORTF(){
    SYSCTL_RCGCGPIO |=RCGCGPIO_PORTF; //(create a function to enable it)
}

/*
 * Fn           : system_clock_set_up
 * Brief        : choose the sysdiv clock
 * Param1       : one of the optiones: SYS_CLK_1MHZ  SYS_CLK_2MHZ  SYS_CLK_4MHZ  SYS_CLK_8MHZ  SYS_CLK_16MHZ
 * Return       : None
 * Note         : for RCC see page 255
 */
void system_clock_set_up(int megahertz){
    //set USESSYSDIV to enable the clock divider
    SYSCTL_RCC |= SYSCTL_RCC_USESSYSDIV;
    //clear bits 23 to 26 (SYSDIV bits) to avoid overwriting
    SYSCTL_RCC &=~(0xF<<23);

    //set clock divisor
    switch(megahertz){

    case 1:
        SYSCTL_RCC |= SYSCTL_RCC_SYSDIV_16;
        break;
    case 2:
        SYSCTL_RCC |= SYSCTL_RCC_SYSDIV_8;
           break;

    case 4:
        SYSCTL_RCC |= SYSCTL_RCC_SYSDIV_4;
        break;

     case 8:
         SYSCTL_RCC |= SYSCTL_RCC_SYSDIV_2;
        break;

     case 16:
         SYSCTL_RCC |= SYSCTL_RCC_SYSDIV_1;
        break;
    }
}

/*
 * Fn           : uart_set_up
 * Brief        : configuring the corresponding UART
 * Param1       : uart - Pointer to a structure of type UART_Param_t that contains the UART parameter
 */
void uart_set_up( UART_Param_t *uart){
    switch(uart->instance){
     case UART5_INSTANCE :
        //step1: enable the uart clock
        RCGCUART |= UART_MODULE5;
        //step2: wait till it clock is ready
        while( ( ( RCGCUART&UART_MODULE5 ) >>5 ) == 0 );
        //step 3: disable CTL before any change
        UART5_CTL &=~(1<<0);
        //step 4: set baudrate
        set_baudrate_uart5(uart->baud_rate);
        //step5: Set 8-bit Data Mode in UARTLCRH
        UART5_LCRH |=(3<<5);
        //step6: enable uart in uartCTL
        enable_UART5_CTL();
        //step 7: enable Tx / Rx or both
        if(uart->tx_rx == TX_RX_BOTH){
            enable_TX_UART5_CTL();
            enable_RX_UART5_CTL();
        }else if(uart->tx_rx == TX_ONLY){
            enable_TX_UART5_CTL();
            disable_RX_UART5_CTL();
        }else{
            disable_TX_UART5_CTL();
            enable_RX_UART5_CTL();
        }
       break;
     case UART0_INSTANCE :
            //step1: enable the uart clock
            RCGCUART |= UART_MODULE0;
            //step2: wait till it clock is ready
            while( ( ( RCGCUART&UART_MODULE0 ) >>0 ) == 0 );
            //step 3: disable CTL before any change
            UART0_CTL &=~(1<<0);
            //step 4: set baudrate
            set_baudrate_uart0(uart->baud_rate);
            //step5: Set 8-bit Data Mode in UARTLCRH
            UART0_LCRH |=(3<<5);
            //step6: enable uart in uartCTL
            enable_UART0_CTL();
            //step 7: enable Tx / Rx or both
            if(uart->tx_rx == TX_RX_BOTH){
                       enable_TX_UART0_CTL();
                       enable_RX_UART0_CTL();
                   }else if(uart->tx_rx == TX_ONLY){
                       enable_TX_UART0_CTL();
                       disable_RX_UART0_CTL();
                   }else{
                       disable_TX_UART0_CTL();
                       enable_RX_UART0_CTL();
                   }
           break;

    }
}

/*
 * Fn           : send_data_uart
 * Brief        : when the flag is 0, pass the data
 * Param1       : word
 * Param2       : word_length
 */
void send_data_uart5(char* word, int word_length){
    int i ;
    for(i=0; i<word_length; i++){
        while( ( (UART5_FR & (1<<7))>>7 == 0   ) );
                // UART5_DR = text[letter];
                  UART5_DR = word[i];
                  //or *word ++;

    }

}

/*
 * Fn           : send_data_uart
 * Brief        : Sends UART data
 * Param1       : instance - The UART instance on which the data should be sent (UART0_INSTANCE or UART5_INSTANCE)
 * Param2       : word - Pointer to the data string to be sent
 * Param3       : word_length - The length of the data string
 */
void send_data_uart(char instance, char* word, int word_length){
    int i;
    switch(instance){
     case UART0_INSTANCE:
         for(i=0; i<word_length; i++){
                 while( ( (UART0_FR & (1<<7))>>7 == 0   ) );
                         // UART5_DR = text[letter];
                           UART0_DR = word[i];
                           //or *word ++;

             }
         break;
     case UART5_INSTANCE:
          for(i=0; i<word_length; i++){
                while( ( (UART5_FR & (1<<7))>>7 == 0   ) );
                         // UART5_DR = text[letter];
                         UART5_DR = word[i];
                         //or *word ++;

                  }
              break;
    }

  }

/*
 * Fn           : set_baudrate_uart5
 * Brief        : Configures the baud rate for UART5
 * Param1       : baud_rate - The desired baud rate for UART communication
 * Return       : None
 * Note         : This function calculates and sets the appropriate values for UART5's IBRD and FBRD registers
 */

void set_baudrate_uart5(uint32_t baud_rate){
    uint32_t int_baudrate = 0;
    uint32_t fractional_baudrate = 0;

    // Calculate the integer part of the baud rate divisor and set UART5_IBRD
    int_baudrate = 16000000 / (16 * baud_rate);
    UART5_IBRD = int_baudrate;

    // Calculate the fractional part of the baud rate divisor and set UART5_FBRD
    uint32_t BRD = (160000000000) / (16 * baud_rate);
    BRD = BRD % 10000;
    float temp = (float)BRD / 10000;
    fractional_baudrate = temp * 64 + 0.5;
    UART5_FBRD = fractional_baudrate;
}

/*
 * Fn           : enable_UART5_CTL
 * Brief        : Enables UART5 by setting the UART5_CTL register
 * Note         : This function sets the UARTEN (UART Enable) bit in the UART5_CTL register.
 */

void enable_UART5_CTL(){
    UART5_CTL |= (1 << 0);
}

/*
 * Fn           : disable_UART5_CTL
 * Brief        : Disables UART5 by clearing the UART5_CTL register
 * Note         : This function clears the UARTEN (UART Enable) bit in the UART5_CTL register.
 */

void disable_UART5_CTL(){
    UART5_CTL &= ~(1 << 0);
}

/*
 * Fn           : uart_gpio_Configuration
 * Brief        : Configures GPIO pins for UART functionality
 * Param1       : configuration - Pointer to a structure (UART_GPIO_t) containing configuration parameters
 * Return       : None
 * Note         : The function configures the GPIO pins to enable UART functionality based on the provided configuration.
 *                It enables the clock for the specified port, sets the pin's alternate function, and configures the pin's
 *                functionality according to the UART standard.
 */

void uart_gpio_Configuration(UART_GPIO_t *configuration){
    // Enable clock for the specified GPIO port
    gpio_rcgcgpio_enable_disable(configuration->port, ENABLE);

    if(configuration->port == PORTE){
        // Enable alternate function for the specified pin
        PORTE_GPIOAFSEL |= (1 << configuration->pin_number);
        // Enable digital functionality for the specified pin
        PORTE_GPIODEN   |= (1 << configuration->pin_number);
        // Configure the pin's functionality using the GPIOPCTL register
        PORTE_GPIOPCTL  |= (configuration->functionality << (4 * configuration->pin_number));
    } else if(configuration->port == PORTA) {
        PORTA_GPIOAFSEL |= (1 << configuration->pin_number);
        PORTA_GPIODEN   |= (1 << configuration->pin_number);
        PORTA_GPIOPCTL  |= (configuration->functionality << (4 * configuration->pin_number));
    } else if(configuration->port == PORTC){
        PORTC_GPIOAFSEL |= (1 << configuration->pin_number);
        PORTC_GPIODEN   |= (1 << configuration->pin_number);
        PORTC_GPIOPCTL  |= (configuration->functionality << (4 * configuration->pin_number));
    }

}

/*
 * Fn           : timer1A_set_up_with_interrupt_enable
 * Brief        : Configures and enables Timer1A with interrupt support
 * Param        : None
 * Return       : None
 * Note         : This function sets up Timer1A in one-shot mode and enables its interrupt.
 *                The interrupt is configured to be handled by the NVIC (Nested Vectored Interrupt Controller).
 */
void timer1A_set_up_with_interrupt_enable(){
    // Enable and provide a clock to 16/32 bit general-purpose timer module 1 in run mode
    SYSCTL_RCGCTIMER |= (1 << RCGCTIMER_R1);

    // Step 1: Before making any changes, clear the TAEN bit
    // Only when CTL is cleared, changes in TAMR can be made
    GPTMCTL1 &= ~(1 << GPTMCTL1_TAEN);

    // Use a 32-bit timer by writing 0 (for the timer with higher value)
    GPTMCFG1 = 0;

    // Set Timer1A in one-shot mode
    GPTMTAMR1 |= (1 << GPTMTAMR1_TAMR); // 0x1 for one-shot mode

    // Enable controller-level interrupts for Timer1A
    NVIC_EN0 |= NVIC_EN0_TIMER1A;

    // Enable controller level interrupt for Timer1A
    GPTMIMR1 |= GPTMIMR1_TATOIM;
}

/*
 * Fn           : timer0A_delay_in_seconds
 * Brief        : Delays program execution for the specified number of seconds using Timer0A
 * Param        : how_many_seconds - The duration of the delay in seconds
 * Return       : None
 * Note         : This function configures Timer0A to generate a delay and waits until the delay is complete.
 *                The delay duration is determined by multiplying the system clock frequency (16,000,000 Hz) by
 *                the number of seconds specified. The function then starts the timer, waits for it to finish counting,
 *                and finally stops the timer.
 */

void timer0A_delay_in_seconds(int how_many_seconds)
{
    // Set the initial count value for Timer0A based on the desired delay in seconds
    GPTMTAILR = 16000000 * how_many_seconds;

    // Start Timer0A
    timer0A_start();

    // Wait until the timer finishes counting
    while((GPTMRIS & (1 << 0)) == 0);

    // Stop Timer0A
    timer0A_stop();

    // Clear the interrupt status to prepare for future use
    timer0A_clear_ris();
}

/*
 * Fn           : turn_on_off_LED_number
 * Brief        : Turns an LED on or off based on color and state
 * Param1       : colour - The color of the LED (RED_LED_PIN, GREEN_LED_PIN, or BLUE_LED_PIN)
 * Param2       : on_off - The state to set for the LED (1 for on, 0 for off)
 * Return       : None
 * Note         : This function controls the state (on or off) of a specified LED color on the PORTF GPIO.
 *                The GPIO data register (PORTF_GPIODATA) is manipulated to turn the LED on or off.
 */
void turn_on_off_LED_number(int colour, int on_off){
    if(on_off == 1){
        // Turn the LED on
        if(colour == RED_LED_PIN){
            PORTF_GPIODATA |= RED_LED_PIN;
        } else if(colour == GREEN_LED_PIN){
            PORTF_GPIODATA |= GREEN_LED_PIN;
        } else if(colour == BLUE_LED_PIN){
            PORTF_GPIODATA |= BLUE_LED_PIN;
        }
    } else if(on_off == 0){
        // Turn the LED off
        if(colour == RED_LED_PIN){
            PORTF_GPIODATA &= ~RED_LED_PIN;
        } else if(colour == GREEN_LED_PIN){
            PORTF_GPIODATA &= ~GREEN_LED_PIN;
        } else if(colour == BLUE_LED_PIN){
            PORTF_GPIODATA &= ~BLUE_LED_PIN;
        }
    }
}

/*
 * Fn           : timer0A_start
 * Brief        : Starts Timer0A by enabling the timer and initiating counting
 * Param        : None
 * Return       : None
 * Note         : This function sets the T(A)EN bit in the GPTMCTL register to enable Timer0A
 *                and initiate counting. It effectively starts the timer.
 */

void timer0A_start()
{
    GPTMCTL |= (1 << GPTMCTL_TAEN); // Step 7
}

/*
 * Fn           : button_SW1_Configuration
 * Brief        : configuring the corresponding
 *                registers for the button 1
 */

void button_SW1_Configuration(){
    //enabling write access to CR
    PORTF_GPIOLOCK = 0x4C4F434B;
    //button is input-> value is 0
    PORTF_GPIODIR  &=~SW1_PIN;
    //access to PUR by enabling CR
    PORTF_GPIOCR |=SW1_PIN;
    //enable digital function for button
    PORTF_GPIODEN |=SW1_PIN;
    //enable pull up regiser for SW2
    PORTF_GPIOPUR |=SW1_PIN;
}
