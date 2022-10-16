import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt 

mpl.rcParams['text.usetex'] = True


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
    
    return systems, epsilons, temperatures, magnetic_fields

with open("pub/data/external_field.txt") as field:
    systems, epsilons, temperatures, magnetic_fields = _parse_ising_system(field)



index = 0
for epsilon in range(3):
    for magnetic_field in range(3):
        figure, axes = plt.subplots(3, 2, figsize=(10, 15))
        for temperature in range(3):
            axes[temperature][0].set_title(r"$\epsilon = {}, B = {}, \tau = {}$".format(
                epsilons[index], magnetic_fields[index], temperatures[index]))
            axes[temperature][0].imshow(systems[index])

            index += 1

            axes[temperature][1].set_title(r"$\epsilon = {}, B = {}, \tau = {}$".format(
                epsilons[index], magnetic_fields[index], temperatures[index]))
            axes[temperature][1].imshow(systems[index])

            index += 1
        plt.show()

