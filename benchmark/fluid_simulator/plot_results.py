import subprocess
import matplotlib.pyplot as plt
import sys
import os

def get_execution_time(executable_path):
    """
    Ejecuta un programa y extrae el tiempo de ejecución de su salida.
    """
    print(f"Ejecutando: {executable_path}")
    try:
        # Ejecuta el programa y captura la salida
        # 'text=True' decodifica la salida de bytes a string
        # 'check=True' lanzará un error si el ejecutable retorna un código distinto de 0
        result = subprocess.run([executable_path], capture_output=True, text=True, check=True)
        output = result.stdout
        # print("Salida capturada:\n", output) # Opcional: para depuración

    except FileNotFoundError:
        print(f"Error: Ejecutable no encontrado en la ruta {executable_path}", file=sys.stderr)
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error al ejecutar {executable_path}: {e}", file=sys.stderr)
        print(f"Salida de error: {e.stderr}", file=sys.stderr)
        return None
    except Exception as e:
        print(f"Ocurrió un error inesperado al ejecutar {executable_path}: {e}", file=sys.stderr)
        return None

    # Buscar la línea que contiene "Execution time:"
    time_line = None
    for line in output.splitlines():
        if "Execution time:" in line:
            time_line = line.strip()
            break

    if not time_line:
        print(f"Advertencia: 'Execution time:' no encontrado en la salida de {executable_path}", file=sys.stderr)
        return None

    # Extraer el tiempo de la línea encontrada
    try:
        # Divide la línea por ':' y toma la segunda parte
        time_str = time_line.split(':', 1)[1].strip()
        # Elimina 's' al final si existe y convierte a float
        if time_str.endswith('s'):
            time_str = time_str[:-1]
        execution_time = float(time_str)
        return execution_time
    except (IndexError, ValueError) as e:
        print(f"Error al parsear el tiempo de la línea '{time_line}' de {executable_path}: {e}", file=sys.stderr)
        return None

if __name__ == "__main__":
    executables = [
        "../../build/Release/benchmark/fluid_simulator/non-reflected-aos",
        "../../build/Release/benchmark/fluid_simulator/reflected-aos",
        "../../build/Release/benchmark/fluid_simulator/non-reflected-soa",
        "../../build/Release/benchmark/fluid_simulator/reflected-soa",
    ]

    executable_labels = []
    execution_times = []

    for exe_path in executables:
        time = get_execution_time(exe_path)
        if time is not None:
            # Usa el nombre del archivo como etiqueta
            label = os.path.basename(exe_path)
            executable_labels.append(label)
            execution_times.append(time)

    # --- Crear el gráfico de barras ---
    if not execution_times:
        print("No se pudieron recopilar tiempos de ejecución válidos para generar el gráfico.", file=sys.stderr)
    else:
        plt.figure(figsize=(10, 6)) # Ajusta el tamaño de la figura si es necesario

        # --- Define una lista de colores pastel ---
        # Asegúrate de tener al menos tantos colores como ejecutables (4 en este caso)
        pastel_colors = [
            '#ffb3ba', # Pastel Red
            '#ffdfba', # Pastel Orange
            '#ffffba', # Pastel Yellow
            '#baffc9', # Pastel Green
            # Puedes añadir más si tu lista de ejecutables crece
            # '#bae1ff', # Pastel Blue
            # '#f4baff', # Pastel Purple
        ]
        # Si hay más ejecutables que colores, los colores se reciclarán
        # O puedes usar una lista de colores más larga

        # Crea las barras usando la lista de colores
        bars = plt.bar(executable_labels, execution_times, color=pastel_colors[:len(executable_labels)]) # Usa solo los colores necesarios


        # Añadir los valores de tiempo encima de las barras
        for bar in bars:
            yval = bar.get_height()
            plt.text(bar.get_x() + bar.get_width()/2.0, yval, f'{yval:.2f}s', va='bottom', ha='center') # .2f para 2 decimales

        plt.ylabel("Execution time (s)")
        plt.title("Fluid simulation performance")
        plt.xticks(rotation=45, ha="right") # Rota las etiquetas del eje x si son largas
        plt.tight_layout() # Ajusta el layout para evitar que las etiquetas se corten
        plt.grid(axis='y', linestyle='--', alpha=0.7) # Añade una rejilla suave en el eje y

        plt.savefig("results/release.png")
        plt.close()