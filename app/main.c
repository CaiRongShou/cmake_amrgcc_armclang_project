#include "main.h"
#include "bsp_uart.h"

#if USE_LED
#include "bsp_led.h"
#endif

#if USE_PRINTF
#include "xprintf.h"
#endif
 
#if USE_LWIP
#include "bsp_enet.h"
#endif


#if USE_USB
#define USBFS_REG_BASE                0x50000000L   /*!< base address of USBFS registers */


void usb_dc_low_level_init(uint8_t busid)
{
    nvic_irq_enable((uint8_t)USBFS_IRQn, 0U, 0U);

    rcu_periph_clock_enable(RCU_SYSCFG);
    rcu_periph_clock_enable(RCU_GPIOA);

    /* USBFS_DM(PA11) and USBFS_DP(PA12) GPIO pin configuration */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11 | GPIO_PIN_12);

    gpio_af_set(GPIOA, GPIO_AF_10, GPIO_PIN_11 | GPIO_PIN_12);

    rcu_pll48m_clock_config(RCU_PLL48MSRC_PLLQ);
    rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);
    rcu_periph_clock_enable(RCU_USBFS);

}

void usb_dc_low_level_deinit(uint8_t busid)
{
}

#endif


int main(void)
{
	board_init(); 
	bsp_uart_init();

#if USE_LED
	bsp_led_init(); 
#endif

#if USE_PRINTF
	xdev_out(uart_sendbyte);
#endif

#if USE_LWIP
	lwip_comm_init();
#endif

#if USE_LED
	bsp_led_off(LED1);
#endif

#if USE_USB
void cdc_acm_init(uint8_t busid, uintptr_t reg_base);
	cdc_acm_init(0,USBFS_REG_BASE);
#endif
    while(1) 	
	{
	#if USE_USB
		
		void cdc_acm_data_send_with_dtr_test(uint8_t busid);
		cdc_acm_data_send_with_dtr_test(0);
		delay_ms(1000);
	#endif
	#if USE_LED
		bsp_led_toggle(LED1);
		delay_ms(500);
	#endif

	#if USE_LWIP
		lwip_periodic_handle(get_system_tick());
	#endif

    }
}

 