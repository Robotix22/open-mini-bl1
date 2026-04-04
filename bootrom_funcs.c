#include <stdint.h>

#include <string.h>
#include <exynos9830.h>
#include <memory.h>

void unknown_func_lol(uint32_t addr)
{
	writel(readl(IRAM_UNK1), addr); // Isn't this redundant???
	writel(readl(0x15920090) - readl(IRAM_UNK1), addr); // TODO: find what 0x15920090 is
}

uint32_t get_bl1_boot_flag(void)
{
    return ((uint32_t(*)(void))readl(PTR_GRAB_BL1_BOOT_FLAG))();
}

void set_status_bit(uint32_t unk1, uint32_t unk2)
{
    ((void(*)(uint32_t, uint32_t))readl(PTR_SET_STATUS_BIT))(unk1, unk2);
}

void usb_reinit(uint32_t struct_addr, uint32_t delay, uint32_t speed)
{
    ((void(*)(uint32_t, uint32_t, uint32_t))readl(PTR_USB_REINIT))(struct_addr, delay, speed);
}

void raw_usb_send(uint32_t address, uint32_t len)
{
    ((void(*)(uint32_t, uint32_t))readl(PTR_USB_SEND))(address, len);
}

uint32_t usb_receive(uint32_t address, uint32_t max_size)
{
    return ((uint32_t(*)(uint32_t, uint32_t))readl(PTR_USB_RECEIVE))(address, max_size);
}

void usb_send(char *msg)
{
    raw_usb_send((uint32_t)msg, strlen(msg));
}
