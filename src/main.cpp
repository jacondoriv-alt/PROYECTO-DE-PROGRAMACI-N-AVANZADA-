// Benchmark: Shell sort vs Seleccion directa
// Uso: benchmark [repeticiones] [archivo_salida] [commit_hash] [tamanos]
// Ejemplo: benchmark 30 results/resultados.csv abc1234 1000,10000,100000
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include "../include/sorts.hpp"
#include "../include/generators.hpp"

using namespace std;
// Reloj de alta resolucion (devuelve milisegundos).
// En Windows con MinGW antiguo (GCC 6.x), std::chrono::steady_clock tiene una
// resolucion de ~0.5-15 ms, asi que se usa QueryPerformanceCounter (resolucion < 1 us).
// En Linux/macOS (o MinGW-w64 reciente) se usa std::chrono::steady_clock.
#ifdef _WIN32
static double ahora_ms() {
    static double frecuencia = 0.0;
    if (frecuencia == 0.0) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        frecuencia = static_cast<double>(f.QuadPart);
    }
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return 1000.0 * static_cast<double>(t.QuadPart) / frecuencia;
}
#else
static double ahora_ms() {
    return chrono::duration<double, milli>(
        chrono::steady_clock::now().time_since_epoch()).count();
}
#endif

typedef void (*FuncionOrden)(vector<int>&, int, Metrics&);

struct Algoritmo {
    string nombre;
    FuncionOrden f;
};

int main(int argc, char* argv[]) {
    int reps            = (argc > 1) ? stoi(argv[1]) : 30;
    string salida       = (argc > 2) ? argv[2] : "results/resultados.csv";
    string commit       = (argc > 3) ? argv[3] : "NA";
    vector<int> tamanos = {1000, 10000, 100000};

    if (argc > 4) {
        tamanos.clear();
        stringstream ss(argv[4]);
        string tok;
        while (getline(ss, tok, ',')) tamanos.push_back(stoi(tok));
    }

    const unsigned SEMILLA_BASE = 12345;

    vector<Algoritmo> algoritmos = {
        {"shell", shellsort},
        {"seleccion", SeleccionDir}
    };

    ofstream out(salida);
    if (!out) {
        cerr << "No se pudo abrir " << salida << " (existe la carpeta?)\n";
        return 1;
    }
    out << "algoritmo,n,patron,repeticion,tiempo_ms,comparaciones,intercambios,"
           "profundidad_recursion,semilla,commit_hash\n";

    for (int n : tamanos) {
        for (const string& patron : patrones()) {
            for (int r = 1; r <= reps; r++) {
                // Misma instancia para ambos algoritmos en cada repeticion
                unsigned semilla = SEMILLA_BASE + r;
                vector<int> original = generar(patron, n, semilla);

                for (const auto& alg : algoritmos) {
                    vector<int> A = original;   // copia (fuera del cronometro)
                    Metrics m;

                    double t0 = ahora_ms();
                    alg.f(A, n, m);
                    double t1 = ahora_ms();

                    double ms = t1 - t0;

                    out << alg.nombre << ',' << n << ',' << patron << ',' << r << ','
                        << ms << ',' << m.comparaciones << ',' << m.intercambios << ','
                        << m.profundidad << ',' << semilla << ',' << commit << '\n';
                }
            }
            cout << "Listo: n=" << n << " patron=" << patron << endl;
        }
    }

    cout << "Resultados en " << salida << endl;
    return 0;
}
