qemu_debug:
	../qemu/build/qemu-system-arm -machine s32k3x8evb -cpu cortex-m7 -kernel ../kernel/Output/demo.elf -monitor none -nographic -serial stdio -s -S

gdb_start:
	gdb-multiarch ../kernel/Output/demo.elf

