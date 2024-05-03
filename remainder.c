#include <stdio.h>

int main() {

    printf("%d mod %d = %d\n", -43, 2, -43 % 2);
    printf("%d mod %d = %d\n", 33, -7, 33 % -7);
    printf("%d mod %d = %d\n", -33, 7, -33 % 7);
    printf("%d mod %d = %d\n", -33, 5, -33 % 5);
    printf("%d div %d = %d\n", -33, 5, -33 / 5);
    printf("\n");
    printf("%d divmod %d = %d, %d\n", 33, 7, 33/7, 33%7);
    printf("%d divmod %d = %d, %d\n", -33, 7, -33/7, -33%7);
    printf("%d divmod %d = %d, %d\n", 33, -7, 33/-7, 33%-7);
    printf("%d divmod %d = %d, %d\n", -33, -7, -33/-7, -33%-7);
}

