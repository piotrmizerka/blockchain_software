#  This may be helpful: https://realpython.com/python-testing/#writing-your-first-test 

from sqlite3 import Row
import unittest
from pca import dataMatrix, pca_sklearn, pca_svd, saveTimeSeries

import os
import shutil
from random import seed
from random import randint
import random
from statistics import mean
import numpy as np

# TODO
class Test_DataMatrix(unittest.TestCase):
    
    def saveTestData(self, test_data, snapshots_dir_path):
        if os.path.exists(snapshots_dir_path) and os.path.isdir(snapshots_dir_path):
            shutil.rmtree(snapshots_dir_path)
        os.mkdir(snapshots_dir_path)

        features_number = len(test_data[0])
        for i in range(0,len(test_data)):
            data_file = open(snapshots_dir_path+"/"+str(i)+".dat", "w")
            for j in range(0, features_number):
                data_file.write(str(j)+" "+str(j)+" "+str(test_data[i][j])+"\n")
            data_file.close()

    # Don't check transformed data - checks only reading from file actually.
    # Otherwise, we would have to repeat the code of the tested function.
    def test_dataMatrix_loading(self): 
        directory = "pca_test_data"
        parent_dir = "./tst"
        snapshots_dir_path = os.path.join(parent_dir, directory) 

        if os.path.exists(snapshots_dir_path) and os.path.isdir(snapshots_dir_path):
            shutil.rmtree(snapshots_dir_path)
        os.mkdir(snapshots_dir_path)

        snapshots_number = 20
        features_number = 5*snapshots_number
        seed(randint(0,10))
        zero_rows_number = randint(0, snapshots_number-1)
        zero_rows_numbers = random.sample(range(0, snapshots_number), zero_rows_number)
        test_data = [[0]*features_number]*snapshots_number
        for i in range(0, snapshots_number):
            if i not in zero_rows_numbers:
                test_data[i] = [randint(1, 100) for k in range(0, features_number)]

        self.saveTestData(test_data, snapshots_dir_path)

        original_data, transformed_data = dataMatrix(snapshots_dir_path)

        self.assertEqual(original_data, [list(map(float,l)) for l in test_data])

        shutil.rmtree(snapshots_dir_path)
    
    def test_dataMatrix(self):
        directory = "pca_test_data"
        parent_dir = "./tst"
        snapshots_dir_path = os.path.join(parent_dir, directory) 

        snapshots_number = 4
        features_number = 4
        seed(randint(0,10))
        test_data = [[0]*features_number]*snapshots_number
        for i in range(0, snapshots_number-1): # the last row is zero
            test_data[i] = [randint(1, 100) for k in range(0, features_number)]

        self.saveTestData(test_data, snapshots_dir_path)
         
        original_data, transformed_data = dataMatrix(snapshots_dir_path)

        self.assertEqual(original_data, test_data)
        ar = average_rows = [[test_data[i][j]/sum(test_data[i]) for j in range(0,4)] for i in range(0,3)]
        ac = average_columns = [mean([average_rows[i][j] for i in range(0,3)]) for j in range(0,4)]
        proper_transformed_data = [[ar[0][0]-ac[0], ar[0][1]-ac[1], ar[0][2]-ac[2], ar[0][3]-ac[3]],
                                   [ar[1][0]-ac[0], ar[1][1]-ac[1], ar[1][2]-ac[2], ar[1][3]-ac[3]],
                                   [ar[2][0]-ac[0], ar[2][1]-ac[1], ar[2][2]-ac[2], ar[2][3]-ac[3]],
                                   [0.0, 0.0, 0.0, 0.0]]
        self.assertEqual(np.round(transformed_data,2).all(), np.round(proper_transformed_data,2).all())
        
        shutil.rmtree(snapshots_dir_path)

class Test_pca_sklearn(unittest.TestCase):

    def test_sum(self):
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

    def test_sum_tuple(self):
        self.assertEqual(sum((1, 2, 3)), 6, "Should be 6")

class Test_pca_svd(unittest.TestCase):

    def test_sum(self):
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

    def test_sum_tuple(self):
        self.assertEqual(sum((1, 2, 3)), 6, "Should be 6")

class Test_saveTimeSeries(unittest.TestCase):

    def test_sum(self):
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

    def test_sum_tuple(self):
        self.assertEqual(sum((1, 2, 3)), 6, "Should be 6")


if __name__ == '__main__':
    unittest.main()
