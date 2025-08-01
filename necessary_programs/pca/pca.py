#   The time series are defined as follows. Let X be the matrix containing the snapshots' data
# (for either trasnsaction number or value). X is a T x L matrix, where T is the number of time
# periods taken into consideration and L is the number of edges in the underlying graph for 
# the long-term subgraph (for the time period to be considered only: e.g. if take the period
# 2015.01.01-2022.06.23, then the edges from the long-term subgraph having transactions only
# in the period 2009.01.19-2014.12.31 won't appear in the underlying graph).
#   We then subsitute X with the matrix conaining the old entries of X divided by the sum 
# (in the case normalizationMode is "l1") or the sum of squares (in the case normalizationMode is "l2")
# of the row's entries of the row they belong to (if such a sum is zero (for either option 1 or 2), then we do nothing). 
# Next, we substitute the so obtained X, with the matrix containing the entries of such X diminished 
# by the column averages (by computing the average, we ingore the zeros coming from zero rows). 
# This ensures that the data in the new X will be centered at the origin - the sums of each row and column will be zero.
# Then, we perform PCA with Singular Value Decomposition (SVD), that is we decompose X into
# X = U x S x V^T, where U is a T x T matrix, S a T x T diagonal matrix with singular
# values at the diagonal, and V a T x L matrix (we can reduce the dimensions of U, S, and V in
# such a way since T << L).
#   Finally, we compute the time series by the following formula:
# ith time series (corresponding to the ith principal component) at time t equals the sum:
# V[1][i]*originalX[t][1] + ... + V[L][i]*originalX[t][L]. Note that now, we turn back again
# to the original snapshots, not their translation to the origin taken for SVD. This is the
# convention we used but the modifications (e.g. take X instead of originalX) are possible as well.

from sklearn.decomposition import PCA
import os
import numpy as np
import sys
import math

def dataMatrix(snapShotsFolder, normalizationMode):
    Xx = []
    for fileName in sorted(os.listdir(snapShotsFolder)):
        read = open( snapShotsFolder+"/"+fileName, "r" )
        vector = []
        for line in read:
            linex = line.split()
            vector.append( float(linex[2]) )
        read.close()
        Xx.append( vector )

    X = []
    zeroRowsNumber = 0
    zeroRows = np.zeros(len(Xx), int)
    for i in range( 0, len(Xx) ):
        sum = 0
        row = Xx[i]
        for elt in row:
            sum += elt
            sum = (sum+elt if normalizationMode == "l1" else sum+elt**2) 
        vector = []
        norm = (float(sum) if normalizationMode == "l1" else math.sqrt(float(sum)))
        if sum != 0:
            for elt in row:
                vector.append( elt/norm ) 
        else:
            for elt in row:
                vector.append( 0 )
            zeroRowsNumber += 1
            zeroRows[i] = 1
        X.append( vector )

    for i in range( 0, len( X[0] ) ):
        sum = 0
        for j in range( 0, len( X ) ):
            sum += X[j][i]
        average = sum/float( len( X )-zeroRowsNumber )
        for j in range( 0, len( X ) ):
            if zeroRows[j] == 0:
                X[j][i] -= average

    return Xx, X

# An equivalent way to perform PCA - we do not apply this, however - we apply different SVD - see below.
# Caution: this method automatically centers (still, not scales) the data! 
# - see https://scikit-learn.org/stable/modules/generated/sklearn.decomposition.PCA.html 
def pca_sklearn(X,n_components = 4):
    pca = PCA(n_components = n_components)
    pca.fit(X)
    return pca.explained_variance_ratio_, pca.singular_values_, pca.components_

# we rather use thin svd, as explained here:
# https://en.wikipedia.org/wiki/Singular_value_decomposition 
def pca_svd(X):
    U, S, VT = np.linalg.svd(X, full_matrices=False)
    return U, S, VT.transpose()

def saveTimeSeries(X, V, savePath, componentsNumber):
    L = len(V)
    for i in range(0,componentsNumber):
        save = open( savePath+"/component_"+str(i+1)+".dat", "w" )
        for t in range(0,len(X)):
            sum = 0
            for l in range(0,L):
                sum += V[l][i]*X[t][l]
            save.write(str(sum)+"\n")
        save.close()

# if __name__ == "main":
snapshotsPath = sys.argv[1] 
timeSeriesPath = sys.argv[2]
componentsNumber = int(sys.argv[3])
normMode = sys.argv[4]
originalSnapshots, modifiedSnapshots = dataMatrix(snapShotsFolder = snapshotsPath, normalizationMode = normMode)
U, S, V = pca_svd(modifiedSnapshots)
saveTimeSeries(originalSnapshots, V, timeSeriesPath, componentsNumber)