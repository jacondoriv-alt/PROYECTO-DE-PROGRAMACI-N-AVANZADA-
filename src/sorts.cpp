#include "../include/sorts.hpp"
using std::vector;

// Shell sort (secuencia n/2, n/4, ..., 1)
void shellsort(vector<int>& A, int n, Metrics& m) {
    int k = n + 1;

    while (k > 1) {
        k = k / 2;

        for (int i = k; i < n; i++) {
            int aux = A[i];
            int j = i;

            while (j - k >= 0) {
                m.comparaciones++;                 // compara A[j-k] con aux
                if (A[j - k] <= aux) break;
                A[j] = A[j - k];
                m.intercambios++;                  // desplazamiento
                j = j - k;
            }

            A[j] = aux;
        }
    }
}

// Seleccion directa
void SeleccionDir(vector<int>& A, int n, Metrics& m) {
    for (int i = 0; i < n - 1; i++) {
        int menor = A[i];
        int k = i;

        for (int j = i + 1; j < n; j++) {
            m.comparaciones++;
            if (A[j] < menor) {
                menor = A[j];
                k = j;
            }
        }

        if (k != i) m.intercambios++;              // swap real
        A[k] = A[i];
        A[i] = menor;
    }
}
