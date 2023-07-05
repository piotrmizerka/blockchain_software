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

# check if columns of a (not necessarily square) matrix form an orthonormal basis
def orthogonality_test(A, self):
    At = A.transpose()
    for i in range(len(At)):
        norm = sum(coeff**2 for coeff in At[i])
        self.assertEqual(np.round(norm,2),1.0)
        for j in range(i):
            dot_product = sum(At[i][k]*At[j][k] for k in range(len(At[i])))
            self.assertEqual(np.round(dot_product,2),0.0)

def test_pca_svd(data_matrix,self):
    data_matrix = np.array(data_matrix)
    M, N = data_matrix.shape
    K = min(M,N)
    U, S, V = pca_svd(data_matrix)
    # dimension check
    self.assertEqual(U.shape,(M,K))
    self.assertEqual(S.shape,(K,))
    self.assertEqual(V.shape,(N,K))
    # singular values check
    explained_variance_ratio_, singular_values_, components_ = pca_sklearn(data_matrix,n_components=K)
    self.assertTrue(np.array_equal(
        np.round(S,2), 
        np.round(singular_values_,2)
    ))
    denominator = sum(s**2 for s in S)
    ratios = [s**2/denominator for s in S]
    self.assertTrue(np.array_equal(
        np.round(ratios,2), 
        np.round(explained_variance_ratio_,2)
    ))
    # orthogonality test
    orthogonality_test(U,self)
    orthogonality_test(V,self)

class Test_DataMatrix(unittest.TestCase):
    
    def saveTestData(self, test_data, snapshots_dir_path):
        if os.path.exists(snapshots_dir_path) and os.path.isdir(snapshots_dir_path):
            shutil.rmtree(snapshots_dir_path)
        os.mkdir(snapshots_dir_path)

        features_number = len(test_data[0])
        filling_size = len(str(len(test_data)))
        for i in range(0,len(test_data)):
            # we must arrange files' names so that they will be sorted exactly as they appear here
            data_file = open(snapshots_dir_path+"/"+str(i).zfill(filling_size)+".dat", "w")
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
    
    # The data for this test taken from here (occurs in other tests as well): 
    # https://scikit-learn.org/stable/modules/generated/sklearn.decomposition.PCA.html
    def test_pca_sklearn(self):
        X = np.array([[-1, -1], [-2, -1], [-3, -2], [1, 1], [2, 1], [3, 2]])
        explained_variance_ratio_, singular_values_, components_ = pca_sklearn(X,n_components=2)
        proper_ratios = [
            singular_values_[0]**2/(singular_values_[0]**2+singular_values_[1]**2),
            singular_values_[1]**2/(singular_values_[0]**2+singular_values_[1]**2)
        ]
        # explained variance ratio must sum up to one in l2 norm
        self.assertEqual(
            np.round(explained_variance_ratio_,2).all(),
            np.round(proper_ratios,2).all()
        )
        # principal components must form an orthonormal basis
        orthogonality_test(components_,self)

class Test_pca_svd(unittest.TestCase):

    def test_small_svd(self):
        X = [[-1, -1], [-2, -1], [-3, -2], [1, 1], [2, 1], [3, 2]]
        test_pca_svd(X,self)

    def test_big_svd(self):
        M = np.random.randint(1,200)
        N = np.random.randint(1,200)
        Xx = [[np.random.randint(1,10) for j in range(N)] for i in range(M)]
        # we must center the data (we scale it also but this is not necessary, I guess)
        # in order to have the same input to compare
        X = []
        for i in range( 0, len(Xx) ):
            row = Xx[i]
            sum_ = sum(elt for elt in row)
            vector = [elt/float(sum_) for elt in row]
            X.append( vector )
        for i in range( 0, len( X[0] ) ):
            sum_ = sum(X[j][i] for j in range(len(X)))
            average = sum_/float(len(X))
            for j in range(len(X)):
                X[j][i] -= average
        test_pca_svd(X,self)

# class Test_saveTimeSeries(unittest.TestCase):

#     def test_sum(self):
#         self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

#     def test_sum_tuple(self):
#         self.assertEqual(sum((1, 2, 3)), 6, "Should be 6")


if __name__ == '__main__':
    unittest.main()
