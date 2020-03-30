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

    printf("SUM: %d\n\t\tB: [", sm);
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
    int8_t B[16];
    int8_t C[16];

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
    : [A] "S" (A),
    [B] "D" (B)
    : "ecx", "ax", "dx"
    );
    
    printf("SUM: %d\n\t\tB: [", sm);
    for (int i = 0; i < count; i++)
        printf("%4d ", B[i]);
    printf("]\n\t\tC: [");
    for (int i = 0; i < count; i++)
        printf("%4d ", C[i]);
    printf("]\n");
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
