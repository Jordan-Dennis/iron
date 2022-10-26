import sys
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt 

mpl.rcParams["text.usetex"] = True

def _print_tensor(tensor: list) -> None:
    """
    This is a fun little function that I used for debugging. It determines 
    the depth of an arbitrary tensor then using this result along with a 
    little bit of magic to print the tensor it was given. 

    parameters
    ----------
    tensor: list
        An arbitrarily nested list. 
    """
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
    """
    Used to decode the ising systems once they have been written to 
    a file. 

    parameters
    ----------
    file: str 
        The file that contains the encoded ising systems.
    """
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
    """
    Plot the systems frozen in time for a variety of parameters.

    parameters
    ----------
    data_file: str
        The file to retrieve the data from. 
    show: bool
        True if the result is to be displayed before saving else False. 
    save_file: str
        The location to save the plots to. 
    """
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


def _chi_squared(theoretical: float, data: float) -> float:
    """
    Given a model and data calculate how well the data fits the model. 

    parameters
    ----------
    theoretical: float 
        The model of the data. 
    data: float 
        The actual data that was recorded. 

    returns
    -------
    chi_squared: float 
        The chi squared value of the fit. 
    """
    return np.sum(((theoretical - data) / theoretical) ** 2)


def physical_parameters(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the physical parameters of the system as a function of the 
    temperature and the magnetic field. 

    parameters
    ----------
    data_file: str
        The file to retrieve the data from. 
    show: bool
        True if the result is to be displayed before saving else False. 
    save_file: str
        The location to save the plots to. 
    """
    with open(f"pub/data/{data_file}") as parameters:
        headers = next(parameters)
        data = [[float(i) for i in line.strip().split(",")] for line in parameters]
        data = np.array(data)

    def _energy(temperature: float, magnetic_field: float) -> float:
        return - magnetic_field * np.tanh(magnetic_field / temperature)

    def _entropy(temperature: float, magnetic_field: float) -> float:
        return (- _free_energy(temperature, magnetic_field) + \
            _energy(temperature, magnetic_field)) / temperature

    def _free_energy(temperature: float, magnetic_field: float) -> float:
        return - temperature * np.log(2 * np.cosh(magnetic_field / temperature))

    def _heat_capacity(temperature: float, magnetic_field: float) -> float:
        return magnetic_field / temperature ** 2 / \
            np.cosh(magnetic_field / temperature) ** 2

    def _magnetisation(temperature: float, magnetic_field: float) -> float:
        return - np.tanh(magnetic_field / temperature)

    fig, axes = plt.subplots(5, 3, sharex = "col", sharey = "row", figsize=(7.3, 10.3))
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
            
            if epsilon == 0:
                min_temp = subset[:,2].min()
                max_temp = subset[:,2].max()
                num_temp = 1000
                temperatures = np.linspace(min_temp, max_temp, num_temp)
                energy = _energy(temperatures, magnetic_field)
                entropy = _entropy(temperatures, magnetic_field)
                free_energy = _free_energy(temperatures, magnetic_field)
                heat_capacity = _heat_capacity(temperatures, magnetic_field)
                magnetisation = _magnetisation(temperatures, magnetic_field)

                axes[0][_epsilon].plot(temperatures, energy, "--")
                axes[1][_epsilon].plot(temperatures, entropy, "--")
                axes[2][_epsilon].plot(temperatures, free_energy, "--")
                axes[4][_epsilon].plot(temperatures, heat_capacity, "--")
                axes[3][_epsilon].plot(temperatures, magnetisation, "--")
            
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


    axes[0][1].plot()
    
    if show:
        plt.show()

    if save_file:
        fig.savefig(f"pub/figures/{save_file}")


def antiferromagnet(data_file: str, show: bool, save_file: str) -> None:
    """
    This is a function to animate the behaviour of the system as 
    a function of time. 

    parameters
    ----------
    data_file: str
        The file to retrieve the data from. 
    show: bool
        True if the result is to be displayed before saving else False. 
    save_file: str
        The location to save the plots to. 
    """
    with open(f"pub/data/{data_file}") as data:
        systems, epsilons, temperatures, magnetic_fields = _parse_ising_system(data)

    import matplotlib
    import matplotlib.pyplot as plt

    import matplotlib.animation as animation

    fps = 30
    nSeconds = 5

    fig = plt.figure()

    a = systems[0]
    im = plt.imshow(a, interpolation='none', aspect='auto', vmin=0, vmax=1)

    def animate_func(i):
        im.set_array(systems[i])
        fig.suptitle(f"$T = {temperatures[i]}, B = {magnetic_fields[i]}, \\epsilon = {epsilons[i]}$")
        return [im]

    anim = animation.FuncAnimation(
        fig, animate_func, frames = len(systems), interval = 1000 / fps)
    plt.show()


def main(mode: str) -> None:
    if mode == "snapshots":
        save_files = ["external_field_epsilon_minus_one.pdf", 
            "external_field_epsilon_zero.pdf", "external_field_epsilon_one.pdf"]
        snapshots("external_field.txt", True, save_files)

    elif mode == "physical_parameters":
        physical_parameters("external_field.csv", True, "physical_parameters_external_field.pdf")

    elif mode == "antiferromagnet":
        antiferromagnet("antiferromagnet.txt", True, None)

option = sys.argv[1]
main(option)
