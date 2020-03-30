#include <stdio.h>
#include <stdlib.h>

int16_t c(int8_t A[16]) {
    int16_t sm = 0;

    for (int i = 0; i < 16; i++) {
        if (A[i] <= -8)
            continue;
        sm += abs(A[i]);
    }

    return sm;
}

int16_t f(int8_t A[16]) {
    int16_t sm = 0;
    __asm__ (
        "xor %[sm], %[sm]\n\t"
        "mov $16, %%ecx\n"
        "1:\n\t"
        "mov (%[A]), %%al\n\t"
        "cmp $-8, %%al\n\t"
        "jle 2f\n\t"
        "cbw\n\t"
        "mov %%ax, %%dx\n\t"
        "sar $31, %%dx\n\t"
        "xor %%dx, %%ax\n\t"
        "sub %%dx, %%ax\n\t"
        "add %%ax, %[sm]\n\t"
        "2:\n\t"
        "inc %[A]\n"
        "loop 1b"
    : [sm] "=b" (sm)
    : [A] "S" (A)
    : "ecx", "ax", "dx"
    );
     return sm;
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
        printf("]\n\tSUM: %d (c: %d)\n", f(AA[i]), c(AA[i]));
    }
}
