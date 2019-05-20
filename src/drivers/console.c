#include "console.h"
static uint16_t *vedio_memory=(uint16_t *)0xB8000;
static uint8_t curser_x=0;
static uint8_t curser_y=0;

static void move_curser()
{
    uint16_t curser_location=curser_y * 80 + curser_x;
    
    outb(0x3D4,14);//14 is the upper
    outb(0x3D5,curser_location >> 8);
    outb(0x3D4,15);//15 is the lowwer
    outb(0x3D5,(uint8_t)curser_location);
}

void clear_console()
{
    int i;
    uint8_t contribute=(0 << 4) | (15 & 0x0F);
    //black background and white words
    uint16_t black = 0x20 | (contribute << 8);
    //Space

    for(i=0;i<80*25;i++)
        vedio_memory[i]=black;

    curser_x=0;
    curser_y=0;
    move_curser();
}

static void screen_roll()
{
    uint8_t contribute = (0 << 4) | (15 & 0x0F);
    uint16_t black = 0x20 | (contribute << 8);

    if(curser_y >= 25)
    {
        int i;
        for(i=80;i<80*25;i++) 
            vedio_memory[i-80]=vedio_memory[i];
        for(i=80*24;i<80*25;i++)
            vedio_memory[i]=black;
        curser_y=24;
        move_curser();
    }
}

void console_putc_color(char c,real_color_t back,real_color_t fore)
{
    uint8_t back_color = (uint8_t)back << 4;
    uint8_t fore_color = (uint8_t)fore & 0x0F;
    uint8_t contribute_byte = back_color | fore_color;
    uint16_t contribute = contribute_byte << 8;

    if(c == 0x08 && curser_x)//tuigejian
        curser_x --;
    else if(c == 0x09)//tab
        curser_x = (curser_x + 8) & 7; 
    else if(c == '\r')
        curser_x = 0;
    else if(c == '\n')
    {
        curser_x = 0;
        curser_y ++;
    }
    else if(c >= ' ')
    {
        vedio_memory[curser_x + curser_y * 80] = c | contribute;
        curser_x ++;
    }

    if(curser_x >= 80)
    {
        curser_x = 0;
        curser_y ++;
    }

    screen_roll();

    move_curser();
}

int random = 0;
void console_putc_random_color(char c)
{
    random += 3;
    if(random >= 10)
        random %= 10;
    if(random == 0)
        random=3;       
    
    uint16_t contribute_random = ((0 << 4) | (random & 0x0F)) << 8;
    uint16_t contribute = ((0 << 4) | (15 & 0x0F)) << 8;

    if(c == 0x08 && curser_x)//tuigejian
        curser_x --;
    else if(c == 0x09)//tab
        curser_x = (curser_x + 8) & 7; 
    else if(c == '\r')
        curser_x = 0;
    else if(c == '\n')
    {
        curser_x = 0;
        curser_y ++;
    }
    else if(c > ' ')
    {
        vedio_memory[curser_x + curser_y * 80] = c | contribute_random;
        curser_x ++;
    }
    else if(c = ' ')
    {
        vedio_memory[curser_x + curser_y * 80] = c | contribute;
        curser_x ++;
    }

    if(curser_x >= 80)
    {
        curser_x = 0;
        curser_y ++;
    }

    screen_roll();

    move_curser();
}
void console_write(char *cstr)
{
    int i=0;
    while(cstr[i] != '\0')
        console_putc_color(cstr[i++],rc_black,rc_white);
}

void console_write_color(char *cstr,real_color_t back,real_color_t fore)
{
    int i=0;
    while(cstr[i] != '\0')
        console_putc_color(cstr[i++],back,fore);
}

char ascii_0toF[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void console_write_hex(uint32_t n,real_color_t back,real_color_t fore)
{
    int stack[9];
    int base=0,top=0;
    
    while(n != 0)
    {
        stack[top++] = n % 16;
        n = n / 16;
    }
    while(top != base)
        console_putc_color(ascii_0toF[stack[--top]],back,fore);
}

void console_write_dec(uint32_t n,real_color_t back,real_color_t fore)
{
    int stack[20];
    int base=0,top=0;
    
    while(n != 0)
    {
        stack[top++] = n % 10;
        n = n / 10;
    }
    while(top != base)
        console_putc_color(ascii_0toF[stack[--top]],back,fore);
}

void console_write_random_color(char *cstr)
{
    int i=0;
    while(cstr[i] != '\0')
        console_putc_random_color(cstr[i++]);
}
