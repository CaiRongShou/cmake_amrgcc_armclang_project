#include "main.h"
#include "bsp_uart.h"
#include "bsp_enet.h"
#include "bsp_led.h"
#include "xprintf.h"


int main(void)
{
	board_init();
	bsp_uart_init();
	bsp_led_init(); 
	xdev_out(uart_sendbyte);
	lwip_comm_init();
    while(1) 	
	{
		lwip_periodic_handle(get_system_tick());
    }
}

 