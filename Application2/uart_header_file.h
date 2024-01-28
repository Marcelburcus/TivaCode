
#include <stdint.h>

//struct for the function uart_set_up
typedef struct UART_Param{
    uint32_t baud_rate;
    uint8_t tx_rx;
    char instance;
} UART_Param_t;

//struct for the function uart_gpio_configuration
typedef struct UART_GPIO{
    uint8_t  port;
    uint32_t  pin_number;
    uint8_t  functionality;

}UART_GPIO_t;

// Macro Definitions
// UARTFR is the flag register. When TXFE == 1, the transmit register is empty and UART_DR can be loaded with new data
#define UART_FR_TXFE (1 << 7)

// UART Functionality Configuration
#define UART_FUNCTIONALITY 1  // Defines the functionality value for UART pins

// GPIO Port Definitions
#define PORTE 4               // Port E identifier
#define PORTA 0               // Port A identifier
#define PORTC 2               // Port C identifier

// Enable/Disable Configuration
#define ENABLE 1
#define DISABLE 0

// UART Instance Identifiers
#define UART0_INSTANCE '0'
#define UART5_INSTANCE '5'

// UART Transmission/Reception Configuration
#define TX_RX_BOTH 0          // Configure for both transmission and reception
#define TX_ONLY 1             // Configure for transmission only
#define RX_ONLY 2             // Configure for reception only

// GPIO Module Configuration
// UART PE4 und PE5 correspond to Uart Module 5
// this will be used in RCGSUART Register
#define UART_MODULE5 (1 << 5)
#define UART_MODULE0 (1 << 0)

#define RCGCGPIO_PORTE (1 << 4)
#define RCGCGPIO_PORTA (1 << 0)
#define RCGCGPIO_PORTC (1 << 2)

// Base Addresses
#define PORTE_GPIO_BASE_ADDRESS (0x40024000)
#define PORTA_GPIO_BASE_ADDRESS (0x40004000)
#define PORTC_GPIO_BASE_ADDRESS (0x40006000)

#define UART5_BASE_ADDRESS (0x40011000)
#define UART0_BASE_ADDRESS (0x4000C000)

// System Control Base Address: UART, GPIO, etc share the same base address. See page 231
#define SYSCTL_BASE_ADDRESS (0x400FE000)

// Offset Definitions
#define RCGCUART_OFFSET (0x618)
#define RCGCUART (*((uint32_t*)(SYSCTL_BASE_ADDRESS + RCGCUART_OFFSET)))


#define RCGCGPIO_OFFSET (0x608)
#define RCGCGPIO (*((uint32_t*)(SYSCTL_BASE_ADDRESS + RCGCGPIO_OFFSET)))

// Alternate Function
#define GPIOAFSEL_OFFSET (0x420)

#define PORTE_GPIOAFSEL (*((uint32_t*)(PORTE_GPIO_BASE_ADDRESS + GPIOAFSEL_OFFSET)))
#define PORTA_GPIOAFSEL (*((uint32_t*)(PORTA_GPIO_BASE_ADDRESS + GPIOAFSEL_OFFSET)))
#define PORTC_GPIOAFSEL (*((uint32_t*)(PORTC_GPIO_BASE_ADDRESS + GPIOAFSEL_OFFSET)))

// GPIO port control is used with Afsel.
#define GPIOPCTL_OFFSET (0x52C)

#define PORTE_GPIOPCTL (*((uint32_t*)(PORTE_GPIO_BASE_ADDRESS + GPIOPCTL_OFFSET)))
#define PORTA_GPIOPCTL (*((uint32_t*)(PORTA_GPIO_BASE_ADDRESS + GPIOPCTL_OFFSET)))
#define PORTC_GPIOPCTL (*((uint32_t*)(PORTC_GPIO_BASE_ADDRESS + GPIOPCTL_OFFSET)))

// DEN register
#define GPIODEN_OFFSET (0x51C)
#define PORTE_GPIODEN (*((uint32_t*)(PORTE_GPIO_BASE_ADDRESS + GPIODEN_OFFSET)))

// Integer baud rate divisor
#define UARTIBRD_OFFSET (0x024)

#define UART5_IBRD (*((uint32_t*)(UART5_BASE_ADDRESS + UARTIBRD_OFFSET)))
#define UART0_IBRD (*((uint32_t*)(UART0_BASE_ADDRESS + UARTIBRD_OFFSET)))

// Fractional baud rate divisor
#define UARTFBRD_OFFSET (0x028)
#define UART5_FBRD (*((uint32_t*)(UART5_BASE_ADDRESS + UARTFBRD_OFFSET)))
#define UART0_FBRD (*((uint32_t*)(UART0_BASE_ADDRESS + UARTFBRD_OFFSET)))

// Flag register
#define UARTFR_OFFSET (0x018)
#define UART5_FR (*((uint32_t*)(UART5_BASE_ADDRESS + UARTFR_OFFSET)))
#define UART0_FR (*((uint32_t*)(UART0_BASE_ADDRESS + UARTFR_OFFSET)))

// Data register
#define UARTDR_OFFSET (0x000)
#define UART5_DR (*((uint32_t*)(UART5_BASE_ADDRESS + UARTDR_OFFSET)))
#define UART0_DR (*((uint32_t*)(UART0_BASE_ADDRESS + UARTDR_OFFSET)))

// Line control register: When we update IBRD and IFRD, this register must be written
// Here we implement data length, parity, stop bit
#define UARTLCRH_OFFSET (0x02C)
#define UART5_LCRH (*((uint32_t*)(UART5_BASE_ADDRESS + UARTLCRH_OFFSET)))
#define UART0_LCRH (*((uint32_t*)(UART0_BASE_ADDRESS + UARTLCRH_OFFSET)))

// It controls the baud clock source for the UART module
#define UARTCC_OFFSET (0xFC8)
#define UART5_CC (*((uint32_t*)(UART5_BASE_ADDRESS + UARTCC_OFFSET)))
#define UART0_CC (*((uint32_t*)(UART0_BASE_ADDRESS + UARTCC_OFFSET)))


// UART5 Control Functions
void enable_UART5_CTL();
void disable_UART5_CTL();

void enable_TX_UART5_CTL();
void enable_RX_UART5_CTL();
void disable_RX_UART5_CTL();
void disable_TX_UART5_CTL();

// UART0 Control Functions
void enable_UART0_CTL();
void disable_UART0_CTL();

void enable_TX_UART0_CTL();
void enable_RX_UART0_CTL();
void disable_RX_UART0_CTL();
void disable_TX_UART0_CTL();

// GPIO Control Functions
void enable_PORTF();
void enable_PORTA();
void enable_PORTC();

// UART GPIO Configuration
void uart_gpio_Configuration(UART_GPIO_t *configuration);

// Data Transmission Functions
void send_data_uar5(char* word, int word_length);
void send_data_uart0(char* word, int word_length);
void send_data_uart(char instance, char* word, int word_length);

// Baud Rate Configuration Functions
void set_baudrate_uart5(uint32_t baud_rate);
void set_baudrate_uart0(uint32_t baud_rate);

// UART Setup Function
void uart_set_up(UART_Param_t *uart);
