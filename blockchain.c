
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_BLOCKS 5
#define DATA_SIZE 256
#define HASH_SIZE 65
#define DIFFICULTE 3


// STRUCTURE D'UN BLOC
typedef struct {
    int index;
    char data[DATA_SIZE];

    char previousHash[HASH_SIZE];
    char hash[HASH_SIZE];

    char validateur[30];

    unsigned long nonce;
} Block;


// STRUCTURE DE LA BLOCKCHAIN
typedef struct {
    Block blocks[MAX_BLOCKS];
    int count;
    int consensus;
} Blockchain;



// STRUCTURE D'UN VALIDATEUR
typedef struct {
    char nom[30];
    int stake;
} Validateur;


// CALCULER SHA-256
void calculerHash(const char *texte, char hash[HASH_SIZE]) {

    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256(
        (const unsigned char *)texte,
        strlen(texte),
        digest
    );

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + i * 2, "%02x", digest[i]);
    }

    hash[64] = '\0';
}



// CALCULER LE HASH D'UN BLOC
void calculerHashBloc(Block *block) {

    char texte[1024];

    snprintf(
        texte,
        sizeof(texte),
        "%d|%s|%s|%s|%lu",
        block->index,
        block->data,
        block->previousHash,
        block->validateur,
        block->nonce
    );

    calculerHash(texte, block->hash);
}


// VERIFIER LA DIFFICULTE
int hashValide(const char *hash) {

    for (int i = 0; i < DIFFICULTE; i++) {

        if (hash[i] != '0') {
            return 0;
        }
    }

    return 1;
}


// PROOF OF WORK
void proofOfWork(Block *block) {

    block->nonce = 0;

    do {

        calculerHashBloc(block);

        if (hashValide(block->hash)) {
            break;
        }

        block->nonce++;

    } while (1);
}


// AJOUTER UN BLOC AVEC PoW
void ajouterBlocPoW(Blockchain *chain, const char *data) {

    if (chain->count >= MAX_BLOCKS) {
        printf("Blockchain pleine !\n");
        return;
    }

    Block *block = &chain->blocks[chain->count];

    block->index = chain->count;

    strcpy(block->data, data);

    // Aucun validateur en PoW
    strcpy(block->validateur, "Aucun");

    // Définir le hash précédent
    if (chain->count == 0) {

        strcpy(block->previousHash, "0");

    } else {

        strcpy(
            block->previousHash,
            chain->blocks[chain->count - 1].hash
        );
    }

    // Miner le bloc
    proofOfWork(block);

    chain->count++;

    printf("\n===== BLOC %d (PoW) =====\n", block->index);
    printf("Data          : %s\n", block->data);
    printf("Previous Hash : %s\n", block->previousHash);
    printf("Nonce         : %lu\n", block->nonce);
    printf("Hash          : %s\n", block->hash);
}


// PROOF OF STAKE
// Selectionner un validateur proportionnellement a son stake
int choisirValidateur(Validateur validateurs[], int n) {

    int totalStake = 0;

    for (int i = 0; i < n; i++) {

        if (validateurs[i].stake < 0) {
            return -1;
        }

        totalStake += validateurs[i].stake;
    }

    if (totalStake <= 0) {
        return -1;
    }

    // Tirage aleatoire entre 1 et totalStake
    int tirage = rand() % totalStake + 1;

    int cumul = 0;

    for (int i = 0; i < n; i++) {

        cumul += validateurs[i].stake;

        if (tirage <= cumul) {
            return i;
        }
    }

    return -1;
}


// AJOUTER UN BLOC AVEC PoS
void ajouterBlocPoS(
    Blockchain *chain,
    const char *data,
    Validateur validateurs[],
    int n
) {

    if (chain->count >= MAX_BLOCKS) {
        printf("Blockchain pleine !\n");
        return;
    }

    Block *block = &chain->blocks[chain->count];

    block->index = chain->count;

    strcpy(block->data, data);

    // Définir le hash précédent
    if (chain->count == 0) {

        strcpy(block->previousHash, "0");

    } else {

        strcpy(
            block->previousHash,
            chain->blocks[chain->count - 1].hash
        );
    }

    // Selectionner le validateur
    int choix = choisirValidateur(validateurs, n);

    if (choix == -1) {
        printf("Erreur : aucun validateur disponible !\n");
        return;
    }

    // Enregistrer le validateur dans le bloc
    strcpy(block->validateur, validateurs[choix].nom);

    // Pas de minage en PoS
    block->nonce = 0;

    // Calculer directement le hash
    calculerHashBloc(block);

    chain->count++;

    printf("\n===== BLOC %d (PoS) =====\n", block->index);
    printf("Data          : %s\n", block->data);
    printf("Validateur    : %s\n", block->validateur);
    printf("Stake         : %d coins\n", validateurs[choix].stake);
    printf("Previous Hash : %s\n", block->previousHash);
    printf("Nonce         : %lu\n", block->nonce);
    printf("Hash          : %s\n", block->hash);
}


