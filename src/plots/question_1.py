import numpy as np
import maplotlib.pyplot as plt

with open("pub/data/1a.csv") as question_1a_data:
    question_1a_data = np.array([
        [int(spin) for spin in line.split(",")] for line in question_1a_data])

plt.figure(figsize=(10, 10))
for temperture in range(3):
    plt.subplot(3, 1, temperature + 1)
    plt.title(temperature)
    plt.imshow(question_1a_data[:2, :])

