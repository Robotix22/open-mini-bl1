#include <stdint.h>
#include <stdbool.h>

#include <bootrom_funcs.h>

#include <epbl_info.h>
#include <exynos9830.h>
#include <memory.h>

bool is_head_marker_present()
{
    return readl(get_epbl_load_address() + 0x0008) == 0x68656164; // ASCII for "head"
}

uint32_t epbl_head_check(void)
{
    uint32_t ret = 0;

    if(is_head_marker_present())
    {
        uint32_t epbl_block_size = readl(get_epbl_load_address());

        if ((epbl_block_size > 2) &&
            (epbl_block_size <= (0x02071000 - (get_epbl_load_address() >> 9))))
            {
                set_epbl_size(epbl_block_size * BLOCK_SIZE);
                set_epbl_expected_hash(readl(get_epbl_load_address() + 0x0004));
                writel(0, get_epbl_load_address() + 0x0004); // Clear hash in binary for whatever reason
                ret = 1;
            }
    }

    return ret;
}

uint32_t check_epbl_hash(void)
{
    // stub
    set_status_bit(1, 0x4000000);
    return 1;
}

uint32_t verify_epbl_i_think(void)
{
    // stub
    set_status_bit(1, 0x8000000);
    return 1;
}

uint32_t check_epbl_arb(void)
{
    // stub
    set_status_bit(1, 0x10000000);
    return 1;
}

uint32_t check_epbl_integ_and_auth(uint32_t boot_flag)
{
    uint32_t ret;
    // stub, meh
    if(!boot_flag)
    {
        ret = check_epbl_hash();
        if(!ret) return 0;

        unknown_func_lol(IRAM_UNK4);
    }
    else
    {
        ret = verify_epbl_i_think();
        if (!ret) return 0;

        ret = check_epbl_arb();
        if(!ret) return 0;

        unknown_func_lol(IRAM_UNK5);
        // some other verification func gets called here, cba, doesn't seem to set a status bit either
    }

    return ret;
}