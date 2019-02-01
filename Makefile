CC = gcc
AS = nasm

CFLAGS = -m32 -O2 -nostdlib -nostdinc -fno-builtin -Wall -Wextra -Werror

clean:
	rm -f *.o