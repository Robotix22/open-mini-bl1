#include <stdint.h>

#include <exynos9810.h>
#include <memory.h>
#include <bl1_info.h>

uint32_t get_bl31_load_address(void)
{
	return readl(PTR_BL31_LOAD_ADDRESS);
}

void set_bl31_load_address(void)
{
	writel(get_bl1_size() + 0x02022000, PTR_BL31_LOAD_ADDRESS);
}

uint32_t get_bl31_size(void)
{
	return readl(IRAM_BL31_SIZE);
}

void set_bl31_size(uint32_t size)
{
	writel(size, IRAM_BL31_SIZE);
}

void set_bl31_expected_hash(uint32_t sha512)
{
	writel(sha512, IRAM_BL31_EXPECTED_HASH);
}
