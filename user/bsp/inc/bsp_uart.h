#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "gd32f4xx.h"

//基本不使用USART，平时用的都是UART
typedef enum
{
    COM_UART_NUM0 = 0,
    COM_UART_NUM1 = 1,
    COM_UART_NUM2 = 2,
    COM_UART_NUM5 = 3,
    COM_UART_MAX_NUM
} uart_type_def;

#define COM_UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define COM_UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

//串口0的宏定义
#define COM_UART0                         USART0
#define COM_UART0_IRQn                    USART0_IRQn
#define COM_UART0_CLK                     RCU_USART0
#define COM_UART0_TX_GPIO_CLK             RCU_GPIOA
#define COM_UART0_TX_GPIO_AF              GPIO_AF_7
#define COM_UART0_TX_PORT                 GPIOA
#define COM_UART0_TX_PIN                  GPIO_PIN_9
#define COM_UART0_RX_GPIO_CLK             RCU_GPIOA
#define COM_UART0_RX_GPIO_AF              GPIO_AF_7
#define COM_UART0_RX_PORT                 GPIOA
#define COM_UART0_RX_PIN                  GPIO_PIN_10
#define COM_UART0_BAUD                    115200U

//串口1的宏定义
#define COM_UART1                         USART1
#define COM_UART1_IRQn                    USART1_IRQn
#define COM_UART1_CLK                     RCU_USART1
#define COM_UART1_TX_GPIO_CLK             RCU_GPIOA
#define COM_UART1_TX_GPIO_AF              GPIO_AF_7
#define COM_UART1_TX_PORT                 GPIOD
#define COM_UART1_TX_PIN                  GPIO_PIN_5
#define COM_UART1_RX_GPIO_CLK             RCU_GPIOD
#define COM_UART1_RX_GPIO_AF              GPIO_AF_7
#define COM_UART1_RX_PORT                 GPIOD
#define COM_UART1_RX_PIN                  GPIO_PIN_6
#define COM_UART1_BAUD                    115200U

//串口2的宏定义
#define COM_UART2                         USART2
#define COM_UART2_IRQn                    USART2_IRQn
#define COM_UART2_CLK                     RCU_USART2
#define COM_UART2_TX_GPIO_CLK             RCU_GPIOB
#define COM_UART2_TX_GPIO_AF              GPIO_AF_7
#define COM_UART2_TX_PORT                 GPIOB
#define COM_UART2_TX_PIN                  GPIO_PIN_10
#define COM_UART2_RX_GPIO_CLK             RCU_GPIOA
#define COM_UART2_RX_GPIO_AF              GPIO_AF_7
#define COM_UART2_RX_PORT                 GPIOB
#define COM_UART2_RX_PIN                  GPIO_PIN_11
#define COM_UART2_BAUD                    115200U


void bsp_uart_init(void);
void uart_sendbyte(uint8_t byte);
uint16_t uart_receivebyte(void);

#endif /* __BSP_UART_H__ */
