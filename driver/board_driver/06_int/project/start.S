/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 start.s
作者	   : 左忠凯
版本	   : V1.0
描述	   : ZERO-I.MX6UL/I.MX6ULL开发板启动文件，完成C环境初始化，
		 C环境初始化完成以后跳转到C代码。
其他	   : 无
日志	   : 初版 2019/1/3 左忠凯修改
**************************************************************/

.global _start  		/* 全局标号 */
#if 1
.global _bss_start
_bss_start:
	.word __bss_start

.global _bss_end
_bss_end:
	.word __bss_end
#endif

/*
 * 描述：	_start函数，程序从此函数开始执行，此函数主要功能是设置C
 *		 运行环境。
 */
_start:

	ldr pc, =Reset_Handler
	ldr pc, =Undefined_Handler
	ldr pc, =SVC_Handler
	ldr pc, =PreAbort_Handler
	ldr pc, =DataAbort_Handler
	ldr pc, =NotUsed_Handler
	ldr pc, =IRQ_Handler
	ldr pc, =FIQ_Handler

	/* reset int service function */
Reset_Handler:
	/* 
		close I cache D cache and MMU
		1.modify the SCTLR reg with the method:read-change-write	
	*/
	MRC p15, 0, r0, c1, c0, 0  // read SCTLR reg to r0
	bic r0, r0, #(1 << 12)	   // close I Cache
	bic r0, r0, #(1 << 11)     // close branch prediction
	bic r0, r0, #(1 << 2) 	   // close D Cache
	bic r0, r0, #(1 << 1)      // close alignment
	bic r0, r0, #(1 << 0)  	   // close MMU
	MCR p15, 0, r0, c1, c0, 0 // write SCTLR reg from r0

	/* set int vector offset */
	ldr r0, =0x87800000
	dsb							// async 
	isb
	MCR p15, 0, r0, c12, c0, 0 // set VBAR = 0x87800000
	dsb
	isb

#if 1
	/* clear bss segment */
	ldr r0, _bss_start
	ldr r1, _bss_end
	mov r2, #0

bss_loop:
	stmia r0!, {r2}
	cmp r0, r1
	ble bss_loop
#endif

	/* 进入IRQ模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x12 	/* r0或上0x13,表示使用IRQ模式					*/
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中 				*/
	ldr sp, =0x80600000	/* diff from SVC设置栈指针			 */

	/* 进入USR模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x1f 	/* r0或上0x13,表示使用SYS模式					*/
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中 				*/
	ldr sp, =0x80400000	/* 设置栈指针			 */

	/* 进入SVC模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x13 	/* r0或上0x13,表示使用SVC模式					*/
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中 				*/
	ldr sp, =0x80200000	/* 设置栈指针			 */
	cpsie i
	b main				/* 跳转到main函数 		 */

	/* Undefined int service function */
Undefined_Handler:
	ldr r0, =Undefined_Handler
	bx r0

	/* SVC int service funtion */
SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0

	/* PreAbort int service function */
PreAbort_Handler:
	ldr r0, =PreAbort_Handler
	bx r0

	/* DataAbort int service function */
DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0

	/* NotUsed */
NotUsed_Handler:
	ldr r0, =NotUsed_Handler
	bx r0

	/* IRQ int service function */
IRQ_Handler:
	push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/							
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc */

	/* FIQ int service function */
FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx r0


