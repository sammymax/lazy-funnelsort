import numpy as np
from subprocess import call
import os

for arr_size in range(100):
    method = 0 #funnelsort
    stream = os.popen("./test_parameter.out %d %d"%(arr_size, method))
    stream = stream.lines()
    print(stream)

