"""
Analisis de resultados: Shell sort vs Seleccion directa.
Uso: python scripts/analyze.py [results/resultados.csv]
Genera:
  results/tabla_tiempos.csv     (media, mediana, desv. est., min, max del tiempo + promedio de comparaciones e intercambios)
  results/tabla_tiempos.md      (misma tabla en formato Markdown, para pegar en el articulo/README)
  results/figuras/tiempo_vs_n.png
  results/figuras/comparaciones_vs_n.png
"""
import sys
from pathlib import Path
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ruta = Path(sys.argv[1] if len(sys.argv) > 1 else "results/resultados.csv")
salida = ruta.parent
figuras = salida / "figuras"
figuras.mkdir(parents=True, exist_ok=True)

df = pd.read_csv(ruta)

ceros = (df["tiempo_ms"] <= 0).sum()
if ceros > 0:
    print(f"ADVERTENCIA: {ceros} de {len(df)} mediciones tienen tiempo 0 ms "
          f"({100*ceros/len(df):.2f} %). El reloj es de baja resolucion: vuelve a medir.")

orden_patrones = ["aleatorio", "ascendente", "descendente", "casi_ordenado", "duplicados"]
patrones = [p for p in orden_patrones if p in df["patron"].unique()]

# ---------- Tabla de estadisticos ----------
grupo = df.groupby(["algoritmo", "patron", "n"])
tabla = grupo["tiempo_ms"].agg(["count", "mean", "median", "std", "min", "max"])
tabla.columns = ["repeticiones", "media_ms", "mediana_ms", "desv_est_ms", "min_ms", "max_ms"]
tabla["comparaciones_prom"] = grupo["comparaciones"].mean()
tabla["intercambios_prom"] = grupo["intercambios"].mean()
tabla = tabla.reset_index()
tabla["patron"] = pd.Categorical(tabla["patron"], categories=patrones, ordered=True)
tabla = tabla.sort_values(["patron", "n", "algoritmo"]).reset_index(drop=True)

tabla.to_csv(salida / "tabla_tiempos.csv", index=False, float_format="%.4f")

# Markdown a mano (sin depender de librerias extra)
cols = list(tabla.columns)
lineas = ["| " + " | ".join(cols) + " |", "|" + "|".join(["---"] * len(cols)) + "|"]
for _, fila in tabla.iterrows():
    celdas = []
    for c in cols:
        v = fila[c]
        celdas.append(f"{v:.4f}" if isinstance(v, float) else str(v))
    lineas.append("| " + " | ".join(celdas) + " |")
(salida / "tabla_tiempos.md").write_text("\n".join(lineas), encoding="utf-8")

# ---------- Graficas ----------
estilos = {"shell": ("tab:blue", "o"), "seleccion": ("tab:red", "s")}

def figura(col_media, col_err, ylabel, archivo, titulo):
    fig, ejes = plt.subplots(2, 3, figsize=(15, 8))
    ejes = ejes.flatten()
    for ax, patron in zip(ejes, patrones):
        for alg, (color, marca) in estilos.items():
            sub = tabla[(tabla["patron"] == patron) & (tabla["algoritmo"] == alg)]
            if sub.empty:
                continue
            ax.errorbar(sub["n"], sub[col_media],
                        yerr=sub[col_err] if col_err else None,
                        label=alg, color=color, marker=marca, capsize=3)
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_title(patron)
        ax.set_xlabel("n")
        ax.set_ylabel(ylabel)
        ax.grid(True, which="both", alpha=0.3)
        ax.legend()
    for ax in ejes[len(patrones):]:
        ax.axis("off")
    fig.suptitle(titulo)
    fig.tight_layout()
    fig.savefig(figuras / archivo, dpi=150)
    plt.close(fig)

figura("media_ms", "desv_est_ms", "tiempo medio (ms)", "tiempo_vs_n.png",
       "Tiempo de ejecucion vs n (media +- desv. est., ejes log-log)")
figura("comparaciones_prom", None, "comparaciones (promedio)", "comparaciones_vs_n.png",
       "Comparaciones vs n (ejes log-log)")

print("Listo. Archivos generados en", salida)
print(tabla.to_string(index=False, float_format=lambda x: f"{x:.3f}"))
