#include <stdio.h>

int16_t c(int8_t A[16]) {
    int16_t sm = 0;

    for (int i = 0; i < 16; i++)
        sm += A[i];

    return sm;
}

int16_t f(int8_t A[16]) {
    int16_t sm = 0;
    __asm__ (
        "mov $16, %%ecx\n"
        "1:\n\t"
        "movb (%[A]), %%al\n\t"
        "cbw\n\t"
        "add %%ax, %[sm]\n\t"
        "add $1, %[A]\n\t"
        "loop 1b"
    : [sm] "=b" (sm)
    : [A] "S" (A),
    "b" (sm)
    : "ecx"
    );
     return sm;
}

int main() {
    int8_t AA[][16] = {
        {0},
        {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127},
        {-20,  20, -17,  17,   8,  -6,   0,   0,   1,   4, -31,  32,   6,   5,  10,   0},
    };
    for (int i = 0; i < sizeof(AA) / sizeof(*AA); i++) {
        printf("#%d:\n\tA: [", i);
        for (int j = 0; j < sizeof (*AA) / sizeof (**AA); j++)
            printf("%3d,", AA[i][j]);
        printf("]\n\tSUM: %d (c: %d)\n", f(AA[i]), c(AA[i]));
    }
}
