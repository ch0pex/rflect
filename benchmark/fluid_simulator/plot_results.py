import subprocess
import matplotlib.pyplot as plt
import sys
import os
import statistics # Necesario para calcular la media

def get_execution_time(executable_path):
    """
    Ejecuta un programa y extrae el tiempo de ejecución de su salida.
    Devuelve el tiempo en segundos o None si hay un error.
    """
    # print(f"Ejecutando: {executable_path}") # Puedes comentar esto para menos verbosidad durante las 5 runs
    try:
        # Ejecuta el programa y captura la salida
        result = subprocess.run([executable_path], capture_output=True, text=True, check=True)
        output = result.stdout

    except FileNotFoundError:
        print(f"Error: Ejecutable no encontrado en la ruta {executable_path}", file=sys.stderr)
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error al ejecutar {executable_path}: {e}", file=sys.stderr)
        print(f"Salida de error (stderr): {e.stderr}", file=sys.stderr)
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

# --- El código principal del script ---
if __name__ == "__main__":
    executables = [
        "../../build/Release/benchmark/fluid_simulator/non-reflected-aos",
        "../../build/Release/benchmark/fluid_simulator/reflected-aos",
        "../../build/Release/benchmark/fluid_simulator/non-reflected-soa",
        "../../build/Release/benchmark/fluid_simulator/reflected-soa",
    ]

    num_runs = 5 # <--- Define aquí el número de ejecuciones por programa

    executable_labels = []
    average_execution_times = [] # Cambiamos el nombre para ser más claros

    print(f"Ejecutando cada programa {num_runs} veces para calcular la media...")

    for exe_path in executables:
        print(f"\nProcesando: {exe_path}")
        current_executable_times = [] # Lista para almacenar los tiempos de las N ejecuciones de este programa

        for i in range(num_runs):
            print(f"  Ejecución {i + 1}/{num_runs}...")
            time = get_execution_time(exe_path)
            if time is not None:
                current_executable_times.append(time)
            # Puedes añadir un pequeño retraso aquí si es necesario entre ejecuciones (import time; time.sleep(0.1))

        if current_executable_times: # Asegurarse de que se obtuvo al menos un tiempo válido
            average_time = statistics.mean(current_executable_times)
            label = os.path.basename(exe_path) # Usa el nombre del archivo como etiqueta
            executable_labels.append(label)
            average_execution_times.append(average_time)
            print(f"  Media calculada para {label}: {average_time:.4f}s ({len(current_executable_times)}/{num_runs} ejecuciones exitosas)")
        else:
            print(f"  No se obtuvieron tiempos válidos para {os.path.basename(exe_path)}. Se omitirá en el gráfico.")


    # --- Crear el gráfico de barras ---
    if not average_execution_times:
        print("\nNo se pudieron recopilar tiempos de ejecución válidos para generar el gráfico.", file=sys.stderr)
    else:
        plt.figure(figsize=(10, 6)) # Ajusta el tamaño de la figura si es necesario

        # --- Define una lista de colores pastel ---
        pastel_colors = [
            '#ffb3ba', # Pastel Red
            '#ffdfba', # Pastel Orange
            '#ffffba', # Pastel Yellow
            '#baffc9', # Pastel Green
            # Asegúrate de tener suficientes colores para el número de barras (ejecutables con tiempos válidos)
            # Si no, matplotlib reciclará colores o puedes añadir más a la lista
        ]
        # Usa solo los colores necesarios de la lista pastel_colors
        colors_to_use = pastel_colors[:len(average_execution_times)]


        # Crea las barras usando la lista de colores
        bars = plt.bar(executable_labels, average_execution_times, color=colors_to_use)


        # Añadir los valores de la media encima de las barras
        for bar in bars:
            yval = bar.get_height()
            # Asegúrate de que yval es un número para el formato
            if isinstance(yval, (int, float)):
                plt.text(bar.get_x() + bar.get_width()/2.0, yval, f'{yval:.2f}s', va='bottom', ha='center') # .2f para 2 decimales


        plt.ylabel("Average Execution time (s)") # Actualiza la etiqueta del eje Y
        plt.title(f"Fluid simulation performance (Average of {num_runs} runs)") # Actualiza el título
        plt.xticks(rotation=45, ha="right") # Rota las etiquetas del eje x si son largas
        plt.tight_layout() # Ajusta el layout para evitar que las etiquetas se corten
        plt.grid(axis='y', linestyle='--', alpha=0.7) # Añade una rejilla suave en el eje y

        plt.savefig("results/release.png")
        plt.close()
        print("\nGráfico 'results/release.png' generado exitosamente.")