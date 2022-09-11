import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt


def initial_state(size: int) -> int:   
    return 2 * np.random.randint(2, size=(size, 1)) - 1



def evolve(spins: int, temperature: float) -> int:
    # The first thing to notice is that they loop over the entrie system here.
    number = spins.size
    for spin in np.arange(number):
        spin = np.random.randint(0, number)
        neighbours = spins[(spin + 1) % number] + spins[(spin - 1) % number]
        cost = 2 * spins[spin] * neighbours
        
        if (cost < 0) or (rand() < np.exp(-cost / temperature)):
            spins[spin] *= -1

    return spins



def energy(spins: int):
    energy = 0 
    
    for spin in np.arange(number):
        neighbours = spins[(spin + 1) % number] + spins[(spin - 1) % number]
        energy += -neighbours * spins[spin]

    return energy/2.  # to compensate for over-counting



def calcMag(config):
    '''
    Magnetization of a given configuration
    '''
    mag = np.sum(
