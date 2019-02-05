# clear screen
clear

CC="/usr/local/i386elfgcc/bin/i386-elf-gcc"

# default variables
CFLAGS="-m32 -c -g -O2 -W -Wall -Wextra -Werror "\
"-ffreestanding -std=gnu99 -fno-builtin -Ikernel/include -Ilibc/include"

CFLAGS_LIBC="-m32 -c -g -O2 -W -Wall -Wextra -Werror "\
"-ffreestanding -std=gnu99 -fno-builtin -Ikernel/include -Ilibc/include"

OBJ=""

# compile libc
$CC $CFLAGS libc/math.c -o math.o
$CC $CFLAGS libc/stdlib.c -o stdlib.o
$CC $CFLAGS libc/string.c -o string.o
$CC $CFLAGS libc/ctype.c -o ctype.o
ar rcs libc.a math.o stdlib.o string.o ctype.o

# compile kernel
nasm -f elf32 kernel/kentry.asm -o kentry.o
nasm -f elf32 kernel/interrupt.asm -o interrupt.o
nasm -f elf32 kernel/x86.asm -o x86.o
$CC $CFLAGS kernel/kernel.c -o kernel.o
$CC $CFLAGS kernel/video.c -o video.o
$CC $CFLAGS kernel/gdt.c -o gdt.o
$CC $CFLAGS kernel/idt.c -o idt.o
$CC $CFLAGS kernel/pic.c -o pic.o
$CC $CFLAGS kernel/isr.c -o isr.o
$CC $CFLAGS kernel/timer.c -o timer.o
$CC $CFLAGS kernel/vbe.c -o vbe.o
$CC $CFLAGS kernel/gui.c -o gui.o
$CC $CFLAGS kernel/keyboard.c -o keyboard.o
$CC $CFLAGS kernel/shell.c -o shell.o
ld -g -m elf_i386 -T kernel/link.ld --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_ -o kernel.bin kentry.o kernel.o video.o gdt.o idt.o pic.o isr.o timer.o x86.o interrupt.o vbe.o gui.o keyboard.o shell.o libc.a

# create ISO
cp kernel.bin iso/boot/kernel.bin
grub-mkrescue -o GekonOS.iso iso/

# clean up
rm -rf *.o
rm -rf libc.a
rm -rf kernel.bin

# run OS
qemu-system-i386 -m 32 -cdrom GekonOS.iso
