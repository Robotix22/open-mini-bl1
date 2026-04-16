#pragma once
#include <stdint.h>

#include <stdint.h>

#define writel(v, a) (*(volatile uint32_t *)(uintptr_t)(a) = (v))
#define readl(a) (*(volatile uint32_t *)(uintptr_t)(a))

// Thank Roger for his Mommy Ortiz Sequence
#define SEARCH_PATTERN_ARM(start_addr, end_addr, pat0, pat1)                    \
    ({                                                                          \
        const uint32_t pattern[2] = { (pat0), (pat1) };                         \
        uint32_t result = 0;                                                    \
                                                                                \
        uint32_t max_addr = (end_addr) - (2 * 4);                               \
        for (uint32_t offset = (start_addr); offset < max_addr; offset += 4) {  \
            uint32_t first_val = *(volatile uint32_t*)(uintptr_t)offset;        \
            if (first_val != pattern[0])                                        \
                continue;                                                       \
                                                                                \
            uint32_t second_val = *(volatile uint32_t*)(uintptr_t)(offset + 4); \
            if (second_val != pattern[1])                                       \
                continue;                                                       \
                                                                                \
            result = offset;                                                    \
            break;                                                              \
        }                                                                       \
                                                                                \
        result;                                                                 \
    })