// VERIFIER LA BLOCKCHAIN
int verifierBlockchain(const Blockchain *chain) {

    printf("\n===== VERIFICATION DE LA BLOCKCHAIN =====\n");

    for (int i = 0; i < chain->count; i++) {

        const Block *block = &chain->blocks[i];

        // 1. Verifier l'index
        if (block->index != i) {
            printf("Index incorrect au bloc %d\n", i);
            return 0;
        }

        // 2. Recalculer le hash
        Block copie = *block;

        calculerHashBloc(&copie);

        if (strcmp(block->hash, copie.hash) != 0) {

            printf("Hash incorrect au bloc %d\n", i);
            return 0;
        }

        // 3. Verifier le hash precedent
        if (i == 0) {

            if (strcmp(block->previousHash, "0") != 0) {
                printf("Hash precedent incorrect au bloc genesis\n");
                return 0;
            }

        } else {

            if (strcmp(
                    block->previousHash,
                    chain->blocks[i - 1].hash
                ) != 0) {

                printf("Lien incorrect au bloc %d\n", i);
                return 0;
            }
        }

        // 4. Verifier la difficulte en PoW
        if (chain->consensus == 1) {

            if (!hashValide(block->hash)) {
                printf("Proof of Work invalide au bloc %d\n", i);
                return 0;
            }
        }
    }

    printf("Blockchain valide !\n");

    return 1;
}



// AFFICHER LA BLOCKCHAIN
void afficherBlockchain(const Blockchain *chain) {

    printf("\n\n========== BLOCKCHAIN ==========\n");

    for (int i = 0; i < chain->count; i++) {

        const Block *block = &chain->blocks[i];

        printf("\n-------- BLOC %d --------\n", block->index);

        printf("Data          : %s\n", block->data);

        printf("Previous Hash : %s\n", block->previousHash);

        printf("Validateur    : %s\n", block->validateur);

        printf("Nonce         : %lu\n", block->nonce);

        printf("Hash          : %s\n", block->hash);
    }
}



int main(void) {

    srand((unsigned int)time(NULL));

    Blockchain chain = {0};

    // Définition des validateurs
    Validateur validateurs[] = {
        {"Alice", 10},
        {"Bob", 30},
        {"Charlie", 60}
    };

    int nombreValidateurs = 3;

    int choix;

    printf("=================================\n");
    printf("      BLOCKCHAIN FROM SCRATCH\n");
    printf("=================================\n");

    printf("\n1. Proof of Work\n");
    printf("2. Proof of Stake\n");

    printf("\nChoisir le consensus : ");
    scanf("%d", &choix);


    // PROOF OF WORK
    if (choix == 1) {

        chain.consensus = 1;

        printf("\n===== PROOF OF WORK =====\n");

        ajouterBlocPoW(
            &chain,
            "Alice -> Bob : 10 coins"
        );

        ajouterBlocPoW(
            &chain,
            "Bob -> Charlie : 5 coins"
        );

        ajouterBlocPoW(
            &chain,
            "Charlie -> Alice : 2 coins"
        );
    }
    

    // PROOF OF STAKE
    else if (choix == 2) {

        chain.consensus = 2;

        printf("\n===== PROOF OF STAKE =====\n");

        ajouterBlocPoS(
            &chain,
            "Alice -> Bob : 10 coins",
            validateurs,
            nombreValidateurs
        );

        ajouterBlocPoS(
            &chain,
            "Bob -> Charlie : 5 coins",
            validateurs,
            nombreValidateurs
        );

        ajouterBlocPoS(
            &chain,
            "Charlie -> Alice : 2 coins",
            validateurs,
            nombreValidateurs
        );
    }

    else {

        printf("\nChoix invalide !\n");
        return 1;
    }

    // Afficher la blockchain
    afficherBlockchain(&chain);

    // Verifier la blockchain
    verifierBlockchain(&chain);

    return 0;
}
