#include <stdio.h>
#include <stdlib.h>

void c(int8_t A[16]) {
    int16_t sm = 0;
    int8_t count = 0;
    int8_t B[16];
    int8_t C[16];

    for (int8_t i = 0; i < 16; i++) {
        if (A[i] <= -8)
            continue;
        sm += abs(A[i]);
        B[count] = A[i];
        C[count] = i;
        count++;
    }

    printf("COUNT: %d\n\t\tSUM: %d\n\t\tB: [", count, sm);
    for (int i = 0; i < count; i++)
        printf("%4d ", B[i]);
    printf("]\n\t\tC: [");
    for (int i = 0; i < count; i++)
        printf("%4d ", C[i]);
    printf("]\n");
}

void f(int8_t A[16]) {
    int16_t sm = 0;
    int8_t count = 0;
    int8_t B[16] = {0};
    int8_t C[16] = {0};
   
    __asm__ (
        "xor %[sm], %[sm]\n\t"       // sum <- 0
        "xor %[count], %[count]\n" // count <- 0
        "1:\n\t"                     // loop: iterate over A
        "mov (%[A]), %%al\n\t"       // AL <- A[i]
        "cmp $-8, %%al\n\t"          // if (A[i] <= -8)
        "jle 2f\n\t"                 // continue
        "mov %%al, (%[B])\n\t"
        "mov %%cl, (%[C])\n\t"
        "cbw\n\t"                    // AL -> AX
        "mov %%ax, %%dx\n\t"         // DX <- AX               -|
        "sar $31, %%dx\n\t"          // DX <- sign bit of A[i]  |
        "xor %%dx, %%ax\n\t"         // AX <- AX xor DX         |--> abs(A[i])
        "sub %%dx, %%ax\n\t"         // AX <- DX - AX          -|
        "add %%ax, %[sm]\n\t"        // sum += abs(A[i])
        "inc %[count]\n\t"             // count += 1
        "inc %[B]\n\t"
        "inc %[C]\n"
        "2:\n\t"
        "inc %[A]\n\t"              // i += 1
        "loop 1b\n"                    //   goto 1
    : [sm] "=b" (sm),
    [count] "=r" (count)
    : [size] "c" (16),
    [A] "S" (A),
    [B] "D" (B),
    [C] "r" (C)
    : "ax", "dx"
    );
    
    printf("COUNT: %d\n\t\tSUM: %d\n\t\tB: [", count, sm);
    for (int i = 0; i < count; i++)
        printf("%4d ", B[i]);
    printf("]\n\t\tC: [");
    for (int i = 0; i < count; i++)
        printf("%4d ", C[i]);
    printf("]\n\t\t");
}

int main() {
    int8_t AA[][16] = {
        {0},
        {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127},
        {-20,  20, -17,  17,   8,  -6,   0,   0,   1,   4, -31,  32,   6,   5,  10,   0},
        {-8,   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  20,  -1,  -1,  -1,  -1,  -2},
        {-8, 1}
    };
    for (int i = 0; i < sizeof(AA) / sizeof(*AA); i++) {
        printf("#%d:\n\tA: [", i);
        for (int j = 0; j < sizeof (*AA) / sizeof (**AA); j++)
            printf("%4d ", AA[i][j]);
        printf("]\n\tC:\n\t\t");
        c(AA[i]);
        printf("\tASM:\n\t\t");
        f(AA[i]);
    }
}
