#pragma once
#include <stdint.h>

#define BLOCK_SIZE (512)

uint32_t get_bl1_size(void);
void set_bl1_size(uint32_t size);
void set_bl1_checksum(uint32_t sha512);