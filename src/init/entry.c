#include "console.h"
int kernel_entry()
{
    clear_console();
    console_write_color("cwp_kernel_0.11:Hello,World! today is 5.20\n",rc_black,rc_green);
    return 0;
}
