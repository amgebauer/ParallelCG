import numpy as np
import numpy.linalg as la
import sys

A = np.load("test/data/genmatrix.npy")
b = np.load("test/data/genvector.npy")
solution = np.load("test/data/gensolution.npy")


norm = la.norm(A.dot(solution)-b)
if norm > 1e-5:
    print("Test not passed: {0}".format(norm))
    sys.exit(1)

print("Test passed")