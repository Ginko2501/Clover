import numpy as np
import matplotlib.pyplot as plt

# blue chanel values
blue = [0.3,
        0.394161,
        0.434773,
        0.455128,
        0.471223,
        0.479728,
        0.484227,
        0.489959,
        0.493628,
        0.495355,
        0.496604,
        0.498904,
        0.497294,
        0.496107]

# loss function values
loss = [84.1839,
        34.8384,
        23.2385,
        19.1421,
        16.9121,
        16.7172,
        16.6916,
        16.1562,
        15.9436,
        16.0524,
        15.9071,
        16.012,
        16.1771,
        16.0458]

# gradients
gradient = [-941.605,
            -406.129,
            -203.549,
            -160.95,
            -85.0494,
            -44.9883,
            -57.3227,
            -36.689,
            -17.2691,
            -12.4863,
            -23.0064,
            16.1034,
            11.866,
            -7.32651]

range = range(14)

fig, ax1 = plt.subplots()
ax1.plot(blue, color='blue', label='blue')
ax1.legend()

ax2 = ax1.twinx()
ax2.plot(loss, color='black', label='loss')
ax2.legend()

# ax3 = ax1.twinx()
# ax3.plot(gradient, color='red', label='gradient')

# ax3.legend()

plt.show()

