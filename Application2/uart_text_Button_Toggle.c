#include <stdint.h>
#include "tiva_header_file.h"
#include "uart_header_file.h"
#include <string.h>

/*
 * Program Description:
 * This program utilizes the two buttons on the Tiva board.
 * When button1 is pressed, a message is transmitted via UART: 'you pressed button1, toggling 1 second.'
 * The same behavior applies to button2.
 * The transmitted message can be observed through a Logic Analyzer on another device
 * or via the Bluetooth module HC-05 connected to a smartphone.
 * The delay function employs two nested for loops. Given that the Tiva runs at 16MHz,
 * the inner loop iterates 2550 times. Setting the delay argument to 1000 achieves a delay of 1 second.
 */

int delay_duration = 1000;
char text1_to_send[] = "you pressed button 1, toggling = 1sec";
char text2_to_send[] = "you pressed button 2, toggling = 2sec";



void main()
{
    //configuration PortF and LEDs
    enable_PORTF();
    set_up_LEDs(RED);

    //button SW2 and SW1 pull up  and interrupt configuration
       button_SW2_Configuration();
       interrupt_configuration_button_SW2();
       button_SW1_Configuration();
       interrupt_configuration_button_SW1();

       //object fot RX
        UART_GPIO_t uart_gpio_config_rx;
        uart_gpio_config_rx.port = PORTE;
        uart_gpio_config_rx.pin_number = 4;
        uart_gpio_config_rx.functionality = UART_FUNCTIONALITY;

          //object for TX
       UART_GPIO_t uart_gpio_config_tx;
       uart_gpio_config_tx.port = PORTE;
       uart_gpio_config_tx.pin_number = 5;
       uart_gpio_config_tx.functionality = UART_FUNCTIONALITY;

          UART_Param_t UART5_struct;
          UART5_struct.baud_rate = 9600;
          UART5_struct.instance = UART5_INSTANCE;
          UART5_struct.tx_rx = TX_RX_BOTH;

          //Uart5 GPIO configuration:
                 //Rx for 4, Tx for 5
                 uart_gpio_Configuration(&uart_gpio_config_rx );
                 uart_gpio_Configuration(&uart_gpio_config_tx );
                 //uart Set.up
                 uart_set_up(&UART5_struct);
                 //this will happen in button interrupt
                // send_data_uart(UART5_INSTANCE, text_to_send, text_length);

       while(1){
               turn_on_off_LED_number(RED_LED_PIN, ON);
               delay(delay_duration) ;
               turn_on_off_LED_number(RED_LED_PIN, OFF);
               delay(delay_duration) ;

           }
}

       void GPIO_PORTF_ISR(){
           //an interrupt has occurred; now we check whether it was button 1 or button 2
        int    button2_interrupt_triggered =( ( PORTF_GPIOMIS & SW2_PIN ) >>0 );
        int button1_interrupt_triggered =( ( PORTF_GPIOMIS & SW1_PIN ) >>4 );
           if(button1_interrupt_triggered ==1){
               send_data_uart(UART5_INSTANCE, text1_to_send, strlen(text1_to_send));


               delay_duration = 1000;

           }else if(button2_interrupt_triggered ==1){
               send_data_uart(UART5_INSTANCE, text2_to_send, strlen(text2_to_send));
               delay_duration = 2000;

           }
          //............... ........  ..................   ............
         //clear the bit in MIS and exit the interrupt
           PORTF_GPIOICR |= SW2_PIN;
           PORTF_GPIOICR |= SW1_PIN;

       }

