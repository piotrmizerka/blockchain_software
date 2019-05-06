#include "stdafx.h"

void TransactionHypergraphTest::clusterTest(bool smallTests, bool bigTests)
{
	cout << "cluster test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "cluster\\small" + dec2String(i) + "_in.txt";
			loadTestIn(relativePath);
			clusterVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "cluster\\big_in.txt";
		loadTestIn(relativePath);
		clusterVerify(false);
	}
}

void TransactionHypergraphTest::createUsersGraphTest(bool smallTests, bool bigTests)
{
	cout << "createUsersGraph test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "createUsersGraph\\small" + dec2String(i) + "_in.txt";
			loadTestIn(relativePath);
			createUsersGraphVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "createUsersGraph\\big_in.txt";
		loadTestIn(relativePath);
		createUsersGraphVerify(false);
	}
}

void TransactionHypergraphTest::createUsersGraphGenerateBigTest(int addressesNumber, int transactionsNumber)
{
	cout << "createUsersGraph big test generating...\n";
	string relativePath = "createUsersGraph\\big_in.txt";
	generateBigTestIn(relativePath, addressesNumber, transactionsNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	testTransactionHypergraph.createUsersGraph();
	Graph usersGraph = testTransactionHypergraph.getUsersGraph();
	usersGraph.computeVerticesDegrees();
	vector<int> verticesDegrees = usersGraph.getVerticesDegrees();
	sort(verticesDegrees.begin(), verticesDegrees.end());
	double weightSum = usersGraph.edgesWeightSum();
	pathxx = pathx + "createUsersGraph\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	fprintf(saveOut, "%d\n", verticesDegrees.size());
	for (int i = 0;i < verticesDegrees.size();i++)fprintf(saveOut, "%d\n", verticesDegrees[i]);
	fprintf(saveOut, "%lf", weightSum);
	fclose(saveOut);
}

void TransactionHypergraphTest::testAll(bool smallTests, bool bigTests)
{
	clusterTest(smallTests, bigTests);
	createUsersGraphTest(smallTests, bigTests);
}

void TransactionHypergraphTest::generateBigTestAll(int addressesNumber, int transactionsNumber)
{
	clusterGenerateBigTest(addressesNumber, transactionsNumber);
	createUsersGraphGenerateBigTest(addressesNumber, transactionsNumber);
}

void TransactionHypergraphTest::clusterGenerateBigTest(int addressesNumber, int transactionsNumber)
{
	cout << "cluster big test generating...\n";
	string relativePath = "cluster\\big_in.txt";
	generateBigTestIn(relativePath, addressesNumber, transactionsNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	testTransactionHypergraph.cluster();
	vector<set<int> > clusters;
	clusters = testTransactionHypergraph.getClusters();
	pathxx = pathx + "cluster\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	FOREACH(cluster, clusters)
	{
		fprintf(saveOut, "%d ", cluster->size());
		FOREACH(address,*cluster)fprintf(saveOut, "%d ", *address);
		fprintf(saveOut, "\n");
	}
	fclose(saveOut);
}

void TransactionHypergraphTest::generateBigTestIn(string relativePath, int addressesNumber, int transactionsNumber)
{
	FILE *saveIn;
	string pathxx;
	vector <int> addresses;
	set<int> addressesx;
	int transId;
	vector<int> out;
	set<int> inx, outx;
	int inSize, outSize;
	int temp, sizex;
	double amount;
	srand(time(NULL));
	pathxx = pathx + relativePath;
	saveIn = fopen(pathxx.c_str(), "w");
	for (int i = 0;i < addressesNumber;i++)
	{
		temp = rand() % (3 * addressesNumber);
		while (addressesx.find(temp) != addressesx.end())temp = rand() % (3 * addressesNumber);
		addressesx.insert(temp);
	}
	FOREACH(address, addressesx)addresses.push_back(*address);
	for(int i=0;i<transactionsNumber;i++)
	{
		transId = i;
		fprintf(saveIn, "%i\n", transId);
		inSize = rand() % 10 + 1;
		fprintf(saveIn, "%i ", inSize);
		inx.clear();
		for (int i = 0;i < inSize;i++)
		{
			temp = addresses[rand() % addresses.size()];
			while(inx.find(temp)!=inx.end())temp = addresses[rand() % addresses.size()];
			inx.insert(temp);
			fprintf(saveIn, "%i ", temp);
		}
		fprintf(saveIn, "\n");
		out.clear();
		outx.clear();
		outSize = rand() % 10 + 1;
		fprintf(saveIn, "%i ", outSize);
		for (int i = 0;i < outSize;i++)
		{
			temp = addresses[rand() % addresses.size()];
			while (inx.find(temp) != inx.end())temp = addresses[rand() % addresses.size()];
			outx.insert(temp);
			fprintf(saveIn, "%i ", temp);
			out.push_back(temp);
		}
		fprintf(saveIn, "\n");
		for (int i = 0;i < outSize;i++)
		{
			sizex = rand() % 3;
			fprintf(saveIn, "%i %i ", out[i], sizex);
			for (int j = 0;j < sizex;j++)
			{
				amount = double(rand() % 10000);
				fprintf(saveIn, "%.lf ", amount);
			}
			fprintf(saveIn, "\n");
		}
	}
	fclose(saveIn);
}

void TransactionHypergraphTest::loadTestIn(string relativePath)
{
	FILE *readIn;
	string pathxx;
	vector <int> addresses;
	set<int> addressesx;
	vector <Transaction> transactions;
	Transaction transaction;
	int transId;
	vector<int> in;
	vector<int> out;
	map<int, vector<double> > amountsReceived;
	int inSize, outSize;
	int temp, sizex;
	double amount;
	pathxx = pathx + relativePath;
	readIn = fopen(pathxx.c_str(), "r");
	while (!feof(readIn))
	{
		fscanf(readIn, "%i", &transId);
		in.clear();
		fscanf(readIn, "%i", &inSize);
		for (int i = 0;i < inSize;i++)
		{
			fscanf(readIn, "%i", &temp);
			in.push_back(temp);
			addressesx.insert(temp);
		}
		out.clear();
		fscanf(readIn, "%i", &outSize);
		for (int i = 0;i < outSize;i++)
		{
			fscanf(readIn, "%i", &temp);
			out.push_back(temp);
			addressesx.insert(temp);
		}
		amountsReceived.clear();
		FOREACH(addr, out)amountsReceived[*addr].clear();
		for (int i = 0;i < outSize;i++)
		{
			fscanf(readIn, "%i %i", &temp, &sizex);
			for (int j = 0;j < sizex;j++)
			{
				fscanf(readIn, "%lf", &amount);
				amountsReceived[temp].push_back(amount);
			}
		}
		transaction.id = transId;
		transaction.in = in;
		transaction.out = out;
		transaction.amountsReceived = amountsReceived;
		transactions.push_back(transaction);
	}
	fclose(readIn);
	FOREACH(address, addressesx)addresses.push_back(*address);
	testTransactionHypergraph = TransactionHypergraph(addresses, transactions);
}

void TransactionHypergraphTest::createUsersGraphVerify(bool smallTests, int smallId)
{
	vector <int> addresses;
	set<int> addressesx;
	Graph usersGraph;
	set<int> cluster;
	vector<int> verticesDegrees, verticesDegrees2;
	double weightSum, weightSum2;
	string pathxx;
	FILE *readOut;
	if (smallTests == true)pathxx = pathx + "createUsersGraph\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "createUsersGraph\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testTransactionHypergraph.createUsersGraph();
	usersGraph = testTransactionHypergraph.getUsersGraph();
	usersGraph.computeVerticesDegrees();
	verticesDegrees = usersGraph.getVerticesDegrees();
	weightSum = usersGraph.edgesWeightSum();
	sort(verticesDegrees.begin(), verticesDegrees.end());
	int n, degv;
	fscanf(readOut, "%i", &n);
	for(int i=0;i<n;i++)
	{
		fscanf(readOut, "%i", &degv);
		verticesDegrees2.push_back(degv);
	}
	fscanf(readOut, "%lf", &weightSum2);
	fclose(readOut);
	sort(verticesDegrees2.begin(), verticesDegrees2.end());
	bool checkDegrees = true, checkSum = true;
	if (n != verticesDegrees.size())checkDegrees = false;
	else for (int i = 0;i < n;i++)if (verticesDegrees[i] != verticesDegrees2[i])checkDegrees = false;
	if (abs(weightSum - weightSum2) > 0.001)checkSum = false;
	if (smallTests == true)
	{
		cout << "small test " << smallId << ": ";
		if (checkDegrees == true && checkSum == true)cout << "OK" << endl;
		else
		{
			cout << "FAILED\n";
			if (checkDegrees == false)
			{
				cout << "Proper vertices degrees: ";
				for (int i = 0;i < verticesDegrees2.size();i++)cout << verticesDegrees2[i] << " ";
				cout << "\nObtained vertices degrees: ";
				for (int i = 0;i < verticesDegrees.size();i++)cout << verticesDegrees[i] << " ";
				cout << endl;
			}
			if (checkSum == false)
			{
				cout << "Proper weight sum: " << weightSum2 << endl;
				cout << "Obtained weight sum: " << weightSum << endl;
			}
		}
	}
	else
	{
		cout << "big test: ";
		if (checkDegrees == true && checkSum == true)cout << "OK" << endl;
		else cout << "FAILED\n";
	}
}

void TransactionHypergraphTest::clusterVerify(bool smallTests, int smallId)
{
	vector <int> addresses;
	set<int> addressesx;
	vector <Edge> edges;
	Edge edge;
	vector < set <int> > clusters, clusters2;
	set<int> cluster;
	vector <int> clusterIds, clusterIds2;
	string pathxx;
	FILE *readOut;
	if (smallTests == true)pathxx = pathx + "cluster\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "cluster\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testTransactionHypergraph.cluster();
	clusters = testTransactionHypergraph.getClusters();
	clusters2.clear();
	int n, v;
	while (!feof(readOut))
	{
		cluster.clear();
		fscanf(readOut, "%i", &n);
		for (int i = 0;i < n;i++)
		{
			fscanf(readOut, "%i", &v);
			cluster.insert(v);
		}
		clusters2.push_back(cluster);
	}
	fclose(readOut);
	addresses = testTransactionHypergraph.getAddresses();
	clusterIds.resize(10 * addresses.size());
	clusterIds2.resize(10 * addresses.size());
	for (int i = 0;i < clusters.size();i++)FOREACH(v, clusters[i])clusterIds[*v] = i;
	for (int i = 0;i < clusters2.size();i++)FOREACH(v, clusters2[i])clusterIds2[*v] = i;
	bool check = true;
	for (int i = 0;i < addresses.size();i++)
	{
		if (clusters[clusterIds[addresses[i]]].size() != clusters2[clusterIds2[addresses[i]]].size())
		{
			check = false;
		}
		else
		{
			FOREACH(v, clusters[clusterIds[addresses[i]]])
			{
				if (clusters2[clusterIds2[addresses[i]]].find(*v) == clusters2[clusterIds2[addresses[i]]].end())
				{
					check = false;
				}
			}
		}
	}
	if (smallTests == true)
	{
		cout << "small test " << smallId << ": ";
		if (check == true)cout << "OK" << endl;
		else
		{
			cout << "FAILED\n\Proper clusters:\n";
			FOREACH(cluster, clusters2)
			{
				FOREACH(v, *cluster)cout << *v << " ";
				cout << endl;
			}
			cout << "Obtained clusters:\n";
			FOREACH(cluster, clusters)
			{
				FOREACH(v, *cluster)cout << *v << " ";
				cout << endl;
			}
		}
	}
	else
	{
		cout << "big test: ";
		if (check == true)cout << "OK" << endl;
		else cout << "FAILED\n";
	}
}
