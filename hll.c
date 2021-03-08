// Copyright 2020 Buf Sorina-Anamaria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_BUCKETS 2048

// functia de hash pentru numere intregi
unsigned int hash_integer(void *a) {
  /*
   * Credits: https://stackoverflow.com/a/12996028/7883884
   */
  unsigned int uint_a = *((unsigned int *)a);

  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = (uint_a >> 16u) ^ uint_a;
  return uint_a;
}

// functie care calculeaza puterea lui 2 la un anumit numar
// dat drept parametru
double power(int x) {
  int nr = 1;
  for (int i = 0; i < x; ++i) {
    nr *= 2;
  }
  return 1 * 1.0 / nr;
}

int main(int argc, char *argv[]) {
  // declararea variabilelor
  int M[M_BUCKETS] = {0}, nr, j, nr_zeros, result;
  double z = 0, alpha, e;
  unsigned int hash, copy, rest_hash;
  FILE *in;

  // verificarea numarului de argumente din linia de comanda
  if (argc != 2) {
    fprintf(stderr, "Wrong number of arguments");
    return -1;
  }

  // deschiderea fisierului cu date de intrare
  in = fopen(argv[1], "rt");
  if (in == NULL) {
    fprintf(stderr, "Cannot open %s file", argv[1]);
    return -1;
  }

  // citirea si prelucrarea numerelor
  while (fscanf(in, "%d", &nr) == 1) {
    nr_zeros = 1;

    // calcularea hashului
    hash = hash_integer(&nr);
    copy = hash;

    // calcularea numarului bucketului pe baza primilor 11 biti
    j = (copy >> 21);

    // calcularea restului din hash
    rest_hash = (hash << 11) >> 11;

    // calcularea numarului de biti 0 initiali din restul hashului
    for (int i = 20; i >= 0; --i) {
      copy = rest_hash;
      if (copy >> i == 0) {
        nr_zeros++;
      } else {
        break;
      }
    }

    // adaugarea numarului de biti 0 in vectorul M
    if (M[j] < nr_zeros) {
      M[j] = nr_zeros;
    }
  }

  // calcularea mediei z
  for (int j = 0; j < M_BUCKETS; ++j) {
    z += power(M[j]);
  }
  z = 1 / z;

  alpha = 0.7213 / (1 + (1.079 / M_BUCKETS));

  // calcularea si afisarea numarului de elemente distincte
  e = alpha * M_BUCKETS * M_BUCKETS * z;
  result = (int)(e);
  printf("%d", result);

  // inchiderea fisierului
  fclose(in);

  return 0;
}
