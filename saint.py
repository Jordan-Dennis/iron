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


temperatures = np.linspace(1.53, 3.28, 32); 
energies = np.zeros((temperatures.size, 2), dtype=float)
entropies = np.zeros((temperatures.size, 2), dtype=float)
free_energies = np.zeros((temperatures.size, 2), dtype=float)
magnetisations = np.zeros((temperatures.size, 2), dtype=float)
heat_capacities = np.zeros((temperatures.size, 2), dtype=float)

for temp, temperature in enumerate(temperatures):
    spins = initial_state(number)         # initialise

    energy = np.zeros((steps,), dtype=float)
    entropy = np.zeros((steps,), dtype=float)
    magnetisation = np.zeros((steps,), dtype=float)

    for _ in range(warm_up):         # equilibrate
        mcmove(spins, temperature)           # Monte Carlo moves

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
   
    energies[temp, 0] = _energy / number
    energies[temp, 1] = _energy_var / number
    entropies[temp, 0] = _entropy / number
    entropies[temp, 1] = _entropy_var / number
    free_energies[temp, 0] = (_energy - temperature * _entropy) / number
    free_energies[temp, 1] = (_energy_var - temperature * _entropy_var) / number
    magnetisations[temp, 0] = np.abs(np.mean(magnetisation)) / number
    magnetisations[temp, 1] = np.std(np.abs(magnetisation)) / number
    heat_capacities[temp, 0] = (_energy_var ** 2) / number / temperature ** 2
    heat_capacities[temp, 1] = (np.std(energy ** 2) - 2 * _energy_var) / number / temperature ** 2
#  plot the calculated values   
#----------------------------------------------------------------------

f = plt.figure(figsize=(18, 10))

sp =  f.add_subplot(2, 2, 1)
plt.errorbar(temperatures, energies[:, 0], energies[:, 1], s=50, marker='o')
plt.xlabel("Temperature (T)", fontsize=20)
plt.ylabel("Energy", fontsize=20)         
plt.axis('tight')

sp =  f.add_subplot(2, 2, 2)
plt.scatter(temperatures, magnetisations[:, 0], magnetisations[:, 1], s=50, marker='o')
plt.xlabel("Temperature (T)", fontsize=20) 
plt.ylabel("Magnetization", fontsize=20)   
plt.axis('tight')

sp =  f.add_subplot(2, 2, 3)
plt.scatter(temperatures, heat_capacities[:, 0], heat_capacities[:, 1], s=50, marker='o')
plt.xlabel("Temperature (T)", fontsize=20)  
plt.ylabel("Specific Heat", fontsize=20)   
plt.axis('tight') 

sp =  f.add_subplot(2, 2, 4)
plt.scatter(temperatures, entropies[:, 0], entropies[:, 1], s=50, marker='o')
plt.xlabel("Temperature (T)", fontsize=20)
plt.ylabel("Entropy", fontsize=20)
plt.axis('tight')
plt.savefig("pub/figures/q2c.pdf")
plt.show()
