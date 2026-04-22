#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *data;
    int  len;
} Slice;

static int cmp(const void *a, const void *b)
{
    int x = *(int *)a, y = *(int *)b;
    return (x > y) - (x < y);
}

static void *sort_slice(void *arg)
{
    Slice *s = arg;
    qsort(s->data, s->len, sizeof(int), cmp);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: task1 <num_threads>\n");
        return 1;
    }

    int nthreads = atoi(argv[1]);
    if (nthreads < 1) nthreads = 1;

    int *arr = NULL, n = 0, cap = 0, x;
    while (scanf("%d", &x) == 1) {
        if (n == cap)
            arr = realloc(arr, (cap = cap ? cap * 2 : 16) * sizeof(int));
        arr[n++] = x;
    }
    if (n == 0) { free(arr); return 0; }
    if (nthreads > n) nthreads = n;

    Slice     *slices  = malloc(nthreads * sizeof(Slice));
    pthread_t *threads = malloc(nthreads * sizeof(pthread_t));

    int base = n / nthreads, rem = n % nthreads, off = 0;
    for (int i = 0; i < nthreads; i++) {
        slices[i].data = arr + off;
        slices[i].len  = base + (i < rem ? 1 : 0);
        off += slices[i].len;
        pthread_create(&threads[i], NULL, sort_slice, &slices[i]);
    }
    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

    /* k-way merge */
    int *result = malloc(n * sizeof(int));
    int *idx    = calloc(nthreads, sizeof(int));
    for (int i = 0; i < n; i++) {
        int best = -1;
        for (int j = 0; j < nthreads; j++) {
            if (idx[j] < slices[j].len)
                if (best == -1 || slices[j].data[idx[j]] < slices[best].data[idx[best]])
                    best = j;
        }
        result[i] = slices[best].data[idx[best]++];
    }
    free(idx);

    for (int i = 0; i < n; i++)
        printf("%s%d", i ? " " : "", result[i]);
    putchar('\n');

    free(result); free(slices); free(threads); free(arr);
    return 0;
}
