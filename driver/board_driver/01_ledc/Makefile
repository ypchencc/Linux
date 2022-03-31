obj = start.o main.o 

ledc.bin: $(obj)
	arm-linux-gnueabihf-ld -Ttext 0X87800000 -o ledc.elf $^
	arm-linux-gnueabihf-objcopy -O binary -S ledc.elf $@
	arm-linux-gnueabihf-objdump -D -m arm ledc.elf > led.dis

%.o:%.s
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<
%.o:%.S
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<
%.o:%.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -o $@ $<

clean:
	-rm -rf *.o ledc.elf ledc.dis ledc.bin