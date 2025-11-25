#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Function to find gcd
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to perform modular exponentiation
long long int mod_pow(long long int base, long long int exp, long long int mod) {
    long long int result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;

        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    // Two prime numbers (small, for demo)
    int p = 61;
    int q = 53;
    int n = p * q; // modulus
    int phi = (p - 1) * (q - 1);

    // Choose e
    int e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1)
            break;
        else
            e++;
    }

    // Choose d (multiplicative inverse of e mod phi)
    int d = 0;
    int k = 1;
    while (1) {
        k = k + phi;
        if (k % e == 0) {
            d = k / e;
            break;
        }
    }

    char msg[100];
    printf("Enter the plain text: ");
    fgets(msg, sizeof(msg), stdin);
    msg[strcspn(msg, "\n")] = 0; // remove newline

    printf("\nEncrypting String: %s\n", msg);

    printf("String in Bytes: ");
    for (int i = 0; i < strlen(msg); i++) {
        printf("%d ", msg[i]);
    }
    printf("\n");

    // Encryption
    long long int encrypted[100];
    printf("Encrypted: ");
    for (int i = 0; i < strlen(msg); i++) {
        encrypted[i] = mod_pow(msg[i], e, n);
        printf("%lld ", encrypted[i]);
    }
    printf("\n");

    // Decryption
    char decrypted[100];
    printf("Decrypted Bytes: ");
    for (int i = 0; i < strlen(msg); i++) {
        decrypted[i] = mod_pow(encrypted[i], d, n);
        printf("%d ", decrypted[i]);
    }

    decrypted[strlen(msg)] = '\0';

    printf("\nDecrypted String: %s\n", decrypted);

    return 0;
}
