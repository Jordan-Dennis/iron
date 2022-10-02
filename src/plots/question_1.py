import numpy as np
import matplotlib.pyplot as plt


def plot_example_states(file_name: str) -> None:
    with open("pub/data/1d_test.csv") as example_states:
        data = []
        for line in example_states:
            if not line.strip().startswith("#"):
                data.append([int(spin) for spin in line.strip().split(",")])

        data = np.array([data])[0]

    plt.figure(figsize=(10, 10))
    for temperature in range(3):
        plt.subplot(3, 1, temperature + 1)
        plt.title(temperature)
        plt.imshow(data[temperature:temperature + 2, :])
    
    plt.show()
    plt.savefig("pub/figures/example_states_ising_1d.pdf")


def plot_physical_parameters(file_name: str, save_name: str) -> None:
    with open(file_name) as physical_parameters:
        next(physical_parameters)
        data = np.array([
            [float(entry) for entry in line.strip().split(",")] 
            for line in physical_parameters])

    plt.figure(figsize=(10, 10))
    plt.subplot(2, 2, 1)
    plt.title(r"$\epsilon$")
    plt.errorbar(data[:, 0], data[:, 1], data[:, 2])
    plt.subplot(2, 2, 2)
    plt.title(r"$\sigma$")
    plt.errorbar(data[:, 0], data[:, 3], data[:, 4])
    plt.subplot(2, 2, 3)
    plt.title(r"$F$")
    plt.errorbar(data[:, 0], data[:, 5], data[:, 6])
    plt.subplot(2, 2, 4)
    plt.title(r"$C_{V}$")
    plt.plot(data[:, 0], data[:, 7])
    plt.savefig(save_name)
    plt.show()

def plot_magnetisation_histogram(file_name: str, save_name: str) -> None:
    with open(file_name) as frequencies:
        next(frequencies)
        data = np.array([
            [float(entry) for entry in line.strip().split(",")] 
            for line in frequencies])

    plt.figure(figsize=(12, 8))
    for temp in range(3):
        plt.subplot(2, 3, temp + 1)
        plt.title(f"$N = 100, T={temp + 1}K$")
        plt.hist(data[:, temp])

        plt.subplot(2, 3, temp + 4)
        plt.title(f"$N = 500, T={temp + 1}K$")
        plt.hist(data[:, temp + 3])

    plt.savefig(save_name)
    plt.show()

plot_magnetisation_histogram(
    "pub/data/1d_test.csv", 
    "pub/figures/1d_magneitsation_histograms.pdf")   
