#include "main.h"
#include "bsp_uart.h"
#include "bsp_enet.h"
#include "bsp_led.h"



int main(void)
{
	board_init();
	
	bsp_led_init();
    while(1) 	{
		bsp_led_toggle(LED1);
		delay_ms(500);
    }
}

