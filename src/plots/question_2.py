import numpy as np 
import matplotlib.pyplot as plt
import typing as tp


def print_tensor(tensor: list) -> None:
    def tensor_depth(tensor: list) -> int:
        def _tensor_depth(tensor: tp.Union[list, object], depth: int) -> int:
            if isinstance(tensor, list):
                return depth
            else:
                return _tensor_depth(tensor[0], depth + 1)
        return _tensor_depth(tensor, 0)

    depth = tensor_depth(tensor, 0)
    for outer in range(len(tensor)):
        print()


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

        print(len(temperatures))
        print(len(images))
        print(images)

        assert len(temperatures) == len(images)
        assert len(temperatures) % 2 == 0



        images = np.array(images, dtype=int)           
        number = images.shape[0]
        figure, axes = plt.subplots(2, number // 2, figsize=(10, 15))
        for subplot in range(number):
            axes[subplot].imshow(images[subplot])
            axes[subplot].set_title(f"$T = {temperatures[subplot]}")
            axes[subplot].xticks([])

        if show:
            plt.show()
        else:
            figure.savefig(f"pub/figures/{save_file}.pdf")


first_and_last("pub/data/2d_test.csv", True)
