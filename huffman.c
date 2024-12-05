#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour les nœuds de l'arbre de Huffman
typedef struct Node {
    char character;
    int frequency;
    struct Node *left, *right;
} Node;

// Structure pour la table des codes de Huffman
typedef struct {
    char character;
    char code[256];
} HuffmanCode;

HuffmanCode huffmanTable[256];
int tableIndex = 0;

// Fonction pour créer un nouveau nœud
Node *createNode(char character, int frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Comparateur pour le tri
int compare(const void *a, const void *b) {
    return (*(Node **)a)->frequency - (*(Node **)b)->frequency;
}

// Construction de l'arbre de Huffman
Node *buildHuffmanTree(Node **nodes, int size) {
    while (size > 1) {
        // Trier les nœuds par fréquence
        qsort(nodes, size, sizeof(Node *), compare);

        // Combiner les deux nœuds ayant les plus petites fréquences
        Node *left = nodes[0];
        Node *right = nodes[1];
        Node *newNode = createNode('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        // Remplacer les deux premiers nœuds par le nouveau
        nodes[0] = newNode;
        nodes[1] = nodes[size - 1];
        size--;
    }
    return nodes[0];
}

// Génération des codes de Huffman
void generateCodes(Node *root, char *code, int length) {
    if (!root->left && !root->right) {
        code[length] = '\0';
        huffmanTable[tableIndex].character = root->character;
        strcpy(huffmanTable[tableIndex].code, code);
        tableIndex++;
        return;
    }
    if (root->left) {
        code[length] = '0';
        generateCodes(root->left, code, length + 1);
    }
    if (root->right) {
        code[length] = '1';
        generateCodes(root->right, code, length + 1);
    }
}

// Compression du fichier texte
void compressFile(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "r");
    if (!in) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        exit(EXIT_FAILURE);
    }

    int freq[256] = {0};
    char buffer;
    while (fread(&buffer, 1, 1, in)) {
        freq[(unsigned char)buffer]++;
    }
    fclose(in);

    // Créer les nœuds pour chaque caractère
    Node *nodes[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            nodes[size++] = createNode(i, freq[i]);
        }
    }

    // Construire l'arbre de Huffman
    Node *root = buildHuffmanTree(nodes, size);

    // Générer les codes
    char code[256];
    generateCodes(root, code, 0);

    // Écrire le fichier compressé
    in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    // Sauvegarder la table des codes
    fwrite(&size, sizeof(int), 1, out);
    for (int i = 0; i < tableIndex; i++) {
        fwrite(&huffmanTable[i].character, sizeof(char), 1, out);
        int codeLength = strlen(huffmanTable[i].code);
        fwrite(&codeLength, sizeof(int), 1, out);
        fwrite(huffmanTable[i].code, sizeof(char), codeLength, out);
    }

    // Écrire les données compressées
    unsigned char byte = 0, bitCount = 0;
    while (fread(&buffer, 1, 1, in)) {
        for (int i = 0; i < tableIndex; i++) {
            if (huffmanTable[i].character == buffer) {
                for (char *c = huffmanTable[i].code; *c; c++) {
                    byte = (byte << 1) | (*c - '0');
                    bitCount++;
                    if (bitCount == 8) {
                        fwrite(&byte, 1, 1, out);
                        byte = 0;
                        bitCount = 0;
                    }
                }
                break;
            }
        }
    }
    if (bitCount) {
        byte <<= (8 - bitCount);
        fwrite(&byte, 1, 1, out);
    }

    fclose(in);
    fclose(out);
}

void decompressFile(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    if (!in) {
        perror("Erreur lors de l'ouverture du fichier binaire compressé");
        exit(EXIT_FAILURE);
    }

    // Lire la taille de la table des codes
    int size;
    fread(&size, sizeof(int), 1, in);

    // Lire la table des codes
    HuffmanCode table[256];
    for (int i = 0; i < size; i++) {
        fread(&table[i].character, sizeof(char), 1, in);
        int codeLength;
        fread(&codeLength, sizeof(int), 1, in);
        fread(table[i].code, sizeof(char), codeLength, in);
        table[i].code[codeLength] = '\0';
    }

    // Construire l'arbre de Huffman à partir de la table
    Node *root = createNode('\0', 0);
    for (int i = 0; i < size; i++) {
        Node *current = root;
        for (char *c = table[i].code; *c; c++) {
            if (*c == '0') {
                if (!current->left) {
                    current->left = createNode('\0', 0);
                }
                current = current->left;
            } else { // *c == '1'
                if (!current->right) {
                    current->right = createNode('\0', 0);
                }
                current = current->right;
            }
        }
        current->character = table[i].character;
    }

    // Décompression des données
    FILE *out = fopen(outputFile, "w");
    if (!out) {
        perror("Erreur lors de l'ouverture du fichier texte décompressé");
        exit(EXIT_FAILURE);
    }

    Node *current = root;
    unsigned char byte;
    while (fread(&byte, 1, 1, in)) {
        for (int i = 7; i >= 0; i--) { // Lire chaque bit
            if (byte & (1 << i)) {
                current = current->right;
            } else {
                current = current->left;
            }

            // Si on atteint une feuille
            if (!current->left && !current->right) {
                fputc(current->character, out);
                current = root;
            }
        }
    }

    fclose(in);
    fclose(out);
}

// Fonction principale
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <compress|decompress> <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "compress") == 0) {
        compressFile(argv[2], argv[3]);
    } else if (strcmp(argv[1], "decompress") == 0) {
        decompressFile(argv[2], argv[3]);   
    } else {
        printf("Option invalide. Utilisez 'compress' ou 'decompress'.\n");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
