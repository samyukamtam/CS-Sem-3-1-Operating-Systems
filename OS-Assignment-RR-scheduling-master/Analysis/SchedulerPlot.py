import numpy as np
import pandas as pd
import sys
import matplotlib.pyplot as plt

data = pd.read_csv(sys.argv[1])
data = data.to_numpy()

X = data[:, :1]
Y1 = data[:, 1]
Y1 = np.reshape(Y1, (-1, 1))
Y2 = data[:, 2]
Y2 = np.reshape(Y2, (-1, 1))
plt.plot(X, Y1)
plt.xlabel("Workload size")
plt.ylabel("Turnaround time")
plt.title("Turnaround time vs Workload size for P1 for Quanta 2 ms")
plt.savefig("turn_2_P1.jpg")
plt.clf()

plt.plot(X, Y2)
plt.xlabel("Workload size")
plt.ylabel("Turnaround time")
plt.title("Turnaround time vs Workload size for P2 for Quanta 2 ms")
plt.savefig("turn_2_P2.jpg")