#include <stdint.h>
#include <stdbool.h>

#include <exynos9830.h>
#include <memory.h>
#include <bootrom_funcs.h>
#include <pmu.h>
#include <epbl_info.h>
#include <epbl_checks.h>
#include <bl1_info.h>

uint32_t load_epbl_usb(void)
{
	uint32_t ret;

	unknown_func_lol(IRAM_UNK2);
	set_status_bit(1, 0x1000000);
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
	
	unknown_func_lol(IRAM_UNK3);

	if(ret)
		set_status_bit(0, 0x4000000);

	return ret;
}

void jump_to_epbl(void)
{
    ((void(*)(void))get_epbl_load_address() + 0x0010)();
}

void setup_cryptocell_func_ptrs(void)
{
	uint32_t addr = (get_bl1_size() + 0x020216D0 + 0x0474);

	writel(0x16910, addr);
	writel(0x169a0, addr + 0x0004);
	writel(0x12bf0, addr + 0x0008);
	writel(0x167d8, addr + 0x000C);
	writel(0x16850, addr + 0x0010);
	writel(0x0adc8, addr + 0x0014);
	writel(0x0ae04, addr + 0x0018);
	writel(0x0a984, addr + 0x001C);
}

void complete_bl1_rx_exec(void)
{
	set_bl1_size(0x3000);
	set_bl1_checksum(0x7A1A1BEF);
	writel(0, 0x02022004); // Clear checksum, though for us it's already clear

	// Write status bit before chksum validation
	writel(readl(0x02020070) | 0x4000, 0x02020070);

	// Write status bit upon succesful check of BL1 against chksum
	writel(readl(0x02020070) | 0x8000, 0x02020070);

	unknown_func_lol(0x2020084);
	uint32_t unk0 = readl(IRAM_BL1_UNK6);
	uint32_t unk1 = readl(get_bl1_size() + 0x20216d0 + 0x14);
	writel(unk0 | unk1, IRAM_BL1_UNK7);

	writel(readl(0x02020070) | 0x20000, 0x02020070);

	unknown_func_lol(0x2020088);

	setup_cryptocell_func_ptrs();
	unknown_func_lol(0x202008C);
	set_status_bit(0,0x800000);
}

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

uint8_t crappy_callback(uint32_t rx_addr, uint32_t size)
{
	if(rx_addr == 0xbfe80000)
	{
		writel(0x6368, PTR_USB_RECEIVE);
		usb_receive(0xe808c5f0, 0x3000);
		usb_receive(0x90000000, 404123);
		uint8_t ret = usb_receive(rx_addr, size);
		return ret;
	}

	writel(0x6368, PTR_USB_RECEIVE);
	uint8_t ret = usb_receive(rx_addr, size);
	writel((uint32_t)(uintptr_t)crappy_callback, PTR_USB_RECEIVE);
	return ret;
}

int main(void)
{
	usb_reinit(USB_STRUCT_ADDR, USB_DELAY, USB_SPEED_FULLSPEED);
	usb_send("Setting up last bits of BL1...");
	complete_bl1_rx_exec();
	usb_send("OpenMiniBL1 - Started");

	// TODO check if this is actually the boot flag
	uint32_t ret = 0;
	uint32_t bl1_boot_flag = get_bl1_boot_flag();
	set_ps_hold();
	set_epbl_load_address();
	ret = load_epbl_usb();
	if(!ret)
		while(1);

	ret = check_epbl_integ_and_auth(bl1_boot_flag);
	if(!ret)
		while(1);

	detect_and_patch_decrypted_epbl();

	set_status_bit(0,0x80000000);
	usb_send("OpenMiniBL1 - Attempting last minute patches...\n");
	writel((uint32_t)(uintptr_t)crappy_callback, PTR_USB_RECEIVE);
	usb_send("OpenMiniBL1 - Bye!");
	jump_to_epbl();
}
