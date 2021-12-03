from sklearn.decomposition import PCA
import os
import math
import numpy as np
import sys

def dataMatrix(snapShotsFolder):
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
    for row in Xx:
        norm = 0
        for elt in row:
            norm += (elt*elt)
        vector = []
        if norm != 0:
            for elt in row:
                vector.append( elt/math.sqrt( norm ) )
        else:
            for elt in row:
                vector.append( 0 )
        X.append( vector )
    
    return X

def pca_sklearn(X, snapShotsFolder): # TODO: determine why this gives different singular values than svd way
    for i in range( 0, len( X[0] ) ):
        sum = 0
        for j in range( 0, len( X ) ):
            sum += X[j][i]
        average = sum/float( len( X ) )
        for j in range( 0, len( X ) ):
            X[j][i] -= average

    pca = PCA(n_components=4)
    pca.fit(X)
    return pca.explained_variance_ratio_, pca.singular_values_, pca.components_

def pca_svd(X, snapShotsFolder):
    U, S, V = np.linalg.svd(X, full_matrices=True)

    return V

def saveTimeSeries(X, V, savePath, componentsNumber):
    L = len(V[0])
    for i in range(0,componentsNumber):
        save = open( savePath+"/component_"+str(i+1)+".dat", "w" )
        for t in range(0,len(X)):
            sum = 0
            for l in range(0,L):
                sum += V[l][i]*X[t][l]
            save.write(str(sum)+"\n")
        save.close()

snapshotsPath = sys.argv[1] 
timeSeriesPath = sys.argv[2]
componentsNumber = int(sys.argv[3])
X = dataMatrix(snapShotsFolder = snapshotsPath)
V = pca_svd(X, snapShotsFolder = snapshotsPath)
saveTimeSeries(X, V, timeSeriesPath, componentsNumber)
