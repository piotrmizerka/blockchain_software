import numpy as np
import random
import calendar
import datetime
import timeit
import math
from sklearn.decomposition import PCA
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import datetime as dt
import matplotlib.dates as mdates
from scipy.optimize import curve_fit

class PCAFileParser:
    
    readPathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\"
    outPathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_weekly_from_2013\\"

    def runPCA( self, unixTimeUpperLimit = 1518134400, intervalInDays = 7 ):
        begTime = 1367193600 # stands for 2013-04-29
        snapshotsNumber = (unixTimeUpperLimit - begTime) / (intervalInDays * 86400)
        Xx = []
        for i in range(0,int(snapshotsNumber)-1):
            unixTime = begTime + i * intervalInDays * 86400
            a = datetime.fromtimestamp(unixTime)
            b = str( a )
            c = b.split()
            read = open( self.readPathx+"snapshots_trans_value_normalized\\"+c[0]+".txt", "r" )
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

        for i in range( 0, len( X[0] ) ):
	        sum = 0
	        for j in range( 0, len( X ) ):
		        sum += X[j][i]
	        average = sum/float( len( X ) )
	        for j in range( 0, len( X ) ):
		        X[j][i] -= average

        start = timeit.default_timer()
        pca = PCA(n_components=100)
        pca.fit(X)
        pcaExplainedVarianceRatio = pca.explained_variance_ratio_
        save = open( self.outPathx+"explained_vairance_ratio.txt", "w" )
        for ratio in pcaExplainedVarianceRatio:
            save.write( str( ratio )+"\n" )
        save.close()
        pcaSingularValues = pca.singular_values_
        save = open( self.outPathx+"singular_values.txt", "w" )
        for value in pcaSingularValues:
            save.write( str( value )+"\n" )
        save.close()
        pcaComponents = pca.components_
        i = 0
        for component in pcaComponents:
            save = open( self.outPathx+"principal_components\\component_"+str(i)+".txt", "w" )
            for x in component:
               save.write( str(x)+"\n" )
            save.close()
            i += 1

        u, s, vh = np.linalg.svd(X, full_matrices=False)
        for i in range( 0, len(s) ):
            save = open( self.outPathx+"component_contributons_over_time\\component_netowork_"+str( i )+".txt", "w" )
            it = 0
            for ut in u:
                unixTime = begTime + it *intervalInDays* 86400
                a = datetime.fromtimestamp(unixTime)
                b = str( a )
                c = b.split()
                save.write( c[0]+" "+str( ut[i] )+"\n" )
                it += 1
            save.close()

        stop = timeit.default_timer()
        print(stop-start)

    def saveComponentCoefficients( self, componentsNumber = 100 ):
        coeffs = []
        readPrices = open( self.outPathx+"prices_2_cons_no_dates.txt","r" )
        prices = []
        for line in readPrices:
            linex = line.split()
            prices.append( float( linex[0] ) )
        readPrices.close()
        for i in range(0,componentsNumber):
            coeff = 0
            read = open( self.outPathx+"component_contributons_over_time\\component_netowork_"+str( i )+".txt", "r" )
            it = 0
            for line in read:
                linex = line.split()
                coeff += ( float( linex[1] )*prices[it] )
                it +=1
            read.close()
            coeffs.append( coeff )
        save = open( self.outPathx+"components_coefficients.txt", "w" )
        for i in range( 0, len( coeffs ) ):
            save.write( str( coeffs[i] )+"\n" )
        save.close()

    def saveComponentCombContributions( self, snapshotsNumber = 3307, componentsNumber = 100 ):
        readCoefficients = open( self.outPathx+"components_coefficients.txt", "r" )
        coeffs = []
        for line in readCoefficients:
            linex = line.split()
            coeffs.append( float( linex[0] ) )
        readCoefficients.close()
        save = []
        for i in range( 0, componentsNumber ):
            temp = open( self.outPathx+"comp_comb_contributions\\combination_"+str( i+1 )+"_components.txt","w")
            save.append( temp )
        contributions = []
        for i in range( 0, snapshotsNumber ):
            contributions.append( 0 )
        for i in range(0,componentsNumber):
            read = open( self.outPathx+"component_contributons_over_time\\component_netowork_"+str( i )+".txt","r")
            for k in range( 0,snapshotsNumber ):
                line = read.readline()
                linex = line.split()
                contributions[k] += ( float( linex[1] )*coeffs[i] )
            read.close()
            for j in range( 0,snapshotsNumber ):
                save[i].write( str( contributions[j] )+"\n" )
        for i in range( 0, componentsNumber ):
            save[i].close()

    def saveResidualStandardErrors( self, componentNumber = 100, snapshotNumber = 3307 ):
        save = open( self.outPathx+"residual_standard_errors.txt", "w" )
        for i in range( 1, componentNumber ):
            readContr = open(self.outPathx+"comp_comb_contributions\\combination_"+str( i )+"_components.txt","r")
            readBitcoinPrices = open( self.outPathx+"prices_2_cons_no_dates.txt", "r" )
            error = 0
            for it in range(0,snapshotNumber):
                line1 = readContr.readline()
                line2 = readBitcoinPrices.readline()
                line1x = line1.split()
                line2x = line2.split()
                error += (float( line1x[0] )-float( line2x[0] ))*(float( line1x[0] )-float( line2x[0] ))
            readBitcoinPrices.close()
            readContr.close()
            error /= snapshotNumber
            error = math.sqrt( error )
            save.write( str( i )+" "+str( error )+"\n" )
        save.close()

