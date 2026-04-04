#pragma once
#include <stdint.h>

#include <stdint.h>

inline void writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
}

inline uint32_t readl(uint32_t addr)
{
	return *(volatile uint32_t*)addr;
}

// Thank Roger for his Mommy Ortiz Sequence
#define SEARCH_PATTERN_ARM(start_addr, end_addr, pat0, pat1)                   \
    ({                                                                         \
        const uint32_t pattern[2] = { (pat0), (pat1) };                        \
        uint32_t result = 0;                                                   \
                                                                               \
        uint32_t max_addr = (end_addr) - (2 * 4);                              \
        for (uint32_t offset = (start_addr); offset < max_addr; offset += 4) { \
            uint32_t first_val = *(volatile uint32_t*)offset;                  \
            if (first_val != pattern[0])                                       \
                continue;                                                      \
                                                                               \
            uint32_t second_val = *(volatile uint32_t*)(offset + 4);           \
            if (second_val != pattern[1])                                      \
                continue;                                                      \
                                                                               \
            result = offset;                                                   \
            break;                                                             \
        }                                                                      \
                                                                               \
        result;                                                                \
    })
