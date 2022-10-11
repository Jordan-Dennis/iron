import matplotlib.pyplot as plt
import numpy as np 

def external_magnetic_field(data_file: str, show: bool, save_file: str = None) -> None:
    with open(f"pub/data/{data_file}", "r") as phases:
        headers = next(phases)
        data = np.array([[float(entry) for entry in line.strip().split(",")] for line in phases])

    temperatures = np.unique(data[:, 0])
    magnetic_fields = np.unique(data[:, 1])
    magnetisations = data[:, 2].reshape((magnetic_fields.size, -1))
    energies = data[:, 3].reshape((magnetic_fields.size, -1))
    critical_temperature = 2 / np.log(1 + np.sqrt(2))
    maximum_temperature = 2. * critical_temperature

    ylabels = [f"{temperature}" for temperature in temperatures]
    xlabels = [f"{magnetic_field}" for magnetic_field in magnetic_fields] 

    figure, axes = plt.subplots(1, 2, figsize=(8, 3))
    mag_im = axes[0].imshow(magnetisations)
    axes[0].set_xticks(np.arange(magnetic_fields.size), labels = xlabels, rotation='vertical')
    axes[0].set_yticks(np.arange(magnetic_fields.size)[::-1], labels = ylabels)
    axes[1].axhline(critical_temperature *  magnetic_fields.size / maximum_temperature)
    plt.colorbar(mag_im)
    eng_im = axes[1].imshow(energies)
    axes[1].set_xticks(np.arange(magnetic_fields.size), labels = xlabels, rotation='vertical')
    axes[1].set_yticks(np.arange(magnetic_fields.size)[::-1], labels = ylabels)
    axes[1].axhline(critical_temperature *  magnetic_fields.size / maximum_temperature)
    plt.colorbar(eng_im) 
    
    if show: plt.show()


external_magnetic_field("external_field.csv", True)
