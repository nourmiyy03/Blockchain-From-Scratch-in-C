#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_BLOCKS 4
#define DATA_SIZE 256
#define HASH_SIZE 65

typedef struct {
    int index;
    char data[DATA_SIZE];
    char previousHash[HASH_SIZE];
    char hash[HASH_SIZE];
    unsigned long long nonce;
} Block;

typedef struct {
    Block blocks[MAX_BLOCKS];
    int count;
} Blockchain;

// Calculer le hash SHA-256 d'une chaine de caracteres
void calculateHash(const char *input, char output[HASH_SIZE]) {
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256((const unsigned char *)input, strlen(input), digest);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + i * 2, "%02x", digest[i]);
    }

    output[64] = '\0';
}

// Verifier si le hash commence par difficulty zeros
int validHash(const char *hash, int difficulty) {
    for (int i = 0; i < difficulty; i++) {
        if (hash[i] != '0') {
            return 0;
        }
    }
    return 1;
}

// Calculer le hash d'un bloc
void calculateBlockHash(Block *block) {
    char input[512];

    snprintf(input, sizeof(input), "%d%s%s%llu",
             block->index,
             block->data,
             block->previousHash,
             block->nonce);

    calculateHash(input, block->hash);
}

// Proof of Work : chercher un nonce valide
unsigned long long proofOfWork(Block *block, int difficulty) {
    block->nonce = 0;

    do {
        calculateBlockHash(block);

        if (validHash(block->hash, difficulty)) {
            break;
        }

        block->nonce++;
    } while (1);

    return block->nonce;
}

// Ajouter un bloc a la blockchain
void addBlock(Blockchain *chain, const char *data, int difficulty) {
    if (chain->count >= MAX_BLOCKS) {
        printf("Blockchain pleine !\n");
        return;
    }

    Block *block = &chain->blocks[chain->count];

    block->index = chain->count;
    snprintf(block->data, DATA_SIZE, "%s", data);

    if (chain->count == 0) {
        strcpy(block->previousHash, "0");
    } else {
        strcpy(block->previousHash,
               chain->blocks[chain->count - 1].hash);
    }

    clock_t start = clock();

    proofOfWork(block, difficulty);

    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n========== BLOC %d ==========\n", block->index);
    printf("Donnees       : %s\n", block->data);
    printf("Hash precedent: %s\n", block->previousHash);
    printf("Nonce trouve  : %llu\n", block->nonce);
    printf("Hash          : %s\n", block->hash);
    printf("Difficulte    : %d zero(s)\n", difficulty);
    printf("Temps         : %.6f seconde(s)\n", elapsed);

    chain->count++;
}

// Afficher toute la blockchain
void displayBlockchain(const Blockchain *chain) {
    printf("\n\n========== BLOCKCHAIN ==========\n");

    for (int i = 0; i < chain->count; i++) {
        const Block *block = &chain->blocks[i];

        printf("\nBloc %d\n", block->index);
        printf("Donnees        : %s\n", block->data);
        printf("Hash precedent: %s\n", block->previousHash);
        printf("Hash           : %s\n", block->hash);
        printf("Nonce          : %llu\n", block->nonce);
    }
}

int main(void) {
    Blockchain chain = {0};

    int difficulties[] = {1, 2, 3, 4};
    int numberOfTests = sizeof(difficulties) / sizeof(difficulties[0]);

    printf("=== IMPLEMENTATION DU PROOF OF WORK ===\n");

    for (int i = 0; i < numberOfTests; i++) {
        char data[DATA_SIZE];

        snprintf(data, sizeof(data),
                 "Transaction pour difficulte %d",
                 difficulties[i]);

        addBlock(&chain, data, difficulties[i]);
    }

    displayBlockchain(&chain);

    return 0;
}
