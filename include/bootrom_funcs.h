#pragma once

void store_rtc_time_difference(uint32_t addr);
uint32_t is_secure_boot(void);
void set_status_bit(uint32_t unk1, uint32_t unk2);
void usb_reinit(uint32_t struct_addr, uint32_t delay, uint32_t speed);
void raw_usb_send(uint32_t address, uint32_t len);
uint32_t usb_receive(uint32_t address, uint32_t max_size);
void usb_send(char *msg);
void setup_cryptocell_func_ptrs(void);
void complete_bl1_rx_exec(void);