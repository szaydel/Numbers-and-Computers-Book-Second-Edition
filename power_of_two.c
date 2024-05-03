#include <stdio.h>

int is_power_of_two(int n) {
    return (n == 0) ? 0
                    : (n & (n-1)) == 0;
}

int main() {
    printf("Is %d a power of two? %d\n",   0, is_power_of_two(0));
    printf("Is %d a power of two? %d\n",  64, is_power_of_two(64));
    printf("Is %d a power of two? %d\n", 127, is_power_of_two(127));
    printf("Is %d a power of two? %d\n", 8192, is_power_of_two(8192));
}

