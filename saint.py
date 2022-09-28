import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt
from scipy.sparse import spdiags,linalg,eye
##  BLOCK OF FUNCTIONS USED IN THE MAIN CODE
#----------------------------------------------------------------------



def initial_state(number: int) -> int:   
    ''' 
    Generates a random spin configuration for initial condition
    '''
    return 2 * np.random.randint(2, size=(number, number)) - 1


def mcmove(spins, temperature):
    '''
    Monte Carlo move using Metropolis algorithm 
    '''
    rows, columns = spins.shape

    for _ in np.arange(spins.size):
        row = np.random.randint(0, rows)
        column = np.random.randint(0, columns)

        neighbours = spins[(row + 1) % rows, column] 
        neighbours += spins[row, (column + 1) % columns] 
        neighbours += spins[(row - 1) % rows, column] 
        neighbours += spins[row, (column - 1) % columns]

        cost = 2 * spins[row, column] * neighbours
        
        if (cost < 0) or (rand() < np.exp(-cost / temperature)):
            spins[row, column] *= -1
    
    return spins



def calc_energy(spins: int) -> float:
    '''
    Energy of a given configuration
    '''
    energy = 0 
   
    rows, columns = spins.shape
    for row in np.arange(rows):
        for column in np.arange(columns):
            neighbours = spins[(row + 1) % rows, column] 
            neighbours += spins[row, (column + 1) % columns] 
            neighbours += spins[(row - 1) % rows, column] 
            neighbours += spins[row, (column - 1) % columns]
            energy += -neighbours * spins[row, column]
    
    return energy / 2.  # to compensate for over-counting


def calc_magnetisation(spins: int) -> float:
    '''
    Magnetization of a given configuration
    '''
    return np.sum(spins)


def calc_entropy(spins: int) -> float:
    rows, columns = spins.shape
    aligned = 0
    for row in np.arange(rows):
        for column in np.arange(columns):
            aligned += spins[row, column] == spins[(row + 1) % rows, column]
            aligned += spins[row, column] == spins[row, (column + 1) % columns]
            aligned += spins[row, column] == spins[(row - 1) % rows, column]
            aligned += spins[row, column] == spins[row, (column - 1) % columns]

    aligned = aligned / 4

    entropy = rows * np.log(rows)
    entropy -= aligned * np.log(aligned) 
    entropy -= (spins.size - aligned) * np.log(spins.size - aligned)
    
    return entropy
        
number = 10         #  size of the lattice, N x N
warm_up = 10**2       #  number of MC sweeps for equilibration
steps = 10**4  #  number of MC sweeps for calculation


temperatures = np.linspace(1.53, 3.28, 30); 
energies = np.zeros((3, temperatures.size, 2), dtype=float)
entropies = np.zeros((3, temperatures.size, 2), dtype=float)
free_energies = np.zeros((3, temperatures.size, 2), dtype=float)
magnetisations = np.zeros((3, temperatures.size, 2), dtype=float)
heat_capacities = np.zeros((3, temperatures.size, 2), dtype=float)

q2a = plt.figure(figsize=(9, 6))

for index, size in enumerate([10, 20, 100]):
    for temp, temperature in enumerate(temperatures):
        spins = initial_state(number)         # initialise

        if temp % 10 == 0 and size == 100:
            plt.subplot(2, 3, 2 * (temp // 10) + 1)
            plt.axis("tight")
            plt.imshow(spins)
            plt.title(f"T = {temperature} Initial")

        energy = np.zeros((steps,), dtype=float)
        entropy = np.zeros((steps,), dtype=float)
        magnetisation = np.zeros((steps,), dtype=float)

        for _ in range(warm_up):         # equilibrate
            mcmove(spins, temperature)           # Monte Carlo moves

        if temp % 10 == 0 and size == 100:
            plt.subplot(2, 3, 2 * (temp // 10) + 2)
            plt.axis("tight")
            plt.imshow(spins)
            plt.title(f"T = {temperature} Final")

        for i in range(steps):
            mcmove(spins, temperature)           
            
            energy[i] = calc_energy(spins)
            entropy[i] = calc_entropy(spins)
            magnetisation[i] = calc_magnetisation(spins)

        # divide by number of sites and iteractions to obtain intensive values 
        _energy = np.mean(energy)
        _energy_var = np.std(energy)
        _entropy = np.mean(entropy)
        _entropy_var = np.std(entropy)

        energies[index, temp, 0] = _energy / number ** 2
        energies[index, temp, 1] = _energy_var / number ** 2
        entropies[index, temp, 0] = _entropy / number ** 2
        entropies[index, temp, 1] = _entropy_var / number ** 2
        free_energies[index, temp, 0] = (_energy - temperature * _entropy) / number ** 2
        free_energies[index, temp, 1] = (_energy_var - temperature * _entropy_var) / number ** 2
        magnetisations[index, temp, 0] = np.abs(np.mean(magnetisation)) / number ** 2
        magnetisations[index, temp, 1] = np.std(np.abs(magnetisation)) / number ** 2
        heat_capacities[index, temp, 0] = (_energy_var ** 2) / number / temperature ** 2
        heat_capacities[index, temp, 1] = (np.std(energy ** 2) + 2 * _energy_var) / number ** 3 / temperature ** 2

plt.savefig("pub/figures/q2a.pdf")
plt.show()

f = plt.figure(figsize=(18, 10))

for size in range(3):
    plt.subplot(5, 3, size + 1)
    plt.errorbar(temperatures, energies[size, :, 0], energies[size, :, 1], marker='o')
    plt.xlabel("Temperature (T)", fontsize=20)
    plt.ylabel("Energy", fontsize=20)         
    plt.axis('tight')

    plt.subplot(5, 3, size + 4)
    plt.errorbar(temperatures, magnetisations[size, :, 0], magnetisations[size, :, 1], marker='o')
    plt.xlabel("Temperature (T)", fontsize=20) 
    plt.ylabel("Magnetization", fontsize=20)   
    plt.axis('tight')

    plt.subplot(5, 3, size + 7)
    plt.errorbar(temperatures, heat_capacities[size, :, 0], heat_capacities[size, :, 1], marker='o')
    plt.xlabel("Temperature (T)", fontsize=20)  
    plt.ylabel("Specific Heat", fontsize=20)   
    plt.axis('tight') 

    plt.subplot(5, 3, size + 10)
    plt.errorbar(temperatures, entropies[size, :, 0], entropies[size, :, 1], marker='o')
    plt.xlabel("Temperature (T)", fontsize=20)
    plt.ylabel("Entropy", fontsize=20)
    plt.axis('tight')

    plt.subplot(5, 3, size + 13)
    plt.errorbar(temperatures, free_energies[size, :, 0], free_energies[size, :, 1], marker='o')
    plt.xlabel("Temperature (T)", fontsize=20)
    plt.ylabel("Free Entropy", fontsize=20)
    plt.axis('tight')
plt.savefig("pub/figures/q2c.pdf")
plt.show()

sizes = [10, 20, 100]

plt.figure(figsize=(9, 9))
for size in range(3):
    for temp, temperature in enumerate([0, 10, 20]):
        plt.subplot(2, 3, size + temp + 1)
        plt.hist(magnetisations[size, temperature, 0])
        plt.title("T = {temperatures[temperature]}, N = {sizes[size]}")
        plt.axis("tight")

plt.savefig("pub/figures/q2e.pdf")
plt.show()
