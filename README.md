# Compression et Décompression avec l'Algorithme de Huffman

Ce projet implémente l'algorithme de compression de données de Huffman en langage C. Il permet de compresser un fichier texte en utilisant un arbre de Huffman et de décompresser le fichier compressé pour retrouver le texte d'origine.

## Table des matières

- [Compression et Décompression avec l'Algorithme de Huffman](#compression-et-décompression-avec-lalgorithme-de-huffman)
  - [Table des matières](#table-des-matières)
  - [Description](#description)
    - [Fonctionnalités](#fonctionnalités)
  - [Prérequis](#prérequis)
  - [Installation](#installation)
  - [Utilisation](#utilisation)
    - [Compression](#compression)
    - [Décompression](#décompression)
    - [Exemple complet](#exemple-complet)
    - [Notes importantes](#notes-importantes)
  - [Explication de l'algorithme](#explication-de-lalgorithme)
    - [Étapes de l'algorithme](#étapes-de-lalgorithme)
  - [Contribuer](#contribuer)
  - [Licence](#licence)

## Description

Ce projet permet de :

1. **Compresser** un fichier texte en utilisant l'algorithme de Huffman pour produire un fichier binaire compressé.
2. **Décompresser** un fichier binaire compressé pour retrouver le fichier texte d'origine.

### Fonctionnalités

- **Compression** : Lecture d'un fichier texte, construction d'un arbre de Huffman basé sur la fréquence des caractères, et génération d'un fichier binaire compressé.
- **Décompression** : Lecture du fichier binaire compressé, reconstruction de l'arbre de Huffman à partir des codes, et génération du fichier texte original.

## Prérequis

Avant d'utiliser ce projet, vous devez avoir les éléments suivants installés :

- Un compilateur C, tel que **GCC** (GNU Compiler Collection).
- Système d'exploitation compatible pour la compilation en C (Linux, Windows avec MinGW, ou macOS).

## Installation

1. Clonez ce repository sur votre machine locale :
   ```bash
   git clone https://github.com/votre-utilisateur/huffman-compression.git
   cd huffman-compression
   ```

2. Compilez le code avec **GCC** :
   ```bash
   gcc -o huffman huffman.c
   ```

   Cela générera un exécutable appelé `huffman` que vous pouvez utiliser pour compresser et décompresser des fichiers.

## Utilisation

### Compression

Pour compresser un fichier texte en utilisant l'algorithme de Huffman, utilisez la commande suivante :

```bash
./huffman compress input.txt output.bin
```

- **input.txt** : Le fichier texte que vous souhaitez compresser.
- **output.bin** : Le fichier binaire compressé qui sera généré.

Exemple :

```bash
./huffman compress example.txt compressed.bin
```

### Décompression

Pour décompresser un fichier binaire compressé et retrouver le texte original, utilisez la commande suivante :

```bash
./huffman decompress input.bin output.txt
```

- **input.bin** : Le fichier binaire compressé.
- **output.txt** : Le fichier texte décompressé qui sera généré.

Exemple :

```bash
./huffman decompress compressed.bin decompressed.txt
```

### Exemple complet

1. Compression :
   ```bash
   ./huffman compress example.txt compressed.bin
   ```
   
2. Décompression :
   ```bash
   ./huffman decompress compressed.bin decompressed.txt
   ```

### Notes importantes

- Assurez-vous que le fichier texte ne contient pas de caractères spéciaux ou non imprimables qui pourraient affecter le processus de compression et décompression.
- Le processus de compression dépend de la fréquence des caractères dans le fichier texte. Les fichiers avec un grand nombre de caractères répétés se compressent mieux.
  
## Explication de l'algorithme

L'algorithme de Huffman est un algorithme de compression sans perte basé sur les fréquences des caractères. Il construit un arbre binaire, où chaque feuille représente un caractère et sa fréquence d'apparition. Les caractères les plus fréquents sont codés avec des codes binaires plus courts, tandis que les caractères moins fréquents sont codés avec des codes plus longs.

### Étapes de l'algorithme

1. **Calcul de la fréquence** des caractères dans le fichier.
2. **Construction de l'arbre de Huffman** en fonction des fréquences.
3. **Génération des codes binaires** pour chaque caractère basé sur l'arbre de Huffman.
4. **Compression** du fichier en remplaçant les caractères par leurs codes binaires.
5. **Décompression** du fichier binaire en utilisant l'arbre de Huffman pour reconstruire les caractères originaux.

## Contribuer

Si vous souhaitez contribuer à ce projet, vous pouvez :

1. Forker ce repository.
2. Créer une branche pour la fonctionnalité que vous souhaitez ajouter.
3. Soumettre une pull request une fois que vos modifications sont terminées.

## Licence

Ce projet est sous licence MIT. Vous pouvez consulter le fichier `LICENSE` pour plus d'informations.