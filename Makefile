.PHONY: build
build: lab4

lab4: main.c
	gcc --std=c11 -Wall -O3 -o $@ -g $^