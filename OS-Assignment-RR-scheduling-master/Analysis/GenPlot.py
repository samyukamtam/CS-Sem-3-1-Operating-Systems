import numpy as np
import sys
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv(sys.argv[1])
data = data.to_numpy()

X = data[:, :1]
Y = data[:, 1]
Y = np.reshape(Y, (-1, 1))
plt.plot(X, Y)
plt.xlabel("No.of threads")
plt.ylabel("Time (in nanoseconds)")
plt.title("An analysis of no.of threads vs execution time for P2")
plt.savefig("P2_plot_1000.jpg")
# plt.show()