#include "console.h"
int kernel_entry()
{
    clear_console();
    console_write_color("cwp_kernel_0.11:Hello,World!   And today is 5.20\n",rc_black,rc_green);
    console_write_random_color("WOW the random color!   AMAZING!\n");
    return 0;
}
