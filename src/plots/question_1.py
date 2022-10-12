import numpy as np
import matplotlib.pyplot as plt
import sys

def first_and_last(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the first and last states of the ising system at three temperatures
    and evolve the system for some amount of time in between. 

    Parameters
    ----------
    data_file: str
        The file from which to collect the data.
    show: bool
        True if the plot is to be shown else false.
    save_file: str
        The location to save the plot as a pdf. 
    """
    with open(f"pub/data/{data_file}") as example_states:
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
    
    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


# TODO: I need to write of the analytical expression and plot them alongside.
def energy(temperature: float) -> float:
def entropy(temperature: float) -> float:
def free_energy(temperature: float) -> float:
def heat_capacity(temperature: float) -> float:


def physical_parameters(data_file: str, show: bool, save_file: str) -> None:
    """
    Plot the physical parameters against there analytical solutions. 

    Parameters
    ----------
    data_file: str
        The file to access the data from.
    show: bool
        True if the plots are to be displayed else false. 
    save_file: str
        The location to save the plot as a pdf. 
    """
    with open(f"pub/data/{data_file}") as physical_parameters:
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

    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


def magnetisation(data_file: str, show: bool, save_file: str) -> None:
    with open(f"pub/data/{data_file}") as frequencies:
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

    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


option = sys.argv[1]
exec(f"{option}('{option}_ising_1d.csv', False, '{option}_ising_1d.pdf')")
    
