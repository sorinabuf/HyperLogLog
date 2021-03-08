// Copyright 2020 Buf Sorina-Anamaria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// am presupus lungimea maxima a hashtable-ului 1000
#define NMAX 1000

#define LENGTH 101

// structura datelor continute de fiecare bucket din hashtable
struct info {
  char key[LENGTH];
  // nr de coliziuni are valoarea 0 daca bucketul este gol;
  // nr de coliziuni are are valoarea -1 daca in bucket este stocat
  // un cavant care a mai fost citit (nu este la prima aparitie);
  // nr de coliziuni are valoarea >= 1 daca in bucket este stocata
  // prima aparitie a cuvantului (contine astfel numarul de aparitii
  // pentru stringul respectiv)
  int nr_collisions;
};

// structura hashtable-ului
struct Hashtable {
  struct info buckets[NMAX];
  int nr_words;
};

// functia de hash pentru stringuri
unsigned int hash_function_string(char key[LENGTH]) {
  unsigned int hash = 0;
  int l = strlen(key);
  for (int i = 0; i < l; ++i) {
    hash += key[i];
  }
  return hash % 37;
}

// functie care rezolva coliziuni in cazul in care doua cuvinte
// identice au acelasi hash
void collision_same_word(struct Hashtable *ht, unsigned int index,
                         char word[LENGTH]) {
  index++;
  // cat timp nu ajung la finalul hashtable-ului
  while (index < NMAX) {
    // verificare ca am gasit un bucket gol
    if (ht->buckets[index].nr_collisions == 0) {
      ht->buckets[index].nr_collisions = -1;
      snprintf(ht->buckets[index].key, strlen(word) + 1, "%s", word);
      break;
    }
    index++;
  }

  // daca am ajuns la capatul hashtable-ului continui verificarea
  // de la inceput
  if (index == NMAX) {
    index = 0;
    while (1) {
      // verificare ca am gasit un bucket gol
      if (ht->buckets[index].nr_collisions == 0) {
        ht->buckets[index].nr_collisions = -1;
        snprintf(ht->buckets[index].key, strlen(word) + 1, "%s", word);
        break;
      }
      index++;
    }
  }
}

// functie care rezolva coliziuni in cazul in care doua cuvinte
// diferite au acelasi hash
void collision_different_word(struct Hashtable *ht, unsigned int index,
                              char word[LENGTH]) {
  index++;

  // presupun ca nu am gasit o alta aparitie a cuvantului citit in hashtable
  int ok = 0;

  // cat timp nu ajung la finalul hashtable-ului
  while (index < NMAX) {
    // verificare ca am ajuns in bucketul care pastreaza numarul de
    // aparitii pentru cavantul citit
    if (strcmp(ht->buckets[index].key, word) == 0 &&
      ht->buckets[index].nr_collisions != -1) {
      ht->buckets[index].nr_collisions++;
      ok = 1;
    }
    // verificarea ca am gasit un bucket gol
    if (ht->buckets[index].nr_collisions == 0) {
      snprintf(ht->buckets[index].key, strlen(word) + 1, "%s", word);
      if (ok == 1) {
        ht->buckets[index].nr_collisions = -1;
      } else {
        ht->buckets[index].nr_collisions = 1;
      }
      break;
    }
    index++;
  }

  // daca am ajuns la capatul hashtable-ului continui verificarea
  // de la inceput
  if (index == NMAX) {
    index = 0;
    while (1) {
      // verificare ca am ajuns in bucketul care pastreaza numarul de
      // aparitii pentru cavantul citit
      if (strcmp(ht->buckets[index].key, word) == 0 &&
        ht->buckets[index].nr_collisions != -1) {
        ht->buckets[index].nr_collisions++;
        ok = 1;
      }

      // verificarea ca am gasit un bucket gol
      if (ht->buckets[index].nr_collisions == 0) {
        snprintf(ht->buckets[index].key, strlen(word) + 1, "%s", word);
        if (ok == 1) {
          ht->buckets[index].nr_collisions = -1;
        } else {
          ht->buckets[index].nr_collisions = 1;
        }
        break;
      }
      index++;
    }
  }
}

// functie care afiseaza perechea string - numar de aparitii
void print_words(struct Hashtable *ht) {
  for (int i = 0; i < NMAX; ++i) {
    if (ht->buckets[i].nr_collisions >= 1) {
      printf("%s %d\n", ht->buckets[i].key, ht->buckets[i].nr_collisions);
    }
  }
}

int main() {
  // declararea variabilelor
  char word[LENGTH];
  unsigned int index;
  struct Hashtable *ht;

  // alocarea de memorie pentru hashtable
  ht = (struct Hashtable *)malloc(sizeof(struct Hashtable));
  if (ht == NULL) {
    fprintf(stderr, "Malloc error");
    return -1;
  }

  // initializarea hashtable-ului
  for (int i = 0; i < NMAX; ++i) {
    memset(ht->buckets[i].key, '\0', LENGTH);
    ht->buckets[i].nr_collisions = 0;
  }
  ht->nr_words = 0;

  // citirea si prelucrarea stringurilor
  while (scanf("%s", word) == 1) {
    ht->nr_words++;

    // calcularea indexului cu functia de hash
    index = hash_function_string(word);

    // verificare ca bucketul aferent indexului este gol
    if (ht->buckets[index].nr_collisions == 0) {
      ht->buckets[index].nr_collisions = 1;
      snprintf(ht->buckets[index].key, strlen(word) + 1, "%s", word);
    } else {
      // verificare ca stringul citit se afla deja in bucket
      if (strcmp(ht->buckets[index].key, word) == 0) {
        ht->buckets[index].nr_collisions++;
        // rezolvare coliziunii pentru acelasi cuvant
        collision_same_word(ht, index, word);
      } else {
        // rezolvare coliziunii pentru cuvinte diferite
        collision_different_word(ht, index, word);
      }
    }
  }

  // printarea cuvintelor
  print_words(ht);

  // eliberarea memoriei
  free(ht);

  return 0;
}
