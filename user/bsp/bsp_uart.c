#include "bsp_uart.h"
#include "stdio.h"

// clang-format off

static const uint32_t COM_UART[COM_UART_MAX_NUM] =
{
    COM_UART0,
    COM_UART1,
    COM_UART2
};
static const IRQn_Type COM_UART_IRQn[COM_UART_MAX_NUM] =
{
    COM_UART0_IRQn,
    COM_UART1_IRQn,
    COM_UART2_IRQn
};
static const rcu_periph_enum COM_UART_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_CLK,
    COM_UART1_CLK,
    COM_UART2_CLK,
};
static const rcu_periph_enum COM_UART_TX_GPIO_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_GPIO_CLK,
    COM_UART1_TX_GPIO_CLK,
    COM_UART2_TX_GPIO_CLK
};
static const uint32_t COM_UART_TX_GPIO_AF[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_GPIO_AF,
    COM_UART1_TX_GPIO_AF,
    COM_UART2_TX_GPIO_AF
};
static const uint32_t COM_UART_TX_PORT[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_PORT,
    COM_UART1_TX_PORT,
    COM_UART2_TX_PORT
};
static const uint32_t COM_UART_TX_PIN[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_PIN,
    COM_UART1_TX_PIN,
    COM_UART2_TX_PIN
};
static const rcu_periph_enum COM_UART_RX_GPIO_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_GPIO_CLK,
    COM_UART1_RX_GPIO_CLK,
    COM_UART2_RX_GPIO_CLK
};
static const uint32_t COM_UART_RX_GPIO_AF[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_GPIO_AF,
    COM_UART1_RX_GPIO_AF,
    COM_UART2_RX_GPIO_AF
};
static const uint32_t COM_UART_RX_PORT[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_PORT,
    COM_UART1_RX_PORT,
    COM_UART2_RX_PORT
};
static const uint32_t COM_UART_RX_PIN[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_PIN,
    COM_UART1_RX_PIN,
    COM_UART2_RX_PIN
};

static const uint32_t COM_UART_BAUD[COM_UART_MAX_NUM] =
{
    COM_UART0_BAUD,
    COM_UART1_BAUD,
    COM_UART2_BAUD
};

// clang-format on

static void bsp_uart_periph_init(uart_type_def _com_uart)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable( COM_UART_TX_GPIO_CLK[_com_uart]);
    rcu_periph_clock_enable( COM_UART_RX_GPIO_CLK[_com_uart]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_UART_CLK[_com_uart]);

    /* connect port to USARTx_Tx */
    gpio_af_set(COM_UART_TX_PORT[_com_uart], COM_UART_TX_GPIO_AF[_com_uart], COM_UART_TX_PIN[_com_uart]);

    /* connect port to USARTx_Rx */
    gpio_af_set(COM_UART_RX_PORT[_com_uart], COM_UART_RX_GPIO_AF[_com_uart], COM_UART_RX_PIN[_com_uart]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(COM_UART_TX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_TX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_TX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_TX_PIN[_com_uart]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(COM_UART_RX_PORT[_com_uart], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_RX_PIN[_com_uart]);
    gpio_output_options_set(COM_UART_RX_PORT[_com_uart], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_RX_PIN[_com_uart]);

    /* USART configure */
    usart_deinit(COM_UART[_com_uart]);
    usart_baudrate_set(COM_UART[_com_uart],COM_UART_BAUD[_com_uart]);
    usart_receive_config(COM_UART[_com_uart], USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_UART[_com_uart], USART_TRANSMIT_ENABLE);
    usart_enable(COM_UART[_com_uart]);

    /* USART interrupt configuration */
    if (_com_uart == COM_UART_NUM0)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM1)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
    else if (_com_uart == COM_UART_NUM2)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 0, 0);
    }
    else if (_com_uart == COM_UART_NUM5)
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 1, 0);
    }
    else
    {
        nvic_irq_enable(COM_UART_IRQn[_com_uart], 2, 0);
    }
}
void bsp_uart_init(void)
{
    for (uart_type_def _uart = COM_UART_NUM0; _uart < COM_UART_MAX_NUM; _uart++)
    {
        bsp_uart_periph_init(_uart);
    }
}

void uart_sendbyte(uint8_t byte)
{
    usart_data_transmit(COM_UART[0], byte);
    while (RESET == usart_flag_get(COM_UART[0], USART_FLAG_TBE));
}

uint16_t uart_receivebyte(void)
{
    while (RESET == usart_flag_get(COM_UART[0], USART_FLAG_RBNE));
    return usart_data_receive(COM_UART[0]); 
}

/* ------------------通过重定向将printf函数映射到串口1上-------------------*/
#if 0

void _sys_exit(int x) //避免使用半主机模式
{
  x = x;
}
//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}
struct __FILE
{
  int handle;
};
FILE __stdout;

#endif

#if 1
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
PUTCHAR_PROTOTYPE
{
  /* 实现串口发送一个字节数据的函数 */
  uart_sendbyte((uint8_t)ch);
  return ch;
}

