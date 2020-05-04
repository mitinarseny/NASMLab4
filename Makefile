.PHONY: build
build: lab4

lab4: main.c
	gcc --std=c11 -Wall -O0 -o $@ -g $^

.PHONY: asm
asm: main.c
	$(CC) $(CFLAGS) -S -o /dev/stdout $^ | less
