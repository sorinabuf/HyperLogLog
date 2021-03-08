// Copyright 2020 Buf Sorina-Anamaria
#include <stdio.h>
#include <stdlib.h>

#define NMAX 2000001

int main() {
  // declararea variabilelor
  int *v_freq;
  int nr;

  // alocarea si initializarea vectorului de frecventa
  v_freq = (int *)calloc(NMAX, sizeof(int));
  if (v_freq == NULL) {
    fprintf(stderr, "Malloc error");
    return -1;
  }

  // crearea vectorului de frecventa
  while (scanf("%d", &nr) == 1) {
    v_freq[nr]++;
  }

  // afisarea perechii numar - numar de aparitii
  for (int i = 0; i < NMAX; ++i) {
    if (v_freq[i] != 0) {
      printf("%d %d\n", i, v_freq[i]);
    }
  }

  // eliberarea memoriei
  free(v_freq);

  return 0;
}
