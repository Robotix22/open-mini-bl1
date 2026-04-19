#include <stdint.h>

#include <bootrom_funcs.h>

#include <bl31_info.h>
#include <bl31_checks.h>

#include <exynos9810.h>

#include <memory.h>

uint32_t load_bl31_usb(void)
{
	uint32_t ret;

	store_rtc_time_difference(TIME_BL31_USB_LOAD_START);
	set_status_bit(1, BL31_LOAD_START);
	usb_send("Ready to rx BL31");

	ret = usb_receive(get_bl31_load_address(), 0x02059000 - get_bl31_load_address());
	if(ret & 0xFF)
	{
		usb_send("BL31 rx done");
		ret = bl31_head_check();
		if(!ret)
			usb_send("BL31 Header fail");
		else
			usb_send("BL31 Header pass");
	}
	else
	{
		usb_send("BL31 rx fail");
		ret = 0;
	}
	
	store_rtc_time_difference(TIME_BL31_USB_LOAD_END);

	if(ret)
		set_status_bit(0, BL1_LOAD_BL31_SUCCESS);

	return ret;
}

void jump_to_bl31(void)
{
	((void(*)(void))(uintptr_t)(get_bl31_load_address() + 0x0010))();
}
