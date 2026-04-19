#include <stdint.h>

#include <memory.h>

#include <exynos9810.h>

void gpio_set_pull(uintptr_t pinctrl, uint8_t pin, uint8_t pull_mode)
{
    uint32_t value;

    value  = readl(pinctrl);
	value &= ~(0xF << ((pin) << 2));
	value |= ((pull_mode) << ((pin) << 2));
	writel(value, pinctrl);
}

void gpio_cfg_pin(uintptr_t pinctrl, uint8_t pin, uint8_t config)
{
    uint32_t value;

    value  = readl(pinctrl);
	value &= ~(0xF << ((pin) << 2));
	value |= ((config) << ((pin) << 2));
	writel(value, pinctrl);
}

void setup_gpio_keys(void)
{
    gpio_set_pull(EXYNOS9810_ALIVE_PINCTRL_GPA0 + 0x8, 3, 3);
    gpio_set_pull(EXYNOS9810_ALIVE_PINCTRL_GPA0 + 0x8, 4, 3);
    gpio_set_pull(EXYNOS9810_ALIVE_PINCTRL_GPA0 + 0x8, 6, 3);
    gpio_set_pull(EXYNOS9810_ALIVE_PINCTRL_GPA2 + 0x8, 4, 3);

    gpio_cfg_pin(EXYNOS9810_ALIVE_PINCTRL_GPA0, 3, 0);
    gpio_cfg_pin(EXYNOS9810_ALIVE_PINCTRL_GPA0, 4, 0);
    gpio_cfg_pin(EXYNOS9810_ALIVE_PINCTRL_GPA0, 6, 0);
    gpio_cfg_pin(EXYNOS9810_ALIVE_PINCTRL_GPA2, 4, 0);
}
