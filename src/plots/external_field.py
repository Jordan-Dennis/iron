import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt 

mpl.rcParams["text.usetex"] = True
mpl.rcParams["figure.figsize"] = (8, 10)
mpl.rcParams["figure.subplot.hspace"] = 0.1
mpl.rcParams["figure.subplot.wspace"] = 0.0

def _print_tensor(tensor: list) -> None:
    def _tensor_depth(tensor: list) -> int:
        def __tensor_depth(tensor: list, depth: int) -> int:
            if not isinstance(tensor[0], list):
                return depth
            else:
                return __tensor_depth(tensor[0], depth + 1)
        return __tensor_depth(tensor, 1)

    max_depth = _tensor_depth(tensor)

    def __print_tensor(tensor: list, depth: int) -> None:
        indentation = (max_depth - depth) * "  "
        if depth == 1:
            print(indentation + str(tensor))
        else:
            print(indentation + "[")
            for inner in tensor:
                __print_tensor(inner, depth - 1)
            print(indentation + "]")

    __print_tensor(tensor, max_depth)


def _parse_ising_system(file: str) -> list:
    systems = []
    epsilons = []
    temperatures = []
    magnetic_fields = []

    system = []
    for line in file:
        if ":" in line:
            if system:
                systems.append(system)
            system = []
            if line.startswith("Epsilon"):
                epsilons.append(line.split(":")[-1].strip())
            elif line.startswith("Temperature"):
                temperatures.append(line.split(":")[-1].strip())
            elif line.startswith("Magnetic"):
                magnetic_fields.append(line.split(":")[-1].strip())
        else:
            system.append([int(i) for i in line.strip().strip(",").split(",")])
    systems.append(system)
    
    return systems, epsilons, temperatures, magnetic_fields

with open("pub/data/external_field.txt") as field:
    systems, epsilons, temperatures, magnetic_fields = _parse_ising_system(field)



index = 0
for epsilon in range(3):
    figure, axes = plt.subplots(3, 3)
    plt.subplots_adjust(wspace=0.0)
    figure.suptitle(r"$\epsilon = {0:.2f}$".format(float(epsilons[index])))
    for magnetic_field in range(3):
        for temperature in range(3):
            if magnetic_field == 0:
                ytitle = r"$\tau = {0:.2f}$".format(float(temperatures[index]))
                axes[temperature][magnetic_field].set_ylabel(ytitle)

            if temperature == 2:
                xtitle = r"$B = {0:.2f}$".format(float(magnetic_fields[index]))
                axes[temperature][magnetic_field].set_xlabel(xtitle)

            axes[temperature][magnetic_field].imshow(systems[index])
            axes[temperature][magnetic_field].set_xticks([])
            axes[temperature][magnetic_field].set_yticks([])
            index += 1
    plt.show()