pcax = PCAFileParser()
pcax.runPCA()
#pcax.saveComponentCoefficients()
#pcax.saveComponentCombContributions(2800)
#pcax.saveResidualStandardErrors(100,2800)

"""read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_values\\comp_comb_contributions\\combination_3_components.txt", "r" )
readBitcoinPrices = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\prices_2_cons_no_dates.txt", "r" )
prices = []
for line in readBitcoinPrices:
	linex = line.split()
	prices.append( float( linex[0] ) )
vector = []
it = 0
x = []
y = []
for line in read:
	linex = line.split()
	temp = []
	temp.append( float( it ) )
	temp.append( float( linex[0] ) )
	vector.append( temp )
	#vector.append( float( linex[1] ) )
	#plt.plot( temp[0], temp[1] )
	x.append( temp[0] )
	y.append( temp[1] )
	it += 1

read.close()
readBitcoinPrices.close()
read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\dates_2_consider.txt", "r" )
dates = []
it = 0
for line in read:
    linex = line.split()
    dates.append( linex[0] )
x = [dt.datetime.strptime(d,'%Y/%m/%d').date() for d in dates]
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
#plt.gca().xaxis.set_major_locator(mdates.MonthLocator())
plt.gcf().autofmt_xdate()
read.close()
#plt.grid(b=True, which='major')
numberOfDates2Conside = 450
fig, componentPlot = plt.subplots()
componentPlot.scatter(x[-numberOfDates2Conside:],y[-numberOfDates2Conside:],color='red',linewidths=1)
l1, = componentPlot.plot(x[-numberOfDates2Conside:],y[-numberOfDates2Conside:],color='red',label="N=400")
#componentPlot.legend(loc="upper left")
pricesPlot = componentPlot.twinx()
pricesPlot.scatter(x[-numberOfDates2Conside:],prices[-numberOfDates2Conside:],linewidths=0.1)
l2, = pricesPlot.plot(x[-numberOfDates2Conside:],prices[-numberOfDates2Conside:],label="bicoin price")
#pricesPlot.legend(loc="center left")
componentPlot.set_ylim((-1500,20300))
pricesPlot.yaxis.set_visible(False)
plt.legend([l1,l2],["N=390", "bicoin price (USD)"])
plt.show()"""

