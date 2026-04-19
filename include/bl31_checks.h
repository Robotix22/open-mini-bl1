#pragma once
#include <stdint.h>

uint32_t bl31_head_check(void);
uint32_t verify_bl31_signature_and_rp_cnt(uint32_t secureboot_status);