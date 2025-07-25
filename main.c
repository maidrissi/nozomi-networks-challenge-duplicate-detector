#include <stdio.h>
#include "find_duplicates.h"

int main() {
    // Test input: Example of duplicates for printable ASCII characters or could use argc, argv
    char str[512] = {0};
    int idx = 0;
    for (int i = 32; i <= 126; i++) {
        str[idx++] = (char)i;
        str[idx++] = (char)i;
    }
    find_duplicates(str);
    return 0;
}