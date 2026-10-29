# Comparación empírica de Shell sort y Selección directa

Proyecto de la Unidad 01 (Evaluación de producto). Se comparan dos algoritmos de
ordenación internos en C++: **Shell sort** (logarítmico) y **Selección directa** (método directo),
sobre distintos tamaños y patrones de entrada.

## Video
https://youtu.be/Il3x20tllMU

## Objetivos

- Implementar correctamente los dos algoritmos, instrumentados con contadores.
- Diseñar y ejecutar un protocolo de medición repetible (5 patrones × 3 tamaños × 30 repeticiones).
- Calcular estadísticos del tiempo (media, mediana, desviación estándar, mínimo, máximo) y medidas algorítmicas (comparaciones e intercambios).
- Relacionar la complejidad asintótica teórica con la evidencia empírica.

## Estructura del repositorio

```
├── README.md
├── .gitignore
├── docs/
│   ├── manuscrito.pdf
│   └── diapositiva.pdf
├── include/        (interfaces comunes: metrics.hpp, sorts.hpp, generators.hpp)
├── src/            (sorts.cpp, main.cpp, test.cpp)
├── scripts/        (analyze.py: tablas y gráficas)
└── results/        (resultados.csv crudo, tablas y figuras)
```

## Cómo compilar

Desde la raíz del repositorio:

```
g++ -O2 -std=c++14 src/main.cpp src/sorts.cpp -o benchmark
g++ -O2 -std=c++14 src/test.cpp src/sorts.cpp -o test
```

(En Windows el ejecutable se llama `benchmark.exe` y `test.exe`.)

## Cómo ejecutar

1. Prueba de corrección (ambos deben mostrar `ok: 1`):
   ```
   ./test
   ```
2. Benchmark completo (30 repeticiones, n = 1 000, 10 000 y 100 000):
   ```
   ./benchmark 30 results/resultados.csv HASH_DEL_COMMIT
   ```
   Argumentos: `benchmark [repeticiones] [archivo_salida] [commit_hash] [tamaños separados por coma]`.
   Ejemplo de prueba rápida: `./benchmark 3 results/prueba.csv NA 1000,5000`.

## Cómo regenerar tablas y figuras

```
pip install pandas matplotlib
python scripts/analyze.py results/resultados.csv
```

Genera `results/tabla_tiempos.csv`, `results/tabla_tiempos.md` y las gráficas en `results/figuras/`.

## Diseño experimental

- **Patrones de entrada:** aleatorio uniforme (enteros en [0, 1 000 000]), ascendente, descendente, casi ordenado (~5 % de elementos permutados al azar) y con duplicados (15 valores distintos).
- **Semillas:** la instancia de la repetición *r* usa la semilla `12345 + r`; ambos algoritmos reciben la misma instancia. La semilla se registra en cada fila del CSV.
- **Tiempo:** solo se mide el ordenamiento (sin copiar el arreglo ni escribir a disco).
  En Windows se usa `QueryPerformanceCounter`, porque `std::chrono::steady_clock` en MinGW GCC 6.3.0 tiene baja resolución (se observaron mediciones de 0.000 ms). En otros sistemas se usa `std::chrono::steady_clock`.
- **Métricas:** comparaciones entre elementos; intercambios (en Selección, swaps reales; en Shell, desplazamientos); profundidad de recursión (0, ambos son iterativos).
- **Columnas del CSV:** `algoritmo, n, patron, repeticion, tiempo_ms, comparaciones, intercambios, profundidad_recursion, semilla, commit_hash`.

## Entorno de ejecución

| Componente | Valor |
|---|---|
| CPU | [ i38100] |
| Núcleos / hilos | [4/4] |
| Frecuencia base | [3.6Hz] |
| RAM | [16GB] |
| Sistema operativo | Windows [11 pro 64 bits (10.0 compilacion 26200)] |
| Compilador | MinGW GCC 6.3.0 |
| Banderas | `-O2 -std=c++14` |
| Python / pandas / matplotlib | [3.13.15/ 3.1.0 /3.11.2 ] |

## Resultados clave

<img width="2250" height="1200" alt="tiempo_vs_n" src="https://github.com/user-attachments/assets/bde81d8d-2fde-47bf-8c38-7861d80beedf" />
<img width="2250" height="1200" alt="comparaciones_vs_n" src="https://github.com/user-attachments/assets/58f5dece-5028-469a-b7c5-eaece52d764e" />


## Versión final

- **Commit utilizado para el benchmark:** `fd75665`
- **Semilla base:** `12345`
- **Repeticiones por escenario:** 30
- **Total de mediciones:** 900
- **Estado:** resultados experimentales obtenidos; documentación final en elaboración.
