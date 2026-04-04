#pragma once

void unknown_func_lol(uint32_t addr);
uint32_t get_bl1_boot_flag(void);
void set_status_bit(uint32_t unk1, uint32_t unk2);
void usb_reinit(uint32_t struct_addr, uint32_t delay, uint32_t speed);
void raw_usb_send(uint32_t address, uint32_t len);
uint32_t usb_receive(uint32_t address, uint32_t max_size);
void usb_send(char *msg);