#include "main.h"
#include "bsp_uart.h"
#include "bsp_led.h"

#if USE_PRINTF
#include "xprintf.h"
#endif

#if USE_LWIP
#include "bsp_enet.h"
#endif

int main(void)
{
	board_init(); 
	bsp_uart_init();
	bsp_led_init(); 

#if USE_PRINTF
	xdev_out(uart_sendbyte);
#endif

#if USE_LWIP
	lwip_comm_init();
#endif

    while(1) 	
	{

	#if USE_LWIP
		lwip_periodic_handle(get_system_tick());
	#endif

    }
}

 