"""for componentsNumber in range(1,451):
    readCoeffs = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\components_coefficients.txt", "r" )
    coeffs = []
    for line in readCoeffs:
	    linex = line.split()
	    coeffs.append( float(linex[0]) )
    readCoeffs.close()
    save = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\comp_comb_contributions\\combination_"+str(componentsNumber)+"_components.txt","w")
    read = []
    for i in range(0,componentsNumber):
	    temp = open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\comp_contr_time_no_dates\\component_"+str(i)+".txt","r")
	    read.append( temp )
    for i in range(0,473):
	    sum = 0
	    for it in range(0,componentsNumber):
		    line = read[it].readline()
		    linex = line.split()
		    sum += coeffs[it]*float(linex[0])
	    save.write( str(sum)+"\n" )
    for i in range(0,componentsNumber):
	    read[i].close()
    save.close()"""

"""read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\explained_vairance_ratio.txt", "r" )
vector = []
it = 1
x = []
y = []
for line in read:
	linex = line.split()
	temp = []
	temp.append( float( it ) )
	temp.append( 100*float( linex[0] ) )
	vector.append( temp )
	x.append( temp[0] )
	y.append( temp[1] )
	it += 1

fig = plt.figure(figsize=(10, 6))
ax = fig.add_subplot(111)
ax.semilogx()
ax.semilogy()
ax.plot(x,y,color="red",linewidth=4)
ax.xaxis.set_major_formatter(ScalarFormatter())
ax.yaxis.set_major_formatter(ScalarFormatter())
plt.xlabel("Component id")
plt.ylabel("Percentage of total variance")
plt.show()"""

"""read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\comp_contr_time_no_dates\\component_0.txt", "r" )
readBitcoinPrices = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\prices_2_cons_no_dates.txt", "r" )
prices = []
for line in readBitcoinPrices:
	linex = line.split()
	prices.append( float( linex[0] ) )
vector = []
it = 0
x = []
y = []
for line in read:
	linex = line.split()
	temp = []
	temp.append( float( it ) )
	temp.append( float( linex[0] ) )
	vector.append( temp )
	#vector.append( float( linex[1] ) )
	#plt.plot( temp[0], temp[1] )
	x.append( temp[0] )
	y.append( temp[1] )
	it += 1

read.close()
readBitcoinPrices.close()
#plt.grid(b=True, which='major')

read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\dates_2_consider.txt", "r" )
dates = []
it = 0
for line in read:
    linex = line.split()
    dates.append( linex[0] )
x = [dt.datetime.strptime(d,'%Y/%m/%d').date() for d in dates]
plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
#plt.gca().xaxis.set_major_locator(mdates.MonthLocator())
plt.gcf().autofmt_xdate()
read.close()
fig, componentPlot = plt.subplots()
componentPlot.scatter(x,y,color='red',linewidths=0.01)
l2, = componentPlot.plot(x,y,color='red')
pricesPlot = componentPlot.twinx()
pricesPlot.scatter(x,prices,linewidths=0.001)
l1, = pricesPlot.plot(x,prices)
#plt.ylabel("bitoin price (USD)")
pricesPlot.set_ylabel("Bitoin price (USD)")
componentPlot.set_ylabel("First component contribution")
plt.legend([l1,l2],["bicoin price","first component contribution"])
plt.show()"""


