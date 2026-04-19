#pragma once
#include <stdint.h>

#define BLOCK_SIZE (512)

uint32_t get_bl31_load_address(void);
void set_bl31_load_address(void);
uint32_t get_bl31_size(void);
void set_bl31_size(uint32_t size);
void set_bl31_expected_hash(uint32_t sha512);
