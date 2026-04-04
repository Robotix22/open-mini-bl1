#include <stdint.h>
#include <exynos9830.h>
#include <memory.h>

uint32_t get_bl1_size(void)
{
	return readl(IRAM_BL1_SIZE);
}

void set_bl1_size(uint32_t size)
{
	writel(size, IRAM_BL1_SIZE);
}

void set_bl1_checksum(uint32_t sha512)
{
	writel(sha512, IRAM_BL1_CHECKSUM);
}