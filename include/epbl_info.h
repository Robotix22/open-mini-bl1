#pragma once
#include <stdint.h>

#define BLOCK_SIZE (512)

uint32_t get_epbl_load_address(void);
void set_epbl_load_address(void);
uint32_t get_epbl_size(void);
void set_epbl_size(uint32_t size);
void set_epbl_expected_hash(uint32_t sha512);
