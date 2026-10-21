#pragma once
#include <vector>
#include <string>
#include <random>
#include <algorithm>

// Patrones de entrada
inline const std::vector<std::string>& patrones() {
    static const std::vector<std::string> p = {
        "aleatorio", "ascendente", "descendente", "casi_ordenado", "duplicados"
    };
    return p;
}

// Genera un arreglo de tamano n con el patron indicado, usando la semilla dada.
inline std::vector<int> generar(const std::string& patron, int n, unsigned seed) {
    std::mt19937 gen(seed);
    std::vector<int> A(n);

    if (patron == "aleatorio") {
        std::uniform_int_distribution<int> d(0, 1000000);
        for (auto& x : A) x = d(gen);
    }
    else if (patron == "ascendente") {
        for (int i = 0; i < n; i++) A[i] = i;
    }
    else if (patron == "descendente") {
        for (int i = 0; i < n; i++) A[i] = n - i;
    }
    else if (patron == "casi_ordenado") {
        // Ascendente + ~5% de elementos movidos al azar (n*0.05/2 intercambios de pares)
        for (int i = 0; i < n; i++) A[i] = i;
        std::uniform_int_distribution<int> d(0, n - 1);
        int swaps = static_cast<int>(n * 0.05 / 2);
        for (int s = 0; s < swaps; s++) std::swap(A[d(gen)], A[d(gen)]);
    }
    else if (patron == "duplicados") {
        // Solo 15 valores distintos repetidos
        std::uniform_int_distribution<int> d(0, 14);
        for (auto& x : A) x = d(gen);
    }
    return A;
}
