#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "imx6ul.h"

enum Keyvalue
{
    KEY_NONE = 0,
    KEY_VALUE,
};

void key_init(void);
int key_getvalue(void);
#endif // !__BSP_KEY_H