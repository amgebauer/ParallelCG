import numpy as np
import argparse


parser = argparse.ArgumentParser()
parser.add_argument("-N", help="Problem size", type=int)
parser.add_argument("-A", help="Path to matrix")
parser.add_argument("-v", help="Path to vector")
args = parser.parse_args()

N = args.N
pathToMat = args.A
pathToVec = args.v

# generate random positive definite, symmetric matrix
tmpmat = np.random.random((N, N))
A = tmpmat*tmpmat.transpose()+0.01*np.eye(N,N)

# generate random vector
b = np.random.random((N))


# save matrix and vector
np.save(pathToMat, A)
np.save(pathToVec, b)