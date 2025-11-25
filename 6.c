#include <stdio.h>

int main() {
    int n, i, j, k, count;
    int dmat[10][10], dist[10][10], via[10][10];

    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    printf("Enter the cost matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &dmat[i][j]);
            if (i == j)
                dmat[i][j] = 0;   // cost to itself is 0

            dist[i][j] = dmat[i][j];
            via[i][j] = j;       // initially direct connection
        }
    }

    // Distance Vector Algorithm
    do {
        count = 0;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (dist[i][j] > dmat[i][k] + dist[k][j]) {
                        dist[i][j] = dmat[i][k] + dist[k][j];
                        via[i][j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);

    // Output routing table
    for (i = 0; i < n; i++) {
        printf("\nState value for router %d:\n", i);
        for (j = 0; j < n; j++) {
            printf("To %d - Via %d   Distance = %d\n",
                   j, via[i][j], dist[i][j]);
        }
    }

    return 0;
}
