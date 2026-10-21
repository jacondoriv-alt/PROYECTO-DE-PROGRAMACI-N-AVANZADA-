#pragma once

// Contadores que cada algoritmo actualiza mientras ordena.
struct Metrics {
    long long comparaciones = 0;   // comparaciones entre elementos del arreglo
    long long intercambios  = 0;   // Seleccion: swaps reales; Shell: desplazamientos (movimientos)
    int profundidad = 0;           // profundidad max. de recursion (0: ambos son iterativos)

    void reset() { comparaciones = 0; intercambios = 0; profundidad = 0; }
};
