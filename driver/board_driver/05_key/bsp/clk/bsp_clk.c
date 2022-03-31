#include "bsp_clk.h"
void clk_enable(void)
{
    CCM->CCGR0 = 0XFFFFFFFF;
    CCM->CCGR1 = 0XFFFFFFFF;
    CCM->CCGR2 = 0XFFFFFFFF;
    CCM->CCGR3 = 0XFFFFFFFF;
    CCM->CCGR4 = 0XFFFFFFFF;
    CCM->CCGR5 = 0XFFFFFFFF;
    CCM->CCGR6 = 0XFFFFFFFF;
}

void imx6u_clckinit(void)
{
    unsigned int reg = 0;
    if (((CCM->CCSR >> 2) & 0x1) == 0) {
        CCM->CCSR |= (1 << 2) ;
        CCM->CCSR &= ~(1 << 8);
    }  
    CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0x7f);
    CCM->CACRR = 1;

    CCM->CCSR &= (1 << 2); // set pll1_sw_clock = pll1_main_clock

    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3F3F3F3F);
    reg |= (32 << 24);                                   // PFD3 = 297M 32 0x9A
    reg |= (24 << 16);                                   // PFD2 = 396M  24 0x14
    reg |= (16 <<  8);                                   // PFD1 = 594M  16 0xe
    reg |= (27 <<  0);                                   // PFD0 = 352Ms 27 0x17
    
    CCM_ANALOG->PFD_528 = reg;

    reg = 0;
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0x3F3F3F3F);
    reg |= (19 << 24);                                   // PFD3 = 297M 32 0x9A
    reg |= (17 << 16);                                   // PFD2 = 396M  24 0x14
    reg |= (16 <<  8);                                   // PFD1 = 594M  16 0xe
    reg |= (12 <<  0);                                   // PFD0 = 352Ms 27 0x17

    CCM_ANALOG->PFD_480 = reg;

    // set AHB_CLK_ROOT = 132M 
    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= (1 << 18); /* set PRE_PERIPH_CLK_SEL = 01 */
    CCM->CBCDR &= ~(1 << 25);
    while(CCM->CDHIPR & (1 << 5)); // wait handshake signal
#if 0
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10);
    while (CCM->CDHIPR & (1 << 1)); // wait handshake signal
#endif

    // set PERCLK_CLK_ROOT = 66M
    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(0x3f << 0);
    CCM->CBCDR  &= ~(3 << 8);
    CCM->CBCDR  |= (1 << 8);
}
