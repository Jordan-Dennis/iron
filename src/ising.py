import jax.numpy as np
import matplotlib.pyplot as pyplot
import jax.random as rand

number = 100

spins = rand.uniform(rand.PRNGKey(0), (number, )) > .5 

for spin in spins:

