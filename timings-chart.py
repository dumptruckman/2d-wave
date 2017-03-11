import matplotlib.pyplot as plt
import numpy as np
import asciitable as at

timingsData = at.read("../timings.txt")
timingsData.dtype.names = "program", "optimizationLevel", "N", "fEnd", "commSize", "elapsedTime"

noOptimization = timingsData[np.logical_and(np.logical_and(timingsData["N"] == 3840, timingsData["optimizationLevel"] == "NA"), timingsData["commSize"] != 128)]
noOptimization.sort(order="commSize")
o1Optimization = timingsData[np.logical_and(np.logical_and(timingsData["N"] == 3840, timingsData["optimizationLevel"] == "O1"), timingsData["commSize"] != 128)]
o1Optimization.sort(order="commSize")
o2Optimization = timingsData[np.logical_and(np.logical_and(timingsData["N"] == 3840, timingsData["optimizationLevel"] == "O2"), timingsData["commSize"] != 128)]
o2Optimization.sort(order="commSize")
o3Optimization = timingsData[np.logical_and(np.logical_and(timingsData["N"] == 3840, timingsData["optimizationLevel"] == "O3"), timingsData["commSize"] != 128)]
o3Optimization.sort(order="commSize")

plt.figure(num=1, figsize=(11, 8.5))
noOptLine, = plt.plot(noOptimization["commSize"], noOptimization["elapsedTime"], "b-")
o1Line, = plt.plot(o1Optimization["commSize"], o1Optimization["elapsedTime"], "r-")
o2Line, = plt.plot(o2Optimization["commSize"], o2Optimization["elapsedTime"], "g-")
o3Line, = plt.plot(o3Optimization["commSize"], o3Optimization["elapsedTime"], "y-")
plt.legend([noOptLine, o1Line, o2Line, o3Line], ["No Optimization", "O1", "O2", "O3"])
plt.xticks([4, 8, 16, 32, 64])
plt.yticks([2, 3, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36])
plt.xlabel("# of Processes")
plt.ylabel("Elapsed time in seconds")
plt.title("Effects of Compiler Optimization")
plt.savefig("optimization-effects.png")
plt.show()
plt.cla()
plt.clf()

nTiming = timingsData[timingsData["N"] != 3840]
nTiming.sort(order="N")

plt.figure(num=2, figsize=(11, 8.5))
plt.plot(nTiming["N"], nTiming["elapsedTime"])
plt.xticks([640, 2560, 5120, 10240, 20480, 30720, 40960, 61440, 81920], rotation="vertical")
plt.xlabel("N value")
plt.yticks([13, 54, 126, 276, 536, 887])
plt.ylabel("Elapsed time in seconds")
plt.title("Large N Values on 128 Cores")
plt.tight_layout()
plt.savefig("large-n-values.png")
plt.show()
