import numpy as np


# generate random positive definite, symmetric matrix
tmpmat = np.random.random((100, 100))
A = tmpmat*tmpmat.transpose()+0.01*np.eye(100,100)

# generate random vector
b = np.random.random((100))


# save matrix and vector
np.save("test/data/genmatrix.npy", A)
np.save("test/data/genvector.npy", b)