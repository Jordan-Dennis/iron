import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl 
import sys


mpl.rcParams["text.usetex"] = True


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

        data = np.array(data)

    plt.figure(figsize=(10, 10))
    for ind, temperature in enumerate(range(0, 6, 2)):
        plt.subplot(3, 1, ind + 1)
        plt.title(temperature)
        plt.imshow(data[temperature:temperature + 2, :])
    
    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


# TODO: I need to write of the analytical expression and plot them alongside.
def energy(temperature: float) -> float:
   return - np.tanh(1 / temperature)


def entropy(temperature: float) -> float:
    return 1 / temperature * (1 - np.tanh(1 / temperature)) + np.log(1 + np.exp(-2 / temperature))


def free_energy(temperature: float) -> float:
    return - 1 - temperature * np.log(1 + np.exp(-2 / temperature))


def heat_capacity(temperature: float) -> float:
    return 1 / temperature ** 2 / np.cosh(1 / temperature) ** 2


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

    temperatures = np.linspace(data[:, 0].min(), data[:, 0].max(), 1000)

    plt.figure(figsize=(10, 10))
    plt.subplot(2, 2, 1)
    plt.ylabel(r"$\epsilon$")
    plt.xlabel(r"$\tau$")
    plt.errorbar(data[:, 0], data[:, 1], data[:, 2])
    plt.plot(temperatures, energy(temperatures))
    plt.subplot(2, 2, 2)
    plt.ylabel(r"$\sigma$")
    plt.xlabel(r"$\tau$")
    plt.errorbar(data[:, 0], data[:, 3], data[:, 4])
    plt.plot(temperatures, entropy(temperatures))
    plt.subplot(2, 2, 3)
    plt.ylabel(r"$F$")
    plt.xlabel(r"$\tau$")
    plt.errorbar(data[:, 0], data[:, 5], data[:, 6])
    plt.plot(temperatures, free_energy(temperatures))
    plt.subplot(2, 2, 4)
    plt.ylabel(r"$C_{V}$")
    plt.xlabel(r"$\tau$")
    plt.errorbar(data[:, 0], data[:, 7], np.abs(data[:, 8]))
    plt.plot(temperatures, heat_capacity(temperatures))

    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


def magnetisation(data_file: str, show: bool, save_file: str) -> None:
    """
    Plot the magnetisation of the 1D ising system as a function of the 
    temperature and the number of spins. 

    Parameters
    ----------
    data_file: str
        The file which contains the data. 
    show: bool
        True if the figure is to be shown else false.
    save_file: str
        The file to save the figure to. 
    """
    with open(f"pub/data/{data_file}") as frequencies:
        next(frequencies)
        data = np.array([
            [float(entry) for entry in line.strip().split(",")] 
            for line in frequencies])

    figure, axes = plt.subplots(2, 3, figsize=(12, 8), sharex=True)
    for temp in range(3):
        axes[0][temp].set_title(f"$N = 100, T={temp + 1}K$")
        axes[0][temp].hist(data[:, temp])

        axes[1][temp].set_title(f"$N = 500, T={temp + 1}K$")
        axes[1][temp].hist(data[:, temp + 3])

    if show:
        plt.show()
    if save_file:
        plt.savefig(f"pub/figures/{save_file}")


option = sys.argv[1]
exec(f"{option}('{option}_ising_1d.csv', False, '{option}_ising_1d.pdf')")
    