class PlotDrawer:
    pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_values\\"

    def plotResidualStandardErrors( self ):
        read = open( self.pathx+"residual_standard_errors.txt", "r" )
        vector = []
        it = 1
        x = []
        y = []
        for line in read:
	        linex = line.split()
	        temp = []
	        temp.append( float( it ) )
	        temp.append( float( linex[1] ) )
	        vector.append( temp )
	        #vector.append( float( linex[1] ) )
	        #plt.plot( temp[0], temp[1] )
	        x.append( temp[0] )
	        y.append( temp[1] )
	        it += 1

        read.close()

        fig, errorPlot = plt.subplots()
        errorPlot.scatter(x,y,color='red',linewidths=1)
        errorPlot.plot(x,y,color='red')
        errorPlot.set_ylabel("Residual standard error")
        errorPlot.set_xlabel("Number of components taken into account (N)")
        errorPlot.set_xlim((1,500))
        ticksx = []
        ticksx.append( 1 )
        for i in range(2,451):
            if i%50 == 0:
                ticksx.append( i )
        plt.xticks(ticksx)
        plt.show()

    def plotClusterSizeFrequency( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\clusters_sizes_frequencies.txt", "r" )
        x = []
        y = []
        for line in read:
	        linex = line.split()
	        x.append( float( linex[1] ) )
	        y.append( float( linex[2] ) )
        read.close()
        fig = plt.figure(figsize=(10, 6))
        ax = fig.add_subplot(111)
        ax.semilogx()
        ax.semilogy()
        ax.scatter(x,y,color="red",linewidth=2)
        ax.xaxis.set_major_formatter(ScalarFormatter())
        ax.yaxis.set_major_formatter(ScalarFormatter())
        plt.xlabel("Natural logarithm of the number of IP addresses at the end of intervals")
        plt.ylabel("Natural logarithm of cluster size frequency")
        plt.show()

    def plotEdgesNumber( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_edges_over_time.txt", "r" )
        dates = []
        y = []
        timeInMonths = []
        for line in read:
            linex = line.split()
            year = linex[0][0:4]
            month = linex[0][5:7]
            day = linex[0][8:10]
            d= dt.datetime(int(year), int(month), int(day), 0, 0)
            unixTime = calendar.timegm( d.utctimetuple() )
            timeInMonths.append( int( ( float( unixTime )-1232390000 )/2628000 ) )
        read.close()
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_edges_over_time.txt", "r" )
        for line in read:
	        linex = line.split()
	        dates.append( linex[0] )
	        y.append( int( linex[2] ) )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        #plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        #plt.gcf().autofmt_xdate()
        #plt.plot( x, y,linewidth=3,color="red" )
        plt.plot( timeInMonths, y,linewidth=3,color="red" )
        plt.ylabel("Number of edges")
        sol1, abc = curve_fit(self.func_powerlaw, timeInMonths, y, maxfev=2000000000 )
        print( sol1 )
        plt.show()
    
    def func_powerlaw( self, x, m ):
        return x**m

    def plotVerticesNumberAndBitcoinTurnover( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\total_trans_amount_over_time.txt", "r" )
        x = []
        y = []
        dates = []
        for line in read:
	        linex = line.split()
	        y.append( float( linex[1] )/100000000 )
        read.close()
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\total_trans_amount_over_time.txt", "r" )
        for line in read:
            linex = line.split()
            dates.append( linex[0] )
        read.close()
        nodesNumbers = []
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_edges_over_time.txt", "r" )
        for line in read:
            linex = line.split()
            nodesNumbers.append( int( linex[1] ) )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        plt.gcf().autofmt_xdate()
        fig, verticesPlot = plt.subplots()
        l1, = verticesPlot.plot(x,nodesNumbers,color='red',label="N=400",linewidth=3)
        turnoverPlot = verticesPlot.twinx()
        l2, = turnoverPlot.plot(x,y,label="bicoin price",linewidth=3)
        plt.legend([l1,l2],["users number", "turnover"])
        verticesPlot.set_ylabel("Number of users")
        turnoverPlot.set_ylabel("Amount (in bitcoins)")
        plt.show()

    def plotBalances( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\average_vertices_balances_over_time2.txt", "r" )
        dates = []
        positiveAverages = []
        negativeAverages = []
        for line in read:
            linex = line.split()
            dates.append( linex[0] )
            positiveAverages.append( float ( linex[2] )/100000000 )
            negativeAverages.append( float( linex[1] )/100000000 )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        plt.gcf().autofmt_xdate()
        fig, positive = plt.subplots()
        l1, = positive.plot(x,positiveAverages,color='red',linewidth=3)
        negative = positive.twinx()
        l2, = negative.plot(x,negativeAverages,linewidth=3)
        negative.yaxis.set_visible(False)
        negative.set_ylim((-800,800))
        positive.set_ylim((-800,800))
        plt.legend([l1,l2],["positve", "negative"])
        positive.set_ylabel("Balance in BTC")
        plt.show()

    def plotGiniBalances( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\balances_gini_over_time_positive.txt", "r" )
        dates = []
        positiveGini = []
        for line in read:
            linex = line.split()
            dates.append( linex[0] )
            positiveGini.append( float ( linex[1] ) )
        read.close()
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\balances_gini_over_time_negative.txt", "r" )
        negativeGini = []
        for line in read:
            linex = line.split()
            negativeGini.append( float ( linex[1] ) )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        plt.gcf().autofmt_xdate()
        fig, positive = plt.subplots()
        l1, = positive.plot(x,positiveGini,color='red',linewidth=3)
        negative = positive.twinx()
        l2, = negative.plot(x,negativeGini,linewidth=3)
        negative.yaxis.set_visible(False)
        negative.set_ylim((0.9,1))
        positive.set_ylim((0.9,1))
        plt.legend([l1,l2],["positve", "negative"],loc="upper left")
        positive.set_ylabel("Gini coefficient")
        plt.show()

    def plotClusteringCoefficient( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\avg_clust_coeffs_over_time2.txt", "r" )
        dates = []
        y = []
        for line in read:
	        linex = line.split()
	        dates.append( linex[0] )
	        y.append( float( linex[1] ) )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        plt.gcf().autofmt_xdate()
        plt.plot( x, y,linewidth=3,color="red" )
        plt.ylabel("Clutering coefficient")
        plt.show()

    def plotBuyersSellersBitcoinPrice( self ):
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\byrs_slrs_no_over_time2.txt", "r" )
        buyers = []
        dates = []
        sellers = []
        for line in read:
	        linex = line.split()
	        buyers.append( int( linex[1] ))
        read.close()
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\byrs_slrs_no_over_time2.txt", "r" )
        for line in read:
            linex = line.split()
            sellers.append( int(linex[2] ))
            dates.append( linex[0] )
        read.close()
        bitcoinPrice = []
        dates2 = []
        read = open( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca\\prices_2_consider.txt", "r" )
        for line in read:
            linex = line.split()
            bitcoinPrice.append( int( linex[1] ) )
            dates2.append( linex[0] )
        read.close()
        x = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates]
        x2 = [dt.datetime.strptime(d,'%Y-%m-%d').date() for d in dates2]
        plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y/%m/%d'))
        plt.gcf().autofmt_xdate()
        fig, buyersPlot = plt.subplots()
        number12Consider = 80
        number22Consider = 110
        l1, = buyersPlot.plot(x[-number12Consider:], buyers[-number12Consider:],color='red',label="N=400",linewidth=3)
        pricePlot = buyersPlot.twinx()
        sellersPlot = buyersPlot.twinx()
        l2, = pricePlot.plot(x2[-number22Consider:],bitcoinPrice[-number22Consider:],color='blue',label="N=400",linewidth=3)
        l3, = sellersPlot.plot(x[-number12Consider:],sellers[-number12Consider:],label="bicoin price",color="green",linewidth=3)
        plt.legend([l1,l3,l2],["buyers","sellers","bitcoin price"],loc="upper left")
        buyersPlot.set_ylabel("Number of users")
        pricePlot.set_ylabel("Bitcoin price (USD)")
        sellersPlot.yaxis.set_visible(False)
        buyersPlot.set_ylim((100000,900000))
        sellersPlot.set_ylim((100000,900000))
        plt.show()

plotDrawer = PlotDrawer()
#plotDrawer.plotClusterSizeFrequency()
#plotDrawer.plotEdgesNumber()
#plotDrawer.plotVerticesNumberAndBitcoinTurnover()
#plotDrawer.plotBalances()
#plotDrawer.plotGiniBalances()
#plotDrawer.plotClusteringCoefficient()
#plotDrawer.plotBuyersSellersBitcoinPrice()
#plotDrawer.plotResidualStandardErrors()