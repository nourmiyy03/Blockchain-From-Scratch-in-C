
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define NB_VALIDATEURS 3
#define REPETITIONS 20
#define DIFFICULTE 3

typedef struct {
    char nom[30];
    int stake;
} Validateur;

// Calculer le hash SHA-256 d'une chaine
void calculerHash(const char *texte, char *hash) {
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256((const unsigned char *)texte, strlen(texte), digest);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + i * 2, "%02x", digest[i]);
    }
    hash[64] = '\0';
}

// Verifier si le hash commence par des zeros
int hashValide(const char *hash) {
    for (int i = 0; i < DIFFICULTE; i++) {
        if (hash[i] != '0')
            return 0;
    }
    return 1;
}

// PROOF OF WORK : chercher un nonce valide
unsigned long proofOfWork(const char *donnee, char *hashFinal) {
    char texte[256];
    unsigned long nonce = 0;

    do {
        sprintf(texte, "%s|%lu", donnee, nonce);
        calculerHash(texte, hashFinal);
        nonce++;
    } while (!hashValide(hashFinal));

    return nonce - 1;
}

// PROOF OF STAKE : selection proportionnelle a la mise
int proofOfStake(Validateur validateurs[], int n,
                 const char *donnee, char *hashFinal) {
    int miseTotale = 0;

    for (int i = 0; i < n; i++) {
        miseTotale += validateurs[i].stake;
    }

    if (miseTotale <= 0)
        return -1;

    // Tirage d'un nombre entre 1 et la mise totale
    int tirage = rand() % miseTotale + 1;
    int cumul = 0;

    for (int i = 0; i < n; i++) {
        cumul += validateurs[i].stake;

        if (tirage <= cumul) {
            // Le validateur est selectionne.
            // On calcule le hash du bloc une seule fois.
            calculerHash(donnee, hashFinal);
            return i;
        }
    }

    return -1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    Validateur validateurs[NB_VALIDATEURS] = {
        {"Alice", 10},
        {"Bob", 30},
        {"Charlie", 60}
    };

    const char *bloc = "Transaction: Alice -> Bob : 5 coins";
    char hash[65];
    char hashPoW[65];
    unsigned long nonce;
    int totalStake = 0;

    for (int i = 0; i < NB_VALIDATEURS; i++) {
        totalStake += validateurs[i].stake;
    }

    printf("===== PROOF OF STAKE =====\n");
    printf("Mise totale : %d coins\n", totalStake);

    // Exemple d'une validation PoS
    int choix = proofOfStake(
        validateurs, NB_VALIDATEURS, bloc, hash
    );

    if (choix >= 0) {
        printf("Validateur selectionne : %s\n",
               validateurs[choix].nom);
        printf("Sa mise : %d coins\n",
               validateurs[choix].stake);
        printf("Hash du bloc : %s\n", hash);
        printf("Bloc valide par le validateur selectionne.\n");
    } else {
        printf("Echec de la selection du validateur.\n");
        return 1;
    }

    printf("\n===== PROOF OF WORK =====\n");

    // Exemple d'une validation PoW
    nonce = proofOfWork(bloc, hashPoW);

    printf("Nonce trouve : %lu\n", nonce);
    printf("Hash du bloc : %s\n", hashPoW);
    printf("Bloc valide : %s\n",
           hashValide(hashPoW) ? "OUI" : "NON");

    // Comparaison des temps d'execution
    clock_t debut, fin;
    double tempsPoS, tempsPoW;

    debut = clock();

    for (int i = 0; i < REPETITIONS; i++) {
        proofOfStake(validateurs, NB_VALIDATEURS, bloc, hash);
    }

    fin = clock();
    tempsPoS = (double)(fin - debut) / CLOCKS_PER_SEC;

    debut = clock();

    for (int i = 0; i < REPETITIONS; i++) {
        proofOfWork(bloc, hashPoW);
    }

    fin = clock();
    tempsPoW = (double)(fin - debut) / CLOCKS_PER_SEC;

    printf("\n===== COMPARAISON =====\n");
    printf("Nombre de repetitions : %d\n", REPETITIONS);
    printf("Difficulte PoW : %d zero(s) au debut du hash\n",
           DIFFICULTE);
    printf("Temps total PoS : %.6f secondes\n", tempsPoS);
    printf("Temps total PoW : %.6f secondes\n", tempsPoW);

    if (tempsPoS < tempsPoW) {
        printf("Conclusion : PoS est plus rapide.\n");
    } else if (tempsPoW < tempsPoS) {
        printf("Conclusion : PoW est plus rapide.\n");
    } else {
        printf("Conclusion : temps identiques a la precision mesuree.\n");
    }

    return 0;
}
