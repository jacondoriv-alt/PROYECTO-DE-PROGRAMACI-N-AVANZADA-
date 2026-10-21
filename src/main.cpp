// Benchmark: Shell sort vs Seleccion directa
// Uso: benchmark [repeticiones] [archivo_salida] [commit_hash] [tamanos]
// Ejemplo: benchmark 30 results/resultados.csv abc1234 1000,10000,100000
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include "../include/sorts.hpp"
#include "../include/generators.hpp"

using namespace std;
using Reloj = chrono::steady_clock;

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

                    auto t0 = Reloj::now();
                    alg.f(A, n, m);
                    auto t1 = Reloj::now();

                    double ms = chrono::duration<double, milli>(t1 - t0).count();

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
