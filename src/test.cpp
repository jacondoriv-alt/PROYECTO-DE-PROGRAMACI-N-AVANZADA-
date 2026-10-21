#include <iostream>
#include <algorithm>
#include <random>
#include "../include/sorts.hpp"
using namespace std;
int main() {
    mt19937 gen(42);
    uniform_int_distribution<int> d(0, 1000000);
    vector<int> base(2000);
    for (auto& x : base) x = d(gen);
    vector<int> ref = base; sort(ref.begin(), ref.end());

    vector<int> a = base; Metrics m1; shellsort(a, a.size(), m1);
    vector<int> b = base; Metrics m2; SeleccionDir(b, b.size(), m2);
    cout << "Shell ok: " << (a == ref) << " comp=" << m1.comparaciones << " mov=" << m1.intercambios << "\n";
    cout << "Seleccion ok: " << (b == ref) << " comp=" << m2.comparaciones << " swaps=" << m2.intercambios << "\n";
}
