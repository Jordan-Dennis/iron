import numpy as np 
import matplotlib.pyplot as plt
import typing as tp
import matplotlib as mpl
import collections
import sys

mpl.rcParams['text.usetex'] = True

def print_tensor(tensor: list) -> None:
    """
    A debugging function that prints out a tensor of arbitrary shape 
    and nesting. 

    parameters
    ----------
    tensor: list
        The tensor to print.
    """
    def tensor_depth(tensor: list) -> int:
        def _tensor_depth(tensor: tp.Union[list, object], depth: int) -> int:
            if not isinstance(tensor[0], list):
                return depth
            else:
                return _tensor_depth(tensor[0], depth + 1)
        return _tensor_depth(tensor, 1)

    max_depth = tensor_depth(tensor)

    def _print_tensor(tensor: list, depth: int) -> None:
        indentation = (max_depth - depth) * "  "
        if depth == 1:
            print(indentation + str(tensor))
        else:
            print(indentation + "[")
            for inner in tensor:
                _print_tensor(inner, depth - 1)
            print(indentation + "]")

    _print_tensor(tensor, max_depth)


def _get_ising_image(data_file: str) -> tuple:
    """
    Load an ising model from a file of many saved images.

    parameters
    ----------
    data_file: str
        The file to search for the ising images.

    returns 
    -------
    images: tuple
        A highly nested tuple with the structure ((temperature, image)*)
    """
    with open(data_file) as images_file:
        image = []
        images = []
        temperatures = []
        for line in images_file:
            if line.strip().startswith("#"):
                temperature = []
                for char in line.strip():
                    if char.isnumeric() or char == '.':
                        temperature.append(char)
                temperatures.append(float("".join(temperature)))
                if image:
                    images.append(image)
                    image = []
            else:
                image.append([int(entry) for entry in line.strip().split(",")])
        if image:
            images.append(image)

        return tuple(zip(temperatures, images))


