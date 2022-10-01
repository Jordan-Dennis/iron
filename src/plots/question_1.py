import numpy as np
import matplotlib.pyplot as plt


with open("pub/data/1d_test.csv") as question_1a_data:
    data = []
    for line in question_1a_data:
        if not line.strip().startswith("#"):
            data.append([int(spin) for spin in line.strip().split(",")])

    data = np.array([data])[0]

print(data.shape)

plt.figure(figsize=(10, 10))
for temperature in range(3):
    plt.subplot(3, 1, temperature + 1)
    plt.title(temperature)
    plt.imshow(data[temperature:temperature + 2, :])

plt.show()

