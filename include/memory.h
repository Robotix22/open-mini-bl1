#pragma once
#include <stdint.h>

#define writel(v, a) (*(volatile uint32_t *)(uintptr_t)(a) = (v))
#define readl(a) (*(volatile uint32_t *)(uintptr_t)(a))
