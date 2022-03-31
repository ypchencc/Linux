#include "main.h"

void clock_enable(void)
{
    CCM_CCGR0 = 0XFFFFFFF;
    CCM_CCGR1 = 0XFFFFFFF;
    CCM_CCGR2 = 0XFFFFFFF;
    CCM_CCGR3 = 0XFFFFFFF;
    CCM_CCGR4 = 0XFFFFFFF;
    CCM_CCGR5 = 0XFFFFFFF;
    CCM_CCGR6 = 0XFFFFFFF;
}

void led_init(void)
{
    SW_PAD_GPIO1_IO03 = 0X10B0;
    // io configuration
    SW_MUX_GPIO1_IO03 = 0X5;
    // gpio mux function
    GPIO1_GDIR = 0X00000008;
    GPIO1_DR = 0X00000000;

}

void led_on(void)
{
    GPIO1_DR &= ~(1<<3);
}

void led_off(void)
{
    GPIO1_DR |= 1<<3;
}

void delay_short(volatile unsigned int n)
{
    while(n--) { }
}

void delay(volatile unsigned int n)
{
    while(n--) {
        delay_short(0X7FF);
    }
}

int main(void) 
{
    clock_enable();
    led_init();

    while(1) {
        led_on();
        delay(500);

        led_off();
        delay(500);
    }
}