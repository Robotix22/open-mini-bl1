#include <memory.h>

#include <exynos9830.h>

void set_ps_hold(void)
{
    writel(0x1300, EXYNOS9830_POWER_PS_HOLD_CONTROL);
}