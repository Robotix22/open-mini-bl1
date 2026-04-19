#include <stdint.h>
#include <string.h>

#include <bl1_info.h>

#include <exynos9810.h>

#include <memory.h>

void store_rtc_time_difference(uint32_t addr)
{
	uint32_t base = readl(IRAM_RTCTICK_STORE);
	uint32_t now  = readl(EXYNOS9810_RTC_CURTICCNT_0);

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

void usleep_range(uint64_t start_time, uint64_t end_time)
{
	((void(*)(uint64_t, uint64_t))(uintptr_t)readl(PTR_USLEEP_RANGE))(start_time, end_time);
}

void usb_reinit(uint32_t struct_addr, uint32_t delay, uint32_t speed)
{
	((void(*)(uint32_t, uint32_t, uint32_t))(uintptr_t)readl(PTR_USB_REINIT))(struct_addr, delay, speed);
}

uint32_t usb_receive(uint32_t address, uint32_t max_size)
{
	return ((uint32_t(*)(uint32_t, uint32_t))(uintptr_t)readl(PTR_USB_RECEIVE))(address, max_size);
}

void usb_send(char *msg)
{
	volatile uint32_t *trb		   = (volatile uint32_t *)0x02037000;
	volatile uint32_t *depcmd_par1 = (volatile uint32_t *)0x10C0C834;
	volatile uint32_t *depcmd_par0 = (volatile uint32_t *)0x10C0C838;
	volatile uint32_t *depcmd	   = (volatile uint32_t *)0x10C0C83C;

	trb[0] = (uint32_t)(uintptr_t)msg;
	trb[1] = 0;
	trb[2] = strlen(msg) & 0x00FFFFFF;
	trb[3] = 0xC13;

	*depcmd_par1 = 0x02037000;
	*depcmd_par0 = 0;

	*depcmd = 0x406;

	usleep_range(1, USB_DELAY);
}

void setup_cryptocell_func_ptrs(void)
{
	uint32_t addr = (get_bl1_size() + 0x02021400 + 0x0144);

	writel(0x9528, addr);
	writel(0x9348, addr + 0x0004);
	writel(0x91F4, addr + 0x0008);
	writel(0x90DC, addr + 0x000C);
	writel(0x8F60, addr + 0x0010);
	writel(0x4860, addr + 0x0014);
	writel(0x48A4, addr + 0x0018);
	writel(0x49BC, addr + 0x001C);
}

void complete_bl1_rx_exec(void)
{
	uint32_t secureboot_key_address = readl(PTR_SECUREBOOT_KEY);
	uint32_t bl1_sign_type = 0;

	set_bl1_size(0x2000);
	set_bl1_checksum(0x7A1A1BEF);
	writel(0, 0x02021804); // Clear checksum, though for us it's already clear

	bl1_sign_type = readl(get_bl1_size() + 0x2021400 + 0x14);

	// Write status bit before checksum validation
	writel(readl(IRAM_STATUS_REG1) | BL1_CHECKSUM_VALIDATION_START, IRAM_STATUS_REG1);

	// Write status bit upon succesful check of BL1 against checksum
	writel(readl(IRAM_STATUS_REG1) | BL1_CHECKSUM_VALIDATION_SUCCESS, IRAM_STATUS_REG1);

	store_rtc_time_difference(TIME_BL1_CHECKSUM_VALIDATION);
	writel(readl(IRAM_STATUS_REG1) & ~(0x1800), IRAM_STATUS_REG1);
	writel(secureboot_key_address | bl1_sign_type, IRAM_SBC_INFORMATION);

	set_status_bit(0, BL1_VERIFICATION_SUCCESS);
	set_status_bit(1, (1 << 14));

	store_rtc_time_difference(TIME_BL1_VERIFICATION);

	set_status_bit(1, (1 << 22));

	setup_cryptocell_func_ptrs();
	store_rtc_time_difference(TIME_BL1_START);
	set_status_bit(0, BL1_START);
}
