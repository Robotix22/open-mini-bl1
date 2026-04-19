#include <stdint.h>
#include <stdbool.h>

#include <bootrom_funcs.h>

#include <bl31_info.h>
#include <exynos9810.h>
#include <memory.h>

bool is_head_marker_present()
{
    return readl(get_bl31_load_address() + 0x0008) == 0x68656164; // ASCII for "head"
}

uint32_t bl31_head_check(void)
{
    uint32_t ret = 0;

    if(is_head_marker_present())
    {
        uint32_t bl31_block_size = readl(get_bl31_load_address());

        if ((bl31_block_size > 2) &&
            (bl31_block_size <= (0x02059000 - (get_bl31_load_address() >> 9))))
            {
                set_bl31_size(bl31_block_size * BLOCK_SIZE);
                set_bl31_expected_hash(readl(get_bl31_load_address() + 0x0004));
                writel(0, get_bl31_load_address() + 0x0004); // Clear hash in binary
                ret = 1;
            }
    }

    return ret;
}

uint32_t check_bl31_hash(void)
{
    // Stub
    set_status_bit(1, BL31_CHECKSUM_VALIDATION_SUCCESS);
    return 1;
}

uint32_t verify_bl31(void)
{
    // Stub
    set_status_bit(1, BL31_VERIFICATION_SUCCESS);
    return 1;
}

uint32_t check_bl31_arb(void)
{
    // Stub
    set_status_bit(1, BL31_ARB_VALIDATION_SUCCESS);
    return 1;
}

uint32_t verify_bl31_signature_and_rp_cnt(uint32_t secure_boot_status)
{
    uint32_t ret;

    if(!secure_boot_status)
    {
        ret = check_bl31_hash();
        if(!ret) return 0;

        store_rtc_time_difference(TIME_BL31_HASH_VALIDATION);
    }
    else
    {
        ret = verify_bl31();
        if (!ret) return 0;

        ret = check_bl31_arb();
        if(!ret) return 0;

        store_rtc_time_difference(TIME_BL31_VERFICATION);
        // Some other verification func gets called here, no point implementing, doesn't seem to set a status bit either
    }

    return ret;
}