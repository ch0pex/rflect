import subprocess
import json
import statistics
import matplotlib
# Use non-interactive backend for remote environments
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import argparse
import os

# Configuration
benchmark_executable_debug = "../build/Debug/benchmark/imaginary_numbers/imaginary_numbers"
benchmark_executable_release = "../build/Release/benchmark/imaginary_numbers/imaginary_numbers"

output_json_debug  = "../build/Debug/benchmark/imaginary_numbers/results_debug.json"
output_json_release  = "../build/Release/benchmark/imaginary_numbers/results_release.json"

# Output image paths
output_plot_debug   = "../build/Debug/benchmark/imaginary_numbers/plot_debug.png"
output_plot_release = "../build/Release/benchmark/imaginary_numbers/plot_release.png"

output_plot_debug_bars   = "../build/Debug/benchmark/imaginary_numbers/plot_debug_bars.png"
output_plot_release_bars = "../build/Release/benchmark/imaginary_numbers/plot_release_bars.png"


def run_benchmark(executable_path, output_json_path):
    subprocess.run([
        executable_path,
        f"--benchmark_out={output_json_path}",
        "--benchmark_out_format=json"
    ], check=True)


def load_results(json_path):
    with open(json_path, 'r') as f:
        data = json.load(f)
    # Collect all real_time values per benchmark name
    results = {}
    for bench in data.get("benchmarks", []):
        name = bench.get("name")
        time = bench.get("real_time")
        results.setdefault(name, []).append(time)
    # Compute median per name to reduce noise
    aggregated = {name: statistics.median(times) for name, times in results.items()}
    print("Benchmarks found:", list(aggregated.keys()))
    return aggregated


def split_results(results):
    # Group by base name before '/'
    series = {}
    for name, time in results.items():
        parts = name.split('/')
        if len(parts) < 2 or not parts[1].isdigit():
            continue
        base = parts[0]
        size = int(parts[1])
        series.setdefault(base, {})[size] = time
    return series


def plot_comparison(series_dict, title, output_path):
    label_map = [
        ('benchmark_aos', 'AoS'),
        ('benchmark_soa', 'SoA'),
        ('RflectSoa', 'Rflect SoA'),
    ]
    plt.figure(figsize=(10, 6))
    plotted = False
    colors = ['#8DA0CB', '#A6D854', '#E15759', '#76B7B2']
    for (base, label), color in zip(label_map, colors):
        data = series_dict.get(base)
        if not data:
            print(f"Warning: no data for series '{base}'")
            continue
        sizes = sorted(data.keys())
        times = [data[s] for s in sizes]
        plt.plot(sizes, times, marker='o', label=label, color=color)
        plotted = True

    if not plotted:
        print(f"No series to plot for '{title}', skipping save.")
    else:
        plt.xscale('log', base=2)
        plt.yscale('log')
        plt.xlabel('Vector size')
        plt.ylabel('Time (µs)')
        plt.title(title)
        plt.legend()
        plt.grid(True, which="both", linestyle='--', linewidth=0.5)
        plt.tight_layout()
        plt.savefig(output_path)
        print(f"Plot saved to {output_path}")
    plt.close()


def plot_comparison_bars(series_dict, title, output_path):
    label_map = [
        ('benchmark_aos', 'AoS'),
        ('RflectAos', 'Rflect AoS'),
        ('benchmark_soa', 'SoA'),
        ('RflectSoa', 'Rflect SoA'),
    ]
    plt.figure(figsize=(12, 7))

    all_sizes = set()
    for base, _ in label_map:
        data = series_dict.get(base)
        if data:
            all_sizes.update(data.keys())
    all_sizes = sorted(all_sizes)

    width = 0.2
    x = range(len(all_sizes))

    offsets = [-1.5, -0.5, 0.5, 1.5]
    colors = ['#8DA0CB', '#A6D854', '#E15759', '#76B7B2']

    for (base, label), offset, color in zip(label_map, offsets, colors):
        data = series_dict.get(base)
        if not data:
            print(f"Warning: no data for series '{base}' (bars)")
            continue
        heights = [data.get(size, 0) for size in all_sizes]
        x_offset = [xi + offset * width for xi in x]
        plt.bar(x_offset, heights, width=width, label=label, color=color)

    plt.yscale('log')
    plt.xlabel('Vector size')
    plt.ylabel('Time (µs)')
    plt.title(title + " (Bars)")
    plt.xticks(ticks=x, labels=[str(size) for size in all_sizes])
    plt.legend()
    plt.grid(True, which="both", linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Bar plot saved to {output_path}")
    plt.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run benchmarks and plot results or use existing results.")
    parser.add_argument('--run', action='store_true', help='Execute benchmarks before plotting')
    args = parser.parse_args()

    if args.run:
        print("Running Debug build...")
        run_benchmark(benchmark_executable_debug, output_json_debug)
        print("Running Release build...")
        run_benchmark(benchmark_executable_release, output_json_release)
    else:
        print("Skipping benchmark execution; using existing JSON results.")

    # Load and aggregate results
    debug_results = load_results(output_json_debug)
    release_results = load_results(output_json_release)

    # Split aggregated results
    debug_series = split_results(debug_results)
    release_series = split_results(release_results)

    # Plot and save
    print("Plotting and saving Debug results (lines)...")
    plot_comparison(debug_series, "Debug Results", output_plot_debug)

    print("Plotting and saving Release results (lines)...")
    plot_comparison(release_series, "Release Results", output_plot_release)

    print("Plotting and saving Debug results (bars)...")
    plot_comparison_bars(debug_series, "Debug Results", output_plot_debug_bars)

    print("Plotting and saving Release results (bars)...")
    plot_comparison_bars(release_series, "Release Results", output_plot_release_bars)
