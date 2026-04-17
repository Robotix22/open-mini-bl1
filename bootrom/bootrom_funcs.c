#include <stdint.h>
#include <string.h>

#include <bl1_info.h>

#include <exynos9830.h>

#include <memory.h>

void store_rtc_time_difference(uint32_t addr)
{
	uint32_t base = readl(IRAM_RTCTICK_STORE);
	uint32_t now  = readl(EXYNOS9830_RTC_CURTICCNT_0);

	writel(now - base, addr);
}

uint32_t is_secure_boot(void)
{
	return ((uint32_t(*)(void))(uintptr_t)readl(PTR_IS_SECUREBOOT))();
}

void set_status_bit(uint32_t status_register_sel, uint32_t status_bit)
{
	((void(*)(uint32_t, uint32_t))(uintptr_t)readl(PTR_SET_STATUS_BIT))(status_register_sel, status_bit);
}

void usb_reinit(uint32_t struct_addr, uint32_t delay, uint32_t speed)
{
	((void(*)(uint32_t, uint32_t, uint32_t))(uintptr_t)readl(PTR_USB_REINIT))(struct_addr, delay, speed);
}

void raw_usb_send(uint32_t address, uint32_t len)
{
	((void(*)(uint32_t, uint32_t))(uintptr_t)readl(PTR_USB_SEND))(address, len);
}

uint32_t usb_receive(uint32_t address, uint32_t max_size)
{
	return ((uint32_t(*)(uint32_t, uint32_t))(uintptr_t)readl(PTR_USB_RECEIVE))(address, max_size);
}

void usb_send(char *msg)
{
	raw_usb_send((uint32_t)(uintptr_t)msg, strlen(msg));
}

void setup_cryptocell_func_ptrs(void)
{
	uint32_t addr = (get_bl1_size() + 0x020216D0 + 0x0474);

	writel(0x16910, addr);
	writel(0x169A0, addr + 0x0004);
	writel(0x12BF0, addr + 0x0008);
	writel(0x167D8, addr + 0x000C);
	writel(0x16850, addr + 0x0010);
	writel(0x0ADC8, addr + 0x0014);
	writel(0x0AE04, addr + 0x0018);
	writel(0x0A984, addr + 0x001C);
}

void complete_bl1_rx_exec(void)
{
	set_bl1_size(0x3000);
	set_bl1_checksum(0x7A1A1BEF);
	writel((0x3000) / BLOCK_SIZE, 0x02022000); // Set size parameter, without this EL3_MON will freak out.
	setup_cryptocell_func_ptrs();
}
