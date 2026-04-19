#include <stdint.h>
#include <stdbool.h>

#include <bootrom_funcs.h>

#include <bl31_checks.h>
#include <bl31_info.h>
#include <bl31_loader.h>

#include <exynos9810.h>

#include <gpio.h>
#include <pmu.h>

int main(void)
{
	uint32_t ret = 0;

	usb_reinit(USB_STRUCT_ADDR, USB_DELAY, USB_SPEED_FULLSPEED);
	usb_receive(0x02024800, 0x2000);

	usb_send("Setting up last bits of BL1...");
	usb_send("Test");
	complete_bl1_rx_exec();
	usb_send("OpenMiniBL1 - Started");

	set_ps_hold();
	setup_gpio_keys ();

	set_bl31_load_address();
	ret = load_bl31_usb();
	if(!ret)
		while(1);

	ret = verify_bl31_signature_and_rp_cnt(is_secure_boot());
	if(!ret)
		while(1);

	set_status_bit(0, BL1_END);
	jump_to_bl31();

	return -1;
}
