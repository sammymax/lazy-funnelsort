import numpy as np
from subprocess import call
import subprocess
import os
import csv

os.popen("g++ --std=c++11 test_parameter.cpp -o test_parameter.o") #compiles the source
L1sz = 32768
LLsz = 4199304
B = 64
arr_size_mult = 4 #since we're using 32 bit integers

funnelsort_res_file = 'funnelsort_results.csv'
stdsort_res_file = 'stdsort_results.csv'
funnelsort_writer = csv.writer(open(funnelsort_res_file, 'w'))
stdsort_writer = csv.writer(open(stdsort_res_file, 'w'))

for arr_size in range(98, 100):
    for method in [0, 1]: #0 is funnelsort, 1 is std sort
        print("Currently processing arr size %d, method %d"%(arr_size, method))
        stream = subprocess.Popen("valgrind --tool=cachegrind ./test_parameter.o %d %d"%(arr_size, method), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout = stream.stdout.read().decode('utf-8')
        stderr = stream.stderr.read().decode('utf-8')
        number = stderr.splitlines()[0].split("==")[1]
        #print(number)
        cachegrind_name = 'cachegrind.out.'+number
        cachegrind_output = open(cachegrind_name, 'r')
        output_lines = cachegrind_output.read().splitlines()
        last_line = output_lines[-1]
        summary_nums = list(map(int, last_line.split(" ")[1:]))
        print(summary_nums)
        Irefs, IL1miss, ILLmiss, DataReadref, DataReadL1miss, DataReadLLmiss, DataWriteref, DataWriteL1miss, DataWriteLLmiss = summary_nums
        TotDataref = DataReadref + DataWriteref
        TotDataL1miss = DataReadL1miss + DataWriteL1miss
        TotDataLLmiss = DataReadLLmiss + DataWriteLLmiss
        print(float(TotDataL1miss)/TotDataref)
        print(float(TotDataLLmiss)/TotDataref)
        res_write = funnelsort_writer 
        if method == 1:
            res_write = stdsort_writer
        res_write.writerow([arr_size, TotDataref, TotDataL1miss, TotDataLLmiss])




