#include <stdio.h>

int main(int argc, char** argv) {
int x = 1;
int y = 2;
int z = x + y;
int a = 3 + 4;
printf("%d\n", a + z);
int b = z + a;
printf("%d\n", b);
}
