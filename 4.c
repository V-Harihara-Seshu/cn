#include <stdio.h>
#define MAX 200   // Maximum bits size

// Function to perform division
void divide(int divisor[], int rem[], int divisor_bits, int rem_length) {
    int cur = 0;

    while (1) {
        for (int i = 0; i < divisor_bits; i++) {
            rem[cur + i] = rem[cur + i] ^ divisor[i];   // XOR division
        }

        while (rem[cur] == 0 && cur != rem_length - 1) {
            cur++;
        }

        if ((rem_length - cur) < divisor_bits)
            break;
    }
}

int main() {
    int data[MAX], div[MAX], divisor[MAX], rem[MAX], crc[MAX];
    int data_bits, divisor_bits, tot_length;

    // Input number of data bits
    printf("Enter number of data bits: ");
    scanf("%d", &data_bits);

    printf("Enter data bits:\n");
    for (int i = 0; i < data_bits; i++)
        scanf("%d", &data[i]);

    // CRC-CCITT polynomial = 10001000000100001 (17 bits)
    divisor_bits = 17;
    int fixed_divisor[17] = {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1};

    for (int i = 0; i < divisor_bits; i++)
        divisor[i] = fixed_divisor[i];

    // Total length = data + (divisor_bits - 1) zeros
    tot_length = data_bits + divisor_bits - 1;

    // Append data into div[]
    for (int i = 0; i < data_bits; i++)
        div[i] = data[i];
    for (int i = data_bits; i < tot_length; i++)
        div[i] = 0;

    // Copy dividend into remainder
    for (int j = 0; j < tot_length; j++)
        rem[j] = div[j];

    printf("Dividend (after appending 0's): ");
    for (int i = 0; i < tot_length; i++)
        printf("%d", div[i]);
    printf("\n");

    // Perform division
    divide(divisor, rem, divisor_bits, tot_length);

    // Generate CRC codeword
    for (int i = 0; i < tot_length; i++)
        crc[i] = div[i] ^ rem[i];

    printf("CRC code: ");
    for (int i = 0; i < tot_length; i++)
        printf("%d", crc[i]);
    printf("\n");

    // ----------- ERROR DETECTION ----------
    printf("Enter received codeword (%d bits):\n", tot_length);
    for (int i = 0; i < tot_length; i++)
        scanf("%d", &crc[i]);

    // Copy received codeword to remainder
    for (int j = 0; j < tot_length; j++)
        rem[j] = crc[j];

    divide(divisor, rem, divisor_bits, tot_length);

    int error = 0;
    for (int i = 0; i < tot_length; i++) {
        if (rem[i] != 0) {
            error = 1;
            break;
        }
    }

    if (error)
        printf("Error detected in received message.\n");
    else
        printf("No error detected.\n");

    printf("THANK YOU.\n");
    return 0;
}
