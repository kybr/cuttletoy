#include <stdio.h>
#include <string.h>

int main() {
    //char str[] = "你好，世界！";
    char str[] = "\u2565";
    printf("%s\n", str);
    printf("Length: %zu\n", strlen(str)); // Length in bytes, not characters
    return 0;
}
