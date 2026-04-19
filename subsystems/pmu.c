#include <memory.h>

#include <exynos9810.h>

void set_ps_hold(void)
{
    writel(0x5300, EXYNOS9810_POWER_PS_HOLD_CONTROL);
}