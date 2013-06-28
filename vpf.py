import numpy as np
import _vpf


dat = np.random.rand(250, 3)

rad = np.arange(0.0001, 0.35, 0.0001)

x = dat[:, 0]
y = dat[:, 1]
z = dat[:, 2]

P_0 = _vpf.vpf(rad, x, y, z)


# Plotting

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import rc
rc('font', **{'family': 'sans-serif', 'sans-serif': ['Helvetica']})
rc('text', usetex=True)

fig = plt.figure()
ax = fig.add_subplot(111)
l, = ax.loglog(rad, P_0)

ax.set_title(r"Void probability function of random distribution of points")
ax.set_xlabel(r"$log[r]$ $(fraction of box size)$")
ax.set_ylabel(r"$log[P_{0}(r)]$")

fig.savefig("/home/kilian/public_html/tinker/poisson_sim.png")
