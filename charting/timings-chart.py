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

noOptLine, = plt.plot(noOptimization["commSize"], noOptimization["elapsedTime"], "b-")
o1Line, = plt.plot(o1Optimization["commSize"], o1Optimization["elapsedTime"], "r-")
o2Line, = plt.plot(o2Optimization["commSize"], o2Optimization["elapsedTime"], "g-")
o3Line, = plt.plot(o3Optimization["commSize"], o3Optimization["elapsedTime"], "y-")
plt.legend([noOptLine, o1Line, o2Line, o3Line], ["No Optimization", "O1", "O2", "O3"])
plt.xticks([4, 8, 16, 32, 64])
plt.yticks([2, 3, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36])
plt.xlabel("# of Processes")
plt.ylabel("Elapsed Time in seconds")
plt.title("Effects of Compiler Optimization")
plt.show()

nTiming = timingsData[timingsData["N"] != 3840]

plt.plot(nTiming["N"], nTiming["elapsedTime"])
plt.show()
