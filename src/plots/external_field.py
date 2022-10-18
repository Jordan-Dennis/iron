import sys
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt 

mpl.rcParams["text.usetex"] = True

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




def snapshots(data_file: str, show: bool, save_file: list = None) -> None:
    mpl.rcParams["figure.figsize"] = (8, 10)
    mpl.rcParams["figure.subplot.hspace"] = 0.1
    mpl.rcParams["figure.subplot.wspace"] = 0.0

    with open(f"pub/data/{data_file}") as field:
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

        if show:
            plt.show()

        if save_file:
            if not isinstance(save_file, list):
                raise ValueError("Three save files must be provided")

            if len(save_file) != 3:
                raise ValueError("Three save files must be provided")

            figure.savefig(f"pub/figures/{save_file[epsilon]}")


def physical_parameters(data_file: str, show: bool, save_file: str = None) -> None:
    with open(f"pub/data/{data_file}") as parameters:
        headers = next(parameters)
        data = [[float(i) for i in line.strip().split(",")] for line in parameters]
        data = np.array(data)

    fig, axes = plt.subplots(5, 3, sharex = "col", sharey = "row")
    for _epsilon in range(3):
        epsilon = _epsilon - 1
        epsilon_cond = data[:, 0] == epsilon
        for magnetic_field in range(3):
            field_cond = data[:, 1] == magnetic_field
            subset = data[epsilon_cond & field_cond, :]
            axes[0][_epsilon].errorbar(subset[:, 2], subset[:, 3], subset[:, 4])
            axes[1][_epsilon].errorbar(subset[:, 2], subset[:, 5], subset[:, 6])
            axes[2][_epsilon].errorbar(subset[:, 2], subset[:, 7], subset[:, 8])
            axes[3][_epsilon].errorbar(subset[:, 2], subset[:, 9], subset[:, 10])
            axes[4][_epsilon].plot(subset[:, 2], subset[:, 11])
            
    axes[0][0].set_ylabel(r"$\textrm{Energy} (J)$")
    axes[1][0].set_ylabel(r"$\textrm{Entropy} (JK^{-1})$")
    axes[2][0].set_ylabel(r"$\textrm{Free Energy} (J)$")
    axes[3][0].set_ylabel(r"$\textrm{Magnetisation}$")
    axes[4][0].set_ylabel(r"$\textrm{Heat Capacity} (JK^{-1})$")
    axes[4][0].set_xlabel(r"$\textrm{Temperature} (J)$")
    axes[4][1].set_xlabel(r"$\textrm{Temperature} (J)$")
    axes[4][2].set_xlabel(r"$\textrm{Temperature} (J)$")
    axes[0][0].set_title(r"$\epsilon = -1$")
    axes[0][1].set_title(r"$\epsilon = 0$")
    axes[0][2].set_title(r"$\epsilon = 1$")
    fig.legend(["$B = 0$", "$B = 1$", "$B = 2$"])
    plt.show()


def main(mode: str) -> None:
    if mode == "snapshots":
        save_files = ["external_field_epsilon_minus_one.pdf", 
            "external_field_epsilon_zero.pdf", "external_field_epsilon_one.pdf"]
        snapshots("external_field.txt", True, save_files)

    elif mode == "physical_parameters":
        physical_parameters("external_field.csv", True, "physical_parameters_external_field.pdf")

option = sys.argv[1]
main(option)
