#include <stdint.h>

#include <bootrom_funcs.h>

#include <epbl_info.h>
#include <epbl_checks.h>

#include <exynos9830.h>

#include <memory.h>

void detect_and_patch_decrypted_epbl(void)
{
	if(readl(get_epbl_load_address() + 0x002C) == 0xD503201F)
	{
		uint32_t ebre_marker_loc = 0;

		usb_send("OpenMiniBL1 - Decrypted EPBL detected, patching decryption status flags");
		usb_send("OpenMiniBL1 - Searching for EBRE marker...");

		ebre_marker_loc = SEARCH_PATTERN_ARM(get_epbl_load_address(),
											 get_epbl_load_address() + get_epbl_size(),
											 0x45524245, 0);

		if(ebre_marker_loc)
		{
			usb_send("OpenMiniBL1 - Found EBRE marker, setting decryption flags");
			writel(0x53554343, ebre_marker_loc - 0x0008);
			writel(readl(ebre_marker_loc + 0x0008) & 0xFFFF0000, ebre_marker_loc + 0x0008);
			usb_send("OpenMiniBL1 - Decryption flags set.");
		}
		else
		{
			usb_send("OpenMiniBL1 - Failed to find EBRE marker, continuing boot, if flags aren't set already, expect failed boot.");
		}
	}
}

uint32_t load_epbl_usb(void)
{
	uint32_t ret;

	store_rtc_time_difference(TIME_EPBL_USB_LOAD_START);
	set_status_bit(1, EPBL_LOAD_START);
	usb_send("Ready to rx EPBL");

	ret = usb_receive(get_epbl_load_address(), 0x02071000 - get_epbl_load_address());
	if(ret & 0xFF)
	{
		usb_send("EPBL rx done");
		ret = epbl_head_check();
		if(!ret)
			usb_send("EPBL Header fail");
		else
			usb_send("EPBL Header pass");
	}
	else
	{
		usb_send("EPBL rx fail");
		ret = 0;
	}
	
	store_rtc_time_difference(TIME_EPBL_USB_LOAD_END);

	if(ret)
		set_status_bit(0, BL1_LOAD_EPBL_SUCCESS);

	return ret;
}

void jump_to_epbl(void)
{
    ((void(*)(void))(uintptr_t)(get_epbl_load_address() + 0x0010))();
}
