import numpy as np 
import matplotlib.pyplot as plt
import typing as tp


def print_tensor(tensor: list) -> None:
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



def first_and_last(data_file: str, show: bool, save_file: str = None) -> None:
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

        assert len(temperatures) == len(images)
        assert len(temperatures) % 2 == 0

        images = np.array(images, dtype=int)           
        number = images.shape[0]
        figure, axes = plt.subplots(2, number // 2, figsize=(10, 15))
        for subplot, index in enumerate(range(0, number, 2)):
            axes[0][subplot].imshow(images[index])
            axes[0][subplot].set_title(f"$T = {temperatures[index]}$")
            axes[0][subplot].set_xticks([])
            axes[0][subplot].set_yticks([])
            axes[1][subplot].imshow(images[index + 1])
            axes[1][subplot].set_title(f"$T = {temperatures[index + 1]}$")
            axes[1][subplot].set_xticks([])
            axes[1][subplot].set_yticks([])

        if show:
            plt.show()
        else:
            figure.savefig(f"pub/figures/{save_file}.pdf")


first_and_last("pub/data/2d_test.csv", True)
