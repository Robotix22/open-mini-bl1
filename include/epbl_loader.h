#pragma once
#include <stdint.h>

void detect_and_patch_decrypted_epbl(void);
uint32_t load_epbl_usb(void);
void jump_to_epbl(void);