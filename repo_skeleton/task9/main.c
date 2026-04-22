#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *arr = NULL, n = 0, cap = 0, x;
    while (scanf("%d", &x) == 1) {
        if (n == cap)
            arr = realloc(arr, (cap = cap ? cap * 2 : 16) * sizeof(int));
        arr[n++] = x;
    }
    if (n == 0) { free(arr); return 0; }

    int bs = 0, bl = 1, bsum = arr[0];
    int cs = 0, cl = 1, csum = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] >= arr[i - 1]) {
            cl++;
            csum += arr[i];
        } else {
            cs = i; cl = 1; csum = arr[i];
        }
        if (cl > bl || (cl == bl && csum > bsum)) {
            bs = cs; bl = cl; bsum = csum;
        }
    }

    for (int i = 0; i < bl; i++)
        printf("%s%d", i ? " " : "", arr[bs + i]);
    putchar('\n');

    free(arr);
    return 0;
}
