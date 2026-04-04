#include <stdint.h>

#include <exynos9830.h>
#include <memory.h>
#include <bl1_info.h>

uint32_t get_epbl_load_address(void)
{
	return readl(PTR_EPBL_LOAD_ADDRESS);
}

void set_epbl_load_address(void)
{
	writel(get_bl1_size() + 0x02023000, PTR_EPBL_LOAD_ADDRESS);
}

uint32_t get_epbl_size(void)
{
	return readl(IRAM_EPBL_SIZE);
}

void set_epbl_size(uint32_t size)
{
	writel(size, IRAM_EPBL_SIZE);
}

void set_epbl_expected_hash(uint32_t sha512)
{
	writel(sha512, IRAM_EPBL_EXPECTED_HASH);
}
