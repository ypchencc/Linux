#include "imx6ul.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_clk.h"

int main()
{
    int keyvalue = 0;
    unsigned char led_state = OFF;

    clk_enable();
    key_init();
    led_init();

    while (1) {
        keyvalue = key_getvalue();

        if (keyvalue) {
            switch (keyvalue)
            {
                case KEY_VALUE:
                    led_state = !led_state;
                    led_switch(LED0, led_state);
                    break;
            }
        }
    }
    return 0;
}