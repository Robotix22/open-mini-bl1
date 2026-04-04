#pragma once

#include <stdint.h>
uint32_t get_bl1_size(void);
void set_bl1_size(uint32_t size);
void set_bl1_checksum(uint32_t sha512);