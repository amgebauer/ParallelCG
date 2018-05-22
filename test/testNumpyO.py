import numpy as np
import sys

arr = np.load("test/data/testarrout.npy")

if not (arr.shape==(1000,)):
    print("numpy out not passed (expected length to be (1000,) but actually is {0}".format(arr.shape))
    sys.exit(1)

for i in range(0,1000):
    if abs(i-arr[i]) > 1e-10:
        print("numpy out not passed at element {0} (expected to be {1} but actually is {2}".format(i, i*1.0, arr[i]))
        sys.exit(1)


arr = np.load("test/data/testarroutparallel.npy")

if not (arr.shape==(1000,)):
    print("numpy out not passed (expected length to be (1000,) but actually is {0}".format(arr.shape))
    sys.exit(1)

for i in range(0,1000):
    if abs(i-arr[i]) > 1e-10:
        print("numpy out not passed at element {0} (expected to be {1} but actually is {2}".format(i, i*1.0, arr[i]))
        sys.exit(1)

print("numpy passed")