def first_and_last(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the first and last states of three different evolved ising system
    models.

    parameters
    ----------
    data_file: str
        The data file to retrieve the data from.
    show: bool
        True if the plot is to be shown else False.
    save_file: str = None
        The name of the file to save the image in. 
    """
    with open(f"pub/data/{data_file}") as images_file:
        image = []
        images = []
        temperatures = []
        for line in images_file:
            if line.strip().startswith("#"):
                temperature = []
                for char in line.strip():
                    if char.isnumeric() or char == '.':
                        temperature.append(char)
                temperatures.append(float("".join(temperature)))
                if image:
                    images.append(image)
                    image = []
            else:
                image.append([int(entry) for entry in line.strip().split(",")])
        if image:
            images.append(image)
    # TODO: Upgrade this to use the new functionality. 

    assert len(temperatures) == len(images)
    assert len(temperatures) % 2 == 0

    images = np.array(images, dtype=int)           
    number = images.shape[0]
    figure, axes = plt.subplots(2, number // 2)
    for subplot, index in enumerate(range(0, number, 2)):
        axes[0][subplot].imshow(images[index])
        axes[0][subplot].set_title(f"$T = {temperatures[index]}$")
        axes[0][subplot].set_xticks([])
        axes[0][subplot].set_yticks([])
        axes[1][subplot].imshow(images[index + 1])
        axes[1][subplot].set_xticks([])
        axes[1][subplot].set_yticks([])
    figure.tight_layout()

    if show:
        plt.show()

    if save_file:
        figure.savefig(f"pub/figures/{save_file}")


def physical_parameters(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the physical parameters of the system against temperature.

    parameters
    ----------
    data_file: str
        The file to search for the data in.
    show: bool 
        True if the plot should be shown else false.
    save_file: str
        The file to save the image. 
    """
    with open(f"pub/data/{data_file}") as phys_param:
        next(phys_param)
        data = [[float(entry) for entry in line.strip().split(",")] 
            for line in phys_param]
        data = np.array(data, dtype=float)

    temperature = data[:, 1]
    numbers = data[:, 0]
    energy_est = data[:, 2]
    energy_err = data[:, 3]
    entropy_est = data[:, 4]
    entropy_err = data[:, 5]
    free_energy_est = data[:, 6]
    free_energy_err = data[:, 7]
    heat_capacity_est = data[:, 8]
    heat_capacity_err = data[:, 9]

    figure, axes = plt.subplots(2, 2, figsize=(10, 10))
    axes[0][0].set_title("$\\textrm{Energy} (J)$")
    axes[0][1].set_title("$\\textrm{Entropy} (JK^{-1})$")
    axes[1][0].set_title("$\\textrm{Free Energy} (J)$")
    axes[1][1].set_title("$\\textrm{Heat Capacity} (JK^{-1})$")

    for number in np.unique(numbers):
        cond = numbers == number
        axes[0][0].errorbar(
            temperature[cond], 
            energy_est[cond], 
            energy_err[cond])

        axes[0][1].errorbar(
            temperature[cond], 
            entropy_est[cond], 
            entropy_err[cond])

        axes[1][0].errorbar(
            temperature[cond], 
            free_energy_est[cond], 
            free_energy_err[cond])

        axes[1][1].errorbar(
            temperature[cond], 
            heat_capacity_est[cond],
            heat_capacity_err[cond]) 
        
        axes[0][0].legend(["20", "50", "100"])
        axes[1][0].legend(["20", "50", "100"])
        axes[0][1].legend(["20", "50", "100"])
        axes[1][1].legend(["20", "50", "100"])



    if show:
        plt.show()
    if save_file:
        figure.savefig(f"pub/figures/{save_file}")


def magnetisation(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the magnetisation as a function temperature for multiple different 
    values of the spin.

    parameters
    ----------
    data_file: str
        The file to search the data for.
    show: bool
        True if the plot is to be shown else False.
    save_file: str
        The file to save the plot in.
    """
    with open(f"pub/data/{data_file}") as mag_data:
        data = np.array([[float(entry) for entry in line.strip().split(",")] 
            for line in mag_data])

    numbers = data[:, 0]
    temperatures = data[:, 1]
    neg_mag_est = data[:, 2] / numbers ** 2
    neg_mag_err = data[:, 3] / numbers ** 2
    pos_mag_est = data[:, 4] / numbers ** 2
    pos_mag_err = data[:, 5] / numbers ** 2
 
    figure = plt.figure(figsize = (10, 10))
    axes = plt.axes()
    axes.set_title(r"$\textrm{Magnetisation vs Temperature}$")

    for number in np.unique(numbers):
        cond = numbers == number
        axes.errorbar(temperatures[cond], neg_mag_est[cond], neg_mag_err[cond])
        axes.errorbar(temperatures[cond], pos_mag_est[cond], pos_mag_err[cond])

    if show:
        plt.show()
    elif save_file:
        figure.savefig(f"pub/data/{save_file}")


def heating_and_cooling(data_file: str, show: bool, save_file: str = None) -> None:
    """
    Plot the initial, middle and final states of a system as it is 
    heated and cooled. 

    parameters
    ----------
    data_file: str
        The file to search the data for.
    show: bool
        True if the plot is to be shown else False.
    save_file: str
        The file to save the plot in.
    """
    images = _get_ising_image(f"pub/data/{data_file}")

    figure, axes = plt.subplots(1, 3, figsize=(15, 5))

    for index, image in enumerate(images):
        axes[index].imshow(image[1])
        axes[index].set_title(r"$\textrm{T = " + f"{image[0]}" + "}$")

    if show:
        plt.show()

    if save_file:
        figure.savefig(f"pub/figures/{save_file}")


option = sys.argv[1]
exec(f"{option}('{option}_ising_2d.csv', True, '{option}_ising_2d.pdf')")
