#include <stdio.h>
#include <stdlib.h>

int main() {
    int bucket_size, output_rate, nsec;
    int packets[100], i, p_remain = 0, drop = 0, mini;

    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);

    printf("Enter output rate: ");
    scanf("%d", &output_rate);

    printf("Enter number of seconds to simulate: ");
    scanf("%d", &nsec);

    // Random packets for each second
    for (i = 0; i < nsec; i++) {
        packets[i] = (rand() % 9 + 1) * 10; // random 10–90
    }

    printf("\nSeconds | PacketsReceived | PacketsSent | PacketsLeft | PacketsDropped\n");
    printf("-----------------------------------------------------------------------------\n");

    for (i = 0; i < nsec; i++) {
        p_remain += packets[i];

        if (p_remain > bucket_size) {
            drop = p_remain - bucket_size;
            p_remain = bucket_size;
        }

        mini = (p_remain < output_rate) ? p_remain : output_rate;

        printf(" %d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               i + 1, packets[i], mini, p_remain - mini, drop);

        p_remain -= mini;
        drop = 0;
    }

    // Drain remaining packets
    while (p_remain > 0) {
        mini = (p_remain < output_rate) ? p_remain : output_rate;

        printf(" *\t\t0\t\t%d\t\t%d\t\t%d\n",
               mini, p_remain - mini, drop);

        p_remain -= mini;
    }

    return 0;
}
