#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX 100

int main() {
    char donnees[100];
    unsigned char hashes[MAX][SHA256_DIGEST_LENGTH];
    int n, i;

    printf("Nombre de donnees : ");
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Nombre invalide !\n");
        return 1;
    }

    // 1. Calculer le hash de chaque donnee
    for (i = 0; i < n; i++) {
        printf("Donnee %d : ", i + 1);
        scanf("%99s", donnees);

        SHA256((unsigned char *)donnees,
               strlen(donnees), hashes[i]);
    }

    // 2. Construire l'arbre de Merkle
    while (n > 1) {
        int nouveauN = 0;

        for (i = 0; i < n; i += 2) {
            unsigned char combinaison[64];

            memcpy(combinaison, hashes[i], 32);

            if (i + 1 < n) {
                memcpy(combinaison + 32, hashes[i + 1], 32);
            } else {
                memcpy(combinaison + 32, hashes[i], 32);
            }

            SHA256(combinaison, 64, hashes[nouveauN]);
            nouveauN++;
        }

        n = nouveauN;
    }

    // 3. Afficher la racine de Merkle
    printf("\nRacine de Merkle : ");

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hashes[0][i]);
    }

    printf("\n");

    return 0;
}

