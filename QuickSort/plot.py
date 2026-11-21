import matplotlib.pyplot as plt
import numpy as np

n, insertion, quick = np.loadtxt("result.csv", delimiter=",", skiprows=1, unpack=True)

plt.plot(n, insertion, label="Сортировка вставками")
plt.plot(n, quick, label="QuickSort")
plt.xlabel("Длина массива")
plt.ylabel("Среднее время сортировки, мс")
plt.legend()
plt.grid(True)
plt.savefig("result.png", dpi=150)
plt.show()
