import numpy as np
import numpy.linalg as la
import argparse
import sys


parser = argparse.ArgumentParser()
parser.add_argument("-A", help="Path to matrix")
parser.add_argument("-v", help="Path to vector")
parser.add_argument("-x", help="Path to solution")
args = parser.parse_args()

pathToMat = args.A
pathToVec = args.v
pathToSol = args.x


A = np.load(pathToMat)
b = np.load(pathToVec)
solution = np.load(pathToSol)


norm = la.norm(A.dot(solution)-b)
if norm > 1e-5:
    print("Test not passed: {0}".format(norm))
    sys.exit(1)

print("Test passed")