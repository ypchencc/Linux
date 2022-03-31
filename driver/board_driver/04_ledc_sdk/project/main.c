/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验4 使用NXP提供的I.MX6ULL官方IAR SDK包开发
其他	   : 前面其他所有实验中，寄存器定义都是我们自己手写的，但是I.MX6U
		 的寄存器有很多，全部自己写太费时间，而且没意义。NXP官方提供了
		 针对I.MX6ULL的SDK开发包，是基于IAR环境的，这个SDK包里面已经提
		 供了I.MX6ULL所有相关寄存器定义，虽然是针对I.MX6ULL编写的，但是同样
		 适用于I.MX6UL。本节我们就将相关的寄存器定义文件移植到Linux环境下，
		 要移植的文件有：
		 fsl_common.h
		 fsl_iomuxc.h
		 MCIMX6Y2.h
		 自定义文件 cc.h
日志	   : 初版V1.0 2019/1/3 左忠凯创建
**************************************************************/


#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"

int main(void)
{
	clk_enable();
	led_init();

	while (1) {
		led_switch(LED0, ON);
		delay(500);

		led_switch(LED0, OFF);
		delay(500);
	}

	return 0;
}