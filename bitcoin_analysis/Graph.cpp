#include "stdafx.h"


vector < set <int> > Graph::createUndirectedNeighborsSet()
{
	vector < set <int> > result;
	int maxVertex = 0;
	FOREACH(v, vertices)if (*v > maxVertex)maxVertex = *v;
	result.resize(maxVertex+1);
	FOREACH(e, edges)
	{
		result[e->u].insert(e->v);
		result[e->v].insert(e->u);
	}
	return result;
}

void Graph::createDirectedNeighborsList()
{
	for (int i = 0; i<vertices.size(); i++)neighborsList[vertices[i]].clear();
	for (int i = 0; i<edges.size(); i++)
	{
		if (neighborsList[edges[i].u].find(edges[i].v) == neighborsList[edges[i].u].end())
		{
			neighborsList[edges[i].u].insert(edges[i].v);
		}
	}
}

void Graph::computeVerticesDegrees()
{
	verticesDegrees.clear();
	int maxVal = maxValue(vertices);
	verticesDegrees.resize(maxVal + 1);
	for (int i = 0; i < maxVal; i++)verticesDegrees[i] = 0;
	FOREACH(e, edges)verticesDegrees[e->u]++;
}

double Graph::verticesNumber()
{
	double verticesNumber = 0;
	FOREACH(v, vertices)
	{
		if (*v > 0)
		{
			verticesNumber++;
		}
	}
	return verticesNumber;
}

double Graph::directedDegreeSum()
{
	double degreeSum = 0;
	createDirectedNeighborsList();
	FOREACH(v, vertices)
	{
		if (*v > 0)
		{
			degreeSum += neighborsList[*v].size();
		}
	}
	return degreeSum;
}

double Graph::averageDirectedDegree()
{
	double verticesNumber = 0, degreeSum = 0;
	createDirectedNeighborsList();
	FOREACH(v, vertices)
	{
		if (*v > 0)
		{
			verticesNumber++;
			degreeSum += neighborsList[*v].size();
		}
	}
	return degreeSum / verticesNumber;
}

Graph Graph::contractEdges(bool weightAsEdgesNumber)
{
	Graph result;
	vector <Edge> edgesx;
	map<pair<int, int>, double> totalEdgeValues;
	map<pair<int, int>, bool> consideredEdges;
	totalEdgeValues.clear();
	consideredEdges.clear();
	FOREACH(edge, edges)
	{
		totalEdgeValues[make_pair(edge->u, edge->v)] = 0;
		consideredEdges[make_pair(edge->u, edge->v)] = false;
	}
	if(weightAsEdgesNumber==true)FOREACH(edge, edges)totalEdgeValues[make_pair(edge->u, edge->v)]++;
	else FOREACH(edge, edges)totalEdgeValues[make_pair(edge->u, edge->v)] += edge->weight;
	edgesx.clear();
	Edge temp;
	FOREACH(edge, edges)
	{
		if (consideredEdges[make_pair(edge->u, edge->v)] == false)
		{
			temp = *edge;
			temp.weight = totalEdgeValues[make_pair(edge->u, edge->v)];
			edgesx.push_back(temp);
			consideredEdges[make_pair(edge->u, edge->v)] = true;
		}
	}
	result = Graph(vertices, edgesx);
	return result;
}

void Graph::saveUsersGraph(int begTime, int endTime)
{
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\separated\\users_graph_" + dec2String(begTime) + "_" + dec2String(endTime) + ".dat";
	FILE *save;
	save = fopen(pathx.c_str(), "w+");
	FOREACH(edge, edges)
	{
		if (edge->time >= begTime && edge->time <= endTime)
		{
			fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), edge -> time);
		}
	}
	fclose(save);
}

void Graph::separeteUsersGraph()
{
	string pathx;
	FILE *save[120];
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\separated\\users_graph_" + dec2String(i * 2500000) + "_" + dec2String((i + 1) * 2500000) + ".dat";
		save[i] = fopen(pathx.c_str(), "w+");
	}
	int fileId;
	FOREACH(edge, edges)
	{
		fileId = (edge->time) / 2500000;
		fprintf(save[fileId], "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), edge->time);
	}
	for (int i = 0;i < 120;i++)fclose(save[i]);
}

void Graph::contractEdgesSeparated()
{
	string pathx;
	Graph graph, graphContractedEdges;
	vector <Edge> edges;
	FILE *save[120];
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\separated_no_multiple_edges\\users_graph_" + dec2String(i * 2500000) + "_" + dec2String((i + 1) * 2500000) + ".dat";
		save[i] = fopen(pathx.c_str(), "w+");
	}
	for (int i = 0;i < 120;i++)
	{
		graph = loadUsersSubgraph(i);
		graphContractedEdges = graph.contractEdges();
		edges = graphContractedEdges.getEdges();
		FOREACH(edge, edges)fprintf(save[i], "%d %d %lld\n", edge->u, edge->v, long long(edge->weight));
		fclose(save[i]);
	}
}

Graph Graph::loadUsersGraph(string pathx, bool considerTimes)
{
	Graph result;
	vector <int> vertices;
	vector <Edge> edges;
	set <int> verticesNoRepetitions;
	long long value;
	vertices.clear();
	verticesNoRepetitions.clear();
	edges.clear();
	Edge edge;
	FILE *read;
	read = fopen(pathx.c_str(), "r");
	if (considerTimes == true)
	{
		while (!feof(read))
		{
			fscanf(read, "%i", &edge.u);
			fscanf(read, "%i", &edge.v);
			fscanf(read, "%lld", &value);
			fscanf(read, "%i", &edge.time);
			edge.weight = double(value);
			edges.push_back(edge);
			verticesNoRepetitions.insert(edge.u);
			verticesNoRepetitions.insert(edge.v);
		}
	}
	else
	{
		while (!feof(read))
		{
			fscanf(read, "%i", &edge.u);
			fscanf(read, "%i", &edge.v);
			fscanf(read, "%lld", &value);
			edge.weight = double(value);
			edges.push_back(edge);
			verticesNoRepetitions.insert(edge.u);
			verticesNoRepetitions.insert(edge.v);
		}
	}
	FOREACH(v, verticesNoRepetitions)vertices.push_back(*v);
	fclose(read);
	result = Graph(vertices, edges);
	return result;
}

/*void Graph::loadUsersGraphParallel(string pathx, int threadsNumber)
{
	vertices.clear();
	edges.clear();
	FILE **read;
	read = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx +dec2String(i);
		read[i] = fopen(pathxx.c_str(), "r");
	}
	consideredVertices.clear();
	consideredVertices.resize(usersNumber);
	vector< vector<Edge> > threadsEdges;
	threadsEdges.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			Edge edge;
			long long value;
			while (!feof(read[i]))
			{
				fscanf(read[i], "%d %d %lld %d", &edge.u, &edge.v, &value, &edge.time);
				edge.weight = double(value);
				threadsEdges[omp_get_thread_num()].push_back(edge);
				if (edge.u < usersNumber&&edge.u >= 0 && edge.v < usersNumber&&edge.v >= 0)
				{
					consideredVertices[edge.u] = true;
					consideredVertices[edge.v] = true;
				}
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)fclose(read[i]);
	vector< vector<int> > threadsVertices;
	threadsVertices.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < usersNumber;i++)
		{
			if (consideredVertices[i])
			{
				threadsVertices[omp_get_thread_num()].push_back(i);
			}
		}
	}
	int edgesUpToThread[40000], verticesUpToThread[40000];
	edgesUpToThread[0] = 0;
	verticesUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)edgesUpToThread[i] = edgesUpToThread[i - 1] + threadsEdges[i - 1].size();
	for (int i = 1;i < threadsNumber;i++)verticesUpToThread[i] = verticesUpToThread[i - 1] + threadsVertices[i - 1].size();
	int edgesNumber = edgesUpToThread[threadsNumber - 1] + threadsEdges[threadsNumber - 1].size();
	int verticesNumber = verticesUpToThread[threadsNumber - 1] + threadsVertices[threadsNumber - 1].size();
	edges.resize(edgesNumber);
	vertices.resize(verticesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for		
		for (int i = 0;i < threadsNumber;i++)
		{
			for (int j = 0;j < threadsEdges[i].size();j++)
			{
				edges[edgesUpToThread[i] + j] = threadsEdges[i][j];
			}
			for (int j = 0;j < threadsVertices[i].size();j++)
			{
				vertices[verticesUpToThread[i] + j] = threadsVertices[i][j];
			}
		}
	}
}*/

Graph Graph::loadUsersSubgraph(int n, bool multipleEdges)
{
	string directory;
	bool considerTimes;
	if (multipleEdges == true)
	{
		directory = "separated\\";
		considerTimes = true;
	}
	else
	{
		directory = "separated_no_multiple_edges\\";
		considerTimes = false;
	}
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\"+directory+"users_graph_" + dec2String(n * 2500000) + "_" + dec2String((n + 1) * 2500000) + ".dat";
	Graph result;
	result = loadUsersGraph(pathx,considerTimes);
	return result;
}

void Graph::saveTransactionsTimes()
{
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\my_transactions_info\\transactions_times.dat";
	FILE *save, *readTransactions;
	ifstream readBlocks;
	readBlocks.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\bh.dat");
	readTransactions = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\tx.dat", "r");
	vector<int> blocksTimes;
	blocksTimes.clear();
	blocksTimes.resize(600000);
	int blockId, timeStamp, nTxs, txId, nIns, nOuts;
	string hashx;
	while (!readBlocks.eof())
	{
		readBlocks >> blockId >> hashx >> timeStamp >> nTxs;
		blocksTimes[blockId] = timeStamp;
	}
	save = fopen(pathx.c_str(), "w+");
	while (!feof(readTransactions))
	{
		fscanf(readTransactions, "%i", &txId);
		fscanf(readTransactions, "%i", &blockId);
		fscanf(readTransactions, "%i", &nIns);
		fscanf(readTransactions, "%i", &nOuts);
		fprintf(save, "%d %lld\n", txId, blocksTimes[blockId]);
	}
	fclose(save);
	readBlocks.close();
	fclose(readTransactions);
}

void Graph::loadTransactionsTimes()
{
	transactionsTimes.clear();
	transactionsTimes.resize(transactionsNumber);
	FILE *read;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\my_transactions_info\\transactions_times.dat", "r");
	int txId;
	long long timex;
	while (!feof(read))
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%lld", &timex);
		transactionsTimes[txId] = timex;
	}
	fclose(read);
}

void Graph::createTransactionsTimesParallel(string pathx, int threadsNumber)
{
	FILE **readTransactions;
	readTransactions = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + "tx_separated\\" + dec2String(i);
		readTransactions[i] = fopen(pathxx.c_str(), "r");
	}
	ifstream readBlocks;
	readBlocks.open(pathx + "bh.dat");
	vector<int> blocksTimes;
	blocksTimes.clear();
	blocksTimes.resize(600000);
	int blockId, timeStamp, nTxs;
	string hashx;
	while (!readBlocks.eof())
	{
		readBlocks >> blockId >> hashx >> timeStamp >> nTxs;
		blocksTimes[blockId] = timeStamp;
	}
	vector<int> transactionsTimes;
	transactionsTimes.clear();
	transactionsTimes.resize(transactionsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			int blockId, txId, nIns, nOuts;
			while (!feof(readTransactions[i]))
			{
				fscanf(readTransactions[i], "%d %d %d %d", &txId, &blockId, &nIns, &nOuts);
				transactionsTimes[txId] = blocksTimes[blockId];
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)fclose(readTransactions[i]);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < edges.size();i++)edges[i].time = transactionsTimes[edges[i].time];
	}
}

void Graph::saveVerticesEdgesOverTime()
{
	loadTransactionsTimes();
	set <int> verticesUpToCurrentTime;
	int edgesNumberUpToCurrentTime = 0;
	vector<int> vertices;
	vector<Edge> edges;
	verticesUpToCurrentTime.clear();
	Graph graph;
	ofstream save;
	save.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_edges_over_time.txt");
	time_t unixTime;
	//struct tm *tm;
	for (int i = 0;i < 120;i++)
	{
		graph = loadUsersSubgraph(i);
		vertices = graph.getVertices();
		edges = graph.getEdges();
		unixTime = transactionsTimes[edges[edges.size()/2].time];
		struct tm *tm = localtime(&unixTime);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		FOREACH(v, vertices)if(*v>0)verticesUpToCurrentTime.insert(*v);
		edgesNumberUpToCurrentTime += edges.size();
		save << date << " " << verticesUpToCurrentTime.size() << " " << edgesNumberUpToCurrentTime << endl;
	}
	save.close();
}

void Graph::saveAverageDegreeOverTime()
{
	loadTransactionsTimes();
	double directedDegreeSumUpToCurrentTime = 0;
	double verticesNumberUpToCurrentTime = 0;
	ofstream save;
	save.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\average_degree_over_time.txt");
	Graph graph;
	time_t unixTime;
	vector<Edge> edges;
	for (int i = 0;i < 120;i++)
	{
		graph = loadUsersSubgraph(i);
		directedDegreeSumUpToCurrentTime += graph.directedDegreeSum();
		verticesNumberUpToCurrentTime += graph.verticesNumber();
		edges = graph.getEdges();
		unixTime = transactionsTimes[edges[edges.size() / 2].time];
		struct tm *tm = localtime(&unixTime);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		save << date << " " << directedDegreeSumUpToCurrentTime / verticesNumberUpToCurrentTime << endl;
	}
	save.close();
}

void Graph::saveBuyersSellersNumberOverTime()
{
	FILE *save, *read;
	string pathx;
	int vertex;
	long long balance;
	int buyersNumber, sellersNumber;
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\byrs_slrs_no_over_time.txt", "w");
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_" + dec2String(i) + ".dat";
		read = fopen(pathx.c_str(), "r");
		buyersNumber = 0;
		sellersNumber = 0;
		while (!feof(read))
		{
			fscanf(read, "%i %lld", &vertex, &balance);
			if (balance > 0)sellersNumber++;
			else if (balance < 0)buyersNumber++;
		}
		fprintf(save, "%d %d\n", buyersNumber, sellersNumber);
		fclose(read);
	}
	fclose(save);
}

void Graph::saveAverageVerticesBalancesOverTime()
{
	FILE *save,*read[120],*readBuyersSellersNo;
	string pathx;
	pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\average_vertices_balances_over_time.txt";
	save = fopen(pathx.c_str(), "w");
	readBuyersSellersNo = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\byrs_slrs_no_over_time.txt", "r");
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_" + dec2String(i) + ".dat";
		read[i] = fopen(pathx.c_str(), "r");
	}
	double totalPositiveBalanceUpToCurrentTime = 0;
	double totalNegativeBalanceUpToCurrentTime = 0;
	double averagePositiveBalanceUpToCurrentTime, averageNegativeBalanceUpToCurrentTime;
	int sellersNumberUpToCurrentTime, buyersNumberUpToCurrentTime;
	int vertex;
	long long balance;
	for (int i = 0;i < 120;i++)
	{
		totalPositiveBalanceUpToCurrentTime = 0;
		totalNegativeBalanceUpToCurrentTime = 0;
		fscanf(readBuyersSellersNo, "%i %i", &buyersNumberUpToCurrentTime, &sellersNumberUpToCurrentTime);
		while (!feof(read[i]))
		{
			fscanf(read[i], "%i %lld", &vertex, &balance);
			if(balance>0)totalPositiveBalanceUpToCurrentTime += double(balance);
			else if (balance < 0)totalNegativeBalanceUpToCurrentTime += double(balance);
		}
		fclose(read[i]);
		averagePositiveBalanceUpToCurrentTime = totalPositiveBalanceUpToCurrentTime / sellersNumberUpToCurrentTime;
		averageNegativeBalanceUpToCurrentTime = totalNegativeBalanceUpToCurrentTime / buyersNumberUpToCurrentTime;
		fprintf(save, "%.3lf %.3lf\n", averageNegativeBalanceUpToCurrentTime, averagePositiveBalanceUpToCurrentTime);
	}
	fclose(save);
}

void Graph::saveMedianVerticesBalancesOverTime()
{
	FILE *save, *read[120];
	string pathx;
	pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\median_vertices_balances_over_time.txt";
	save = fopen(pathx.c_str(), "w");
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_" + dec2String(i) + ".dat";
		read[i] = fopen(pathx.c_str(), "r");
	}
	
	int vertex;
	long long balance;
	vector <long long> positiveBalancesUpToCurrentTime;
	vector <long long> negativeBalancesUpToCurrentTime;
	for (int i = 0;i < 120;i++)
	{
		positiveBalancesUpToCurrentTime.clear();
		negativeBalancesUpToCurrentTime.clear();
		while (!feof(read[i]))
		{
			fscanf(read[i], "%i %lld", &vertex, &balance);
			if(balance>0)positiveBalancesUpToCurrentTime.push_back(balance);
			else if (balance < 0)negativeBalancesUpToCurrentTime.push_back(balance);
		}
		fclose(read[i]);
		sort(positiveBalancesUpToCurrentTime.begin(), positiveBalancesUpToCurrentTime.end());
		sort(negativeBalancesUpToCurrentTime.begin(), negativeBalancesUpToCurrentTime.end());
		fprintf(save, "%lld %lld\n", positiveBalancesUpToCurrentTime[positiveBalancesUpToCurrentTime.size()/2], negativeBalancesUpToCurrentTime[negativeBalancesUpToCurrentTime.size() / 2]);
	}
	fclose(save);
}

/*void Graph::saveBalancesClusteringCoefficients(int timeId)
{
	FILE *readBalances, *save;
	string pathx;
	pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_" + dec2String(timeId) + ".dat";
	readBalances = fopen(pathx.c_str(), "r");
	pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\balances_clustering_coefficients_over_time\\balances_coefficients_" + dec2String(timeId) + ".txt";
	save = fopen(pathx.c_str(), "w");
	double balancesSums[101] = { 0 };
	double balancesNumber[101] = { 0 };
	loadClusteringCoefficients(timeId);
	int vertex;
	long long balance;
	while (!feof(readBalances))
	{
		fscanf(readBalances, "%i %lld", &vertex, &balance);
		balancesSums[int(clusteringCoefficients[vertex])] += double(balance);
		balancesNumber[int(clusteringCoefficients[vertex])]++;
	}
	fclose(readBalances);
	for (int i = 0;i <= 100;i++)
	{
		fprintf(save,"[%d,%d] %.0lf\n", i, i + 1, balancesSums[i] / balancesNumber[i]);
	}
	fclose(save);
}*/

void Graph::saveLongtermUsersSubgraphContractedEdges()
{
	Graph graph = loadUsersGraph("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\long_term_subgraph2.dat");
	Graph graphContractedEdges = graph.contractEdges(true);
	graphContractedEdges.saveGraph("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\long_term_subgraph_contracted_edges.dat");
}

void Graph::saveActiveUsersSubgraphContractedEdges()
{
	Graph graph = loadUsersGraph("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\active_users_subgraph.dat");
	Graph graphContractedEdges = graph.contractEdges(true);
	graphContractedEdges.saveGraph("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\active_users_subgraph_contracted_edges.dat");
}

void Graph::createUsersIdDictionaries()
{
	FILE *readLongTerm, *readActive, *save;
	set<int> consideredIds;
	vector<int> ids2Save;
	int u, v, weight, time;
	readLongTerm = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph\\long_term_subgraph_contracted_edges.dat", "r");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph\\id_dictionary.dat", "w");
	while (!feof(readLongTerm))
	{
		fscanf(readLongTerm,"%i %i %i %i", &u, &v, &weight, &time);
		consideredIds.insert(u);
		consideredIds.insert(v);
	}
	FOREACH(vertex, consideredIds)ids2Save.push_back(*vertex);
	sort(ids2Save.begin(), ids2Save.end());
	for (int i = 0;i < ids2Save.size();i++)fprintf(save, "%d %d\n", ids2Save[i], i);
	consideredIds.clear();
	ids2Save.clear();
	fclose(readLongTerm);
	fclose(save);
	readActive = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\active_users_subgraph_contracted_edges.dat", "r");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\id_dictionary.dat", "w");
	while (!feof(readActive))
	{
		fscanf(readActive, "%i %i %i %i", &u, &v, &weight, &time);
		consideredIds.insert(u);
		consideredIds.insert(v);
	}
	FOREACH(vertex, consideredIds)ids2Save.push_back(*vertex);
	sort(ids2Save.begin(), ids2Save.end());
	for (int i = 0;i < ids2Save.size();i++)fprintf(save, "%d %d\n", ids2Save[i], i);
	fclose(readActive);
	fclose(save);
}

void Graph::saveSubgraphTimeInterval(string sourceGraphPath, string destinationGraphPath, int begTime, int endTime)
{
	FILE *read, *save;
	read = fopen(sourceGraphPath.c_str(), "r");
	save = fopen(destinationGraphPath.c_str(), "w");
	int u, v, time;
	long long weight;
	while (!feof(read))
	{
		fscanf(read, "%d %d %lld %d", &u, &v, &weight, &time);
		if (time >= begTime && time <= endTime)fprintf(save, "%d %d %lld %d\n", u, v, weight, time);
	}
	fclose(read);
	fclose(save);
}

void Graph::normalizeSnapshots(string directoryPath, int intervalInDays)
{
	FILE *read, *save;
	string pathx;
	set< pair<int, int> > consideredEdges;
	vector< pair<int, int> > allEdges;
	map<int, int> verticesIds;
	pathx = directoryPath + "id_dictionary.dat";
	read = fopen(pathx.c_str(), "r");
	int u, v;
	while (!feof(read))
	{
		fscanf(read, "%i %i", &u, &v);
		verticesIds[u] = v;
	}
	fclose(read);
	int begTime = 1232390000;
	int snapshotsNumber = int(double(1518134400 - begTime) / double(intervalInDays * 86400)); // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	string temp;
	time_t unixTime;
	char date[20];
	pathx = directoryPath + "long_term_subgraph_contracted_edges.dat";
	read = fopen(pathx.c_str(), "r");
	int weight, time;
	map< pair<int, int>, long long > edgesWeights;
	while (!feof(read))
	{
		fscanf(read, "%i %i %i %i", &u, &v, &weight, &time);
		if (time >= 1367107200)allEdges.push_back(make_pair(u, v));
		edgesWeights[make_pair(u, v)] = 0;
	}
	fclose(read);
	long long weightx;
	for (int i = 0;i <= snapshotsNumber;i++)
	{
		unixTime = begTime + i * intervalInDays * 86400;
		struct tm *tm = localtime(&unixTime);
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		temp = date;
		pathx = directoryPath + "snapshots\\" + temp + ".txt";
		read = fopen(pathx.c_str(), "r");
		consideredEdges.clear();
		while (!feof(read))
		{
			fscanf(read, "%i %i %lld", &u, &v, &weightx);
			consideredEdges.insert(make_pair(u, v));
			edgesWeights[make_pair(u, v)] = weightx;
		}
		fclose(read);
		pathx = directoryPath + "snapshots_normalized\\" + temp + ".txt";
		save = fopen(pathx.c_str(), "w");
		for (int i = 0;i < allEdges.size();i++)
		{
			if (consideredEdges.find(allEdges[i]) != consideredEdges.end())
			{
				fprintf(save, "%d %d %lld\n", verticesIds[allEdges[i].first], verticesIds[allEdges[i].second], edgesWeights[allEdges[i]]);
			}
			else
			{
				fprintf(save, "%d %d %d\n", verticesIds[allEdges[i].first], verticesIds[allEdges[i].second], 0);
			}
		}
		fclose(save);
	}
}

void Graph::normalizeSnapshotsParallel(string directoryPath, int intervalInDays, int threadsNumber)
{
	FILE *read, *save;
	string pathx;
	vector< pair<int, int> > allEdges;
	map<int, int> verticesIds;
	pathx = directoryPath + "id_dictionary.dat";
	read = fopen(pathx.c_str(), "r");
	int u, v;
	while (!feof(read))
	{
		fscanf(read, "%i %i", &u, &v);
		verticesIds[u] = v;
	}
	fclose(read);
	int begTime = 1232390000;
	int snapshotsNumber = int(double(1518134400 - begTime) / double(intervalInDays * 86400)); // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	pathx = directoryPath + "long_term_subgraph_contracted_edges.dat";
	read = fopen(pathx.c_str(), "r");
	int weight, time;
	while (!feof(read))
	{
		fscanf(read, "%i %i %i %i", &u, &v, &weight, &time);
		if (time >= 1367107200)allEdges.push_back(make_pair(u, v));// stands for 2013-04-28
	}
	fclose(read);
	long long weightx;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i <= snapshotsNumber;i++)
		{
			time_t unixTime = begTime + i * intervalInDays * 86400;
			struct tm *tm = localtime(&unixTime);
			char date[20];
			strftime(date, sizeof(date), "%Y-%m-%d", tm);
			string temp = date;
			string pathx = directoryPath + "snapshots_trans_value\\" + temp + ".txt";
			FILE *read, *save;
			read = fopen(pathx.c_str(), "r");
			set< pair<int, int> > consideredEdges;
			map< pair<int, int>, long long > edgesWeights;
			while (!feof(read))
			{
				fscanf(read, "%i %i %lld", &u, &v, &weightx);
				consideredEdges.insert(make_pair(u, v));
				edgesWeights[make_pair(u, v)] = weightx;
			}
			fclose(read);
			pathx = directoryPath + "snapshots_trans_value_normalized\\" + temp + ".txt";
			save = fopen(pathx.c_str(), "w");
			for (int i = 0;i < allEdges.size();i++)
			{
				if (consideredEdges.find(allEdges[i]) != consideredEdges.end())
				{
					fprintf(save, "%d %d %lld\n", verticesIds[allEdges[i].first], verticesIds[allEdges[i].second], edgesWeights[allEdges[i]]);
				}
				else
				{
					fprintf(save, "%d %d %d\n", verticesIds[allEdges[i].first], verticesIds[allEdges[i].second], 0);
				}
			}
			fclose(save);
		}
	}
}

void Graph::saveTransactionTimeSeries(string directoryPath, int intervalInDays)
{
	FILE *read;
	ifstream read2;
	ofstream saveTransNumber, saveTransNumberAverage, saveTransValue, saveTransValueAverage;
	saveTransNumber.open(directoryPath + "transaction_number_time_series.txt");
	saveTransNumberAverage.open(directoryPath + "transaction_number_average_time_series.txt");
	saveTransValue.open(directoryPath + "transaction_value_time_series.txt");
	saveTransValueAverage.open(directoryPath + "transaction_value_average_time_series.txt");
	int begTime = 1276128000;// stands for 2010 - 06 - 10
	int snapshotsNumber = int(double(1518134400 - begTime) / double(intervalInDays * 86400)); // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	long long transNumber, transValue,tempValue;
	double transNumber2,transValue2,tempNumber;
	string pathx, temp;
	time_t unixTime;
	char date[20];
	int u, v, fileSize;
	for (int i = 0;i < snapshotsNumber;i++)
	{
		unixTime = begTime + i * intervalInDays * 86400;
		struct tm *tm = localtime(&unixTime);
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		temp = date;
		pathx = directoryPath + "snapshots_trans_number\\"+temp + ".txt";
		read = fopen(pathx.c_str(), "r");
		fseek(read, 0, SEEK_END);
		fileSize = ftell(read);
		fclose(read);
		transNumber = 0;
		tempNumber = 0;
		if (fileSize > 0)
		{
			read = fopen(pathx.c_str(), "r");
			while (!feof(read))
			{
				fscanf(read, "%d %d %lld", &u, &v, &tempValue);
				transNumber += tempValue;
				tempNumber++;
			}
		}
		fclose(read);
		/*while (!read2.eof())
		{
			read2 >> u >> v >> tempValue;
			transNumber += tempValue;
			tempNumber++;
		}
		read2.close();*/
		if (tempNumber > 0)transNumber2 = double(transNumber) / tempNumber;
		else transNumber2 = 0;
		pathx = directoryPath + "snapshots_trans_value\\" + temp + ".txt";
		read = fopen(pathx.c_str(), "r");
		fseek(read, 0, SEEK_END);
		fileSize = ftell(read);
		fclose(read);
		//read2.open(pathx);
		transValue = 0;
		tempNumber = 0;
		if (fileSize > 0)
		{
			read = fopen(pathx.c_str(), "r");
			while (!feof(read))
			{
				fscanf(read, "%d %d %lld", &u, &v, &tempValue);
				transValue += tempValue;
				tempNumber++;
			}
		}
		fclose(read);
		/*while (!read2.eof())
		{
			read2 >> u >> v >> tempValue;
			transValue += tempValue;
			tempNumber++;
		}
		read2.close();*/
		if (tempNumber > 0)transValue2 = double(transValue) / tempNumber;
		else transValue2 = 0;
		saveTransNumber << temp << " " << transNumber << endl;
		saveTransNumberAverage << temp << " " << transNumber2 << endl;
		saveTransValue << temp << " " << transValue << endl;
		saveTransValueAverage << temp << " " << transValue2 << endl;
	}
	saveTransNumber.close();
	saveTransNumberAverage.close();
	saveTransValue.close();
	saveTransValueAverage.close();
}

void Graph::saveTransactionTimeSeriesComponent(string snapshotsPath, 
	string componentsPathTransNumber, string componentsPathTransValue, 
	int componentId, int intervalInDays)
{
	FILE *read;
	ofstream saveTransNumber, saveTransValue;
	saveTransNumber.open(snapshotsPath + "transaction_number_time_series_component_"+dec2String(componentId)+".txt");
	saveTransValue.open(snapshotsPath + "transaction_value_time_series_component_"+dec2String(componentId)+".txt");
	int begTime = 1367193600;// stands for 2013 - 04 - 29
	int snapshotsNumber = int(double(1518134400 - begTime) / double(intervalInDays * 86400)); // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	long long tempValue;
	double transNumber, transValue, tempx;
	string pathx, temp;
	time_t unixTime;
	char date[20];
	int u, v, fileSize, it;
	vector<double>componentTransNumberCoordinates, componentTransValueCoordinates;
	pathx = componentsPathTransNumber + "principal_components\\component_" + dec2String(componentId) + ".txt";
	read = fopen(pathx.c_str(), "r");
	while (!feof(read))
	{
		fscanf(read, "%lf", &tempx);
		componentTransNumberCoordinates.push_back(tempx);
	}
	fclose(read);
	pathx = componentsPathTransValue + "principal_components\\component_" + dec2String(componentId) + ".txt";
	read = fopen(pathx.c_str(), "r");
	while (!feof(read))
	{
		fscanf(read, "%lf", &tempx);
		componentTransValueCoordinates.push_back(tempx);
	}
	fclose(read);
	for (int i = 0;i < snapshotsNumber;i++)
	{
		unixTime = begTime + i * intervalInDays * 86400;
		struct tm *tm = localtime(&unixTime);
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		temp = date;
		pathx = snapshotsPath + "snapshots_trans_number_normalized\\" + temp + ".txt";
		read = fopen(pathx.c_str(), "r");
		it = 0;
		transNumber = 0;
		while (!feof(read))
		{
			fscanf(read, "%d %d %lld", &u, &v, &tempValue);
			transNumber += componentTransNumberCoordinates[it] * double(tempValue);
			it++;
		}
		fclose(read);
		pathx = snapshotsPath + "snapshots_trans_value_normalized\\" + temp + ".txt";
		read = fopen(pathx.c_str(), "r");
		transValue = 0;
		it = 0;
		while (!feof(read))
		{
			fscanf(read, "%d %d %lld", &u, &v, &tempValue);
			transValue += componentTransValueCoordinates[it] * double(tempValue);
			it++;
		}
		fclose(read);
		saveTransNumber << temp << " " << transNumber << endl;
		saveTransValue << temp << " " << transValue << endl;
	}
	saveTransNumber.close();
	saveTransValue.close();
}

void Graph::saveTransactionTimeSeriesMoreComponentsParallel(string snapshotsPath, 
	string componentsPathTransNumber, string componentsPathTransValue, 
	int lastComponentId, int intervalInDays, int threadsNumber)
{
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i <= lastComponentId;i++)
		{
			saveTransactionTimeSeriesComponent(snapshotsPath, 
				componentsPathTransNumber, componentsPathTransValue,
				i, intervalInDays);
		}
	}
}

void Graph::saveVarianceByIntervalsParallel(string componentsPath, string snapshotsPath, int threadsNumber,
	int componentId, int intervalInDays, int begTime)
{
	ifstream readComponentCoefficients;
	vector<double> componentCoefficients;
	if (componentId > -1)
	{
		readComponentCoefficients.open(componentsPath + "principal_components\\component_" + dec2String(componentId) + ".txt");
		double coeff;
		while (!readComponentCoefficients.eof())
		{
			readComponentCoefficients >> coeff;
			componentCoefficients.push_back(coeff);
		}
		readComponentCoefficients.close();
	}
	FILE *readSnapshot;
	int unixTimeUpperLimit = 1518134400;
	int snapshotsNumber = int(double(unixTimeUpperLimit - begTime) / 86400);
	vector<vector<pair<int,double> > > intervalData;
	intervalData.resize(intervalInDays);
	time_t unixTime;
	char date[20];
	string temp, pathx;
	int u, v, x;
	vector<pair<int,double> > row;
	ofstream saveVariance;
	map<pair<int, int>, int> edgesIds;
	unixTime = begTime;
	struct tm *tm = localtime(&unixTime);
	strftime(date, sizeof(date), "%Y-%m-%d", tm);
	temp = date;
	pathx = snapshotsPath + temp + ".txt";
	readSnapshot = fopen(pathx.c_str(), "r");
	int it = 0;
	while (!feof(readSnapshot))
	{
		fscanf(readSnapshot, "%d %d %d", &u, &v, &x);
		edgesIds[make_pair(u, v)] = it;
		it++;
	}
	fclose(readSnapshot);
	vector<vector<pair<int, int> > > snapshotEfficientData;
	snapshotEfficientData.resize(snapshotsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < snapshotsNumber;i++)
		{
			time_t unixTime = begTime + i * 86400;
			struct tm *tm = localtime(&unixTime);
			char date[20];
			strftime(date, sizeof(date), "%Y-%m-%d", tm);
			string temp = date;
			string pathx = snapshotsPath + temp + ".txt";
			FILE *readSnapshot;
			readSnapshot = fopen(pathx.c_str(), "r");
			int u, v, x;
			while (!feof(readSnapshot))
			{
				fscanf(readSnapshot, "%d %d %d", &u, &v, &x);
				if (x > 0)snapshotEfficientData[i].push_back(make_pair(edgesIds[make_pair(u, v)], x));
			}
			fclose(readSnapshot);
		}
	}
	vector<double> variance;
	variance.resize(snapshotsNumber - intervalInDays + 1);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i <= snapshotsNumber - intervalInDays;i++)
		{
			vector<vector<pair<int, double> > > intervalData;
			vector < pair<int, double> > row;
			intervalData.resize(intervalInDays);
			for (int j = 0;j < intervalInDays;j++)intervalData[j].clear();
			for (int j = 0;j < intervalInDays;j++)
			{
				row.clear();
				double sum = 0;
				for (int k = 0;k < snapshotEfficientData[i + j].size();k++)
				{
					row.push_back(make_pair(snapshotEfficientData[i + j][k].first, double(snapshotEfficientData[i + j][k].second)));
					sum += double(snapshotEfficientData[i + j][k].second)*double(snapshotEfficientData[i + j][k].second);
				}
				fclose(readSnapshot);
				if (sum > 0)
				{
					for (int k = 0;k < row.size();k++)
					{
						intervalData[j].push_back(make_pair(row[k].first, row[k].second / sqrt(sum)));
					}
				}
			}
			double sum = 0;
			vector<double> intervalValues;
			intervalValues.resize(intervalInDays);
			for (int j = 0;j < intervalInDays;j++)
			{
				intervalValues[j] = 0;
				for (int k = 0;k < intervalData[j].size();k++)
				{
					if (componentId > -1)
					{
						intervalValues[j] += componentCoefficients[intervalData[j][k].first] * intervalData[j][k].second;
					}
					else
					{
						intervalValues[j] += intervalData[j][k].second;
					}
				}
				sum += intervalValues[j];
			}
			double mean = sum / double(intervalInDays);
			variance[i] = 0;
			for (int j = 0;j < intervalInDays;j++)variance[i] += (mean - intervalValues[j])*(mean - intervalValues[j]);
			variance[i] /= double(intervalInDays - 1);
		}
	}
	if(componentId>-1)saveVariance.open(componentsPath + dec2String(componentId) + "_component_variance_in_time.txt");
	else saveVariance.open(componentsPath + "total_variance_in_time.txt");
	for (int i = 0;i <= snapshotsNumber - intervalInDays;i++)
	{
		time_t unixTime = begTime + i * 86400;
		struct tm *tm = localtime(&unixTime);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		string temp1 = date;
		time_t unixTime2 = begTime + (i+365) * 86400;
		struct tm *tm2 = localtime(&unixTime2);
		char date2[20];
		strftime(date2, sizeof(date2), "%Y-%m-%d", tm2);
		string temp2 = date2;
		saveVariance << temp1 << " -- " << temp2 << " " << variance[i] << endl;
	}
	saveVariance.close();
}

/*void Graph::createGraphSnapshotsParallel(string inputPath, string outputPath, int intervalInDays, 
	string parameter, int threadsNumber, bool performSave)
{
	loadUsersGraphParallel(inputPath, threadsNumber);
	int begTime = 1232390000;
	int snapshotsNumber = int(double(1518134400 - begTime) / double(intervalInDays * 86400))+1; // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	vector<vector<map<pair<int,int>,long long> > > threadsSnapshots;
	threadsSnapshots.resize(threadsNumber);
	for (int i = 0;i < threadsNumber;i++)threadsSnapshots[i].resize(snapshotsNumber);
	if (parameter == "transactions_number")
	{
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < edges.size();i++)
			{
				int snapshotId = int(double(edges[i].time - begTime) / double(intervalInDays * 86400));
				threadsSnapshots[omp_get_thread_num()][snapshotId][make_pair(edges[i].u,edges[i].v)]++;
			}
		}
	}
	else if (parameter == "transactions_value")
	{
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < edges.size();i++)
			{
				int snapshotId = int(double(edges[i].time - begTime) / double(intervalInDays * 86400));
				threadsSnapshots[omp_get_thread_num()][snapshotId][make_pair(edges[i].u, edges[i].v)] += 
					long long(edges[i].weight);
			}
		}
	}
	snapshots.clear();
	snapshots.resize(snapshotsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < snapshotsNumber;i++)
		{
			for (int j = 0;j < threadsNumber;j++)
			{
				FOREACH(elt, threadsSnapshots[j][i])
				{
					snapshots[i][elt->first] += (elt->second);
				}
			}
		}
	}
	if (performSave)
	{
		FILE **save;
		_setmaxstdio(snapshotsNumber + 100);
		save = new FILE*[snapshotsNumber + 100];
		string pathx, temp;
		time_t unixTime;
		char date[20];
		for (int i = 0;i < snapshotsNumber;i++)
		{
			unixTime = begTime + i * intervalInDays * 86400;
			struct tm *tm = localtime(&unixTime);
			strftime(date, sizeof(date), "%Y-%m-%d", tm);
			temp = date;
			pathx = outputPath + temp + ".txt";
			save[i] = fopen(pathx.c_str(), "w");
		}
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < snapshotsNumber;i++)
			{
				FOREACH(edgeData, snapshots[i])
				{
					fprintf(save[i], "%d %d %lld\n", ((*edgeData).first).first, ((*edgeData).first).second, edgeData->second);
				}
			}
		}
		for (int i = 0;i < snapshotsNumber;i++)fclose(save[i]);
	}
}*/

void Graph::saveTotalTransactionAmountOverTime()
{
	loadTransactionsTimes();
	double sumUpToCurrentTime = 0;
	ofstream save;
	save.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\total_trans_amount_over_time.txt");
	Graph graph;
	time_t unixTime;
	vector<Edge> edges;
	for (int i = 0;i < 120;i++)
	{
		graph = loadUsersSubgraph(i);
		sumUpToCurrentTime += graph.edgesWeightSum();
		edges = graph.getEdges();
		unixTime = transactionsTimes[edges[edges.size() / 2].time];
		struct tm *tm = localtime(&unixTime);
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		save << date << " " << sumUpToCurrentTime << endl;
	}
	save.close();
}

void Graph::saveGiniCoefficientUsersBalancesOverTime()
{
	FILE *save;
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\balances_gini_over_time.txt","w");
	FILE *read;
	string pathx;
	int v;
	long long balance;
	vector<double> balances2Consider;
	double totalSum;
	double denominator;
	double fraction2Subtract;
	double giniCoefficient;
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_" + dec2String(i) + ".dat";
		read = fopen(pathx.c_str(),"r");
		balances2Consider.clear();
		while (!feof(read))
		{
			fscanf(read, "%i %lld",&v, &balance);
			if (balance > 0)balances2Consider.push_back(balance);
		}
		fclose(read);
		sort(balances2Consider.begin(), balances2Consider.end());
		totalSum = 0;
		denominator = 0;
		for (int i = 0;i < balances2Consider.size();i++)
		{
			totalSum += double(i)*balances2Consider[i];
			denominator += balances2Consider[i];
		}
		denominator *= double(balances2Consider.size());
		fraction2Subtract = (double(balances2Consider.size()) + 1) / double(balances2Consider.size());
		giniCoefficient = double(2 * totalSum) / double(denominator) - fraction2Subtract;
		fprintf(save, "%.3lf\n", giniCoefficient);
	}
	fclose(save);
}

void Graph::createGraphSnapshots(string inputPath, string outputPath, int intervalInDays, string parameter)
{
	Graph graph = loadUsersGraph(inputPath, true);
	FILE *save[10000];
	int begTime = 1232390000;
	int snapshotsNumber = (1518134400-begTime) / (intervalInDays * 86400); // 1518134400 = 2018-02-09 - latest date of hungarian dataset
	_setmaxstdio(snapshotsNumber+100);
	string pathx, temp;
	time_t unixTime;
	char date[20];
	for (int i = 0;i <= snapshotsNumber;i++)
	{
		unixTime = begTime+i * intervalInDays * 86400;
		struct tm *tm = localtime(&unixTime);
		strftime(date, sizeof(date), "%Y-%m-%d", tm);
		temp = date;
		pathx = outputPath + temp + ".dat";
		save[i] = fopen(pathx.c_str(), "w");
	}
	vector<Edge> edges = graph.getEdges();
	set< pair<int, int> > consideredEdges;
	FOREACH(edge, edges)consideredEdges.insert(make_pair(edge->u, edge->v));
	map< pair<int, int>, set<pair<int,double> > > contractedEdgesTransactionInfo;
	FOREACH(edge, consideredEdges)contractedEdgesTransactionInfo[*edge].clear();
	FOREACH(edge, edges)contractedEdgesTransactionInfo[make_pair(edge->u,edge->v)].insert(make_pair(edge->time,edge->weight));
	long long parameterValues[10000];
	set <int> fileIds2Consider;
	int itx = 0;
	FOREACH(edge, consideredEdges)
	{
		fileIds2Consider.clear();
		for (int i = 0;i < 10000;i++)parameterValues[i] = 0;
		FOREACH(transInfo, contractedEdgesTransactionInfo[*edge])
		{
			fileIds2Consider.insert(((transInfo->first) - begTime) / (intervalInDays * 86400));
			if (parameter == "transaction_number")
			{
				parameterValues[((transInfo->first) - begTime) / (intervalInDays * 86400)] ++;
			}
			else if (parameter == "transaction_value")
			{
				parameterValues[((transInfo->first) - begTime) / (intervalInDays * 86400)] += long long(transInfo->second);
			}
		}
		FOREACH(idx, fileIds2Consider)
		{
			fprintf(save[*idx], "%d %d %lld\n", edge->first, edge->second, parameterValues[*idx]);
		}
		itx++;
	}
	for (int i = 0;i < snapshotsNumber;i++)fclose(save[i]);
}

void Graph::saveActiveUsersSubgraph(int usersNumber2Consider)
{
	Graph graph = loadUsersGraph("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\users_graph_times_converted.dat");
	vector<pair<int, int> > verticesTransactionsNumbers;
	for (int i = 0;i < usersNumber;i++)verticesTransactionsNumbers.push_back(make_pair(0, i));
	edges = graph.getEdges();
	FOREACH(edge, edges)
	{
		verticesTransactionsNumbers[edge->u].first++;
		verticesTransactionsNumbers[edge->v].first++;
	}
	sort(verticesTransactionsNumbers.rbegin(), verticesTransactionsNumbers.rend());
	vector<bool> activeUsers;
	activeUsers.resize(usersNumber);
	for (int i = 0;i < usersNumber;i++)activeUsers[i] = false;
	for (int i = 0;i < usersNumber2Consider;i++)activeUsers[verticesTransactionsNumbers[i].second] = true;
	FILE *save;
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\active_users_subgraph.dat", "w");
	FOREACH(edge, edges)
	{
		if (activeUsers[edge->u] == true && activeUsers[edge->v] == true)
		{
			fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), edge->time);
		}
	}
	fclose(save);
}

void Graph::saveGraph(string pathx)
{
	FILE *save;
	save = fopen(pathx.c_str(), "w");
	FOREACH(edge, edges)
	{
		fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), edge->time);
	}
	fclose(save);
}

void Graph::saveLongTermUsersSubgraph(int minimalIntervalInDays,int minimalTransactionsNumber)
{
	FILE *read, *save;
	vector<long long> usersFirstTransactionTimes;
	vector<long long> usersLastTransactionTimes;
	vector<int> usersTransactionsNumber;
	vector<Edge> edges;
	Edge edge;
	usersFirstTransactionTimes.resize(usersNumber);
	usersLastTransactionTimes.resize(usersNumber);
	usersTransactionsNumber.resize(usersNumber);
	for (int i = 0;i < usersNumber;i++)
	{
		usersFirstTransactionTimes[i] = -1;
		usersLastTransactionTimes[i] = -1;
		usersTransactionsNumber[i] = 0;
	}
	int u, v;
	long long value, time;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\users_graph_times_converted.dat", "r");
	while (!feof(read))
	{
		fscanf(read, "%i %i %lld %lld", &u, &v, &value, &time);
		if (usersFirstTransactionTimes[u] == -1)usersFirstTransactionTimes[u] = time;
		usersLastTransactionTimes[u] = time;
		if (usersFirstTransactionTimes[v] == -1)usersFirstTransactionTimes[v] = time;
		usersLastTransactionTimes[v] = time;
		usersTransactionsNumber[u]++;
		usersTransactionsNumber[v]++;
		edge.u = u;
		edge.v = v;
		edge.weight = double(value);
		edge.time = time;
		edges.push_back(edge);
	}
	fclose(read);
	vector<bool> longTermCore;
	longTermCore.resize(usersNumber);
	long long minimalIntervalInSeconds = long long(minimalIntervalInDays) * 86400;
	for (int i = 0;i < usersNumber;i++)
	{
		if ((usersLastTransactionTimes[i] - usersFirstTransactionTimes[i] >= minimalIntervalInSeconds) &&
			(usersTransactionsNumber[i] >= minimalTransactionsNumber))
		{
			longTermCore[i] = true;
		}
		else longTermCore[i] = false;
	}
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\long_term_subgraph.dat", "w");
	FOREACH(edge, edges)
	{
		if (longTermCore[edge->u] == true && longTermCore[edge->v] == true)
		{
			fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), edge -> time);
		}
	}
	fclose(save);
}

void Graph::saveVerticesBalancesOverTime()
{
	FILE *save[120];
	string pathx;
	for (int i = 0;i < 120;i++)
	{
		pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\vertices_balances_over_time\\vertices_balances_"+dec2String(i)+".dat";
		save[i] = fopen(pathx.c_str(), "w");
	}
	verticesBalances.clear();
	verticesBalances.resize(1500000);
	set <int> vertices2Consider;
	vertices2Consider.clear();
	vector <Edge> edges;
	Graph graph;
	for (int i = 0;i < 1500000;i++)verticesBalances[i] = moreThanPossibleBitcoinsAmount;
	for (int i = 0;i < 120;i++)
	{
		graph = loadUsersSubgraph(i, false);
		edges = graph.getEdges();
		FOREACH(edge, edges)
		{
			vertices2Consider.insert(edge->u);
			vertices2Consider.insert(edge->v);
			if (verticesBalances[edge->u] == moreThanPossibleBitcoinsAmount)verticesBalances[edge->u] = 0;
			if (verticesBalances[edge->v] == moreThanPossibleBitcoinsAmount)verticesBalances[edge->v] = 0;
			verticesBalances[edge->u] -= long long(edge->weight);
			verticesBalances[edge->v] += long long(edge->weight);
		}
		FOREACH(v, vertices2Consider)fprintf(save[i], "%d %lld\n", *v, verticesBalances[*v]);
		fclose(save[i]);
	}
}

void Graph::convertUsersGraphTimes()
{
	Graph graph= loadUsersGraph( "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\users_graph.dat" );
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\users_graph_times_converted.dat";
	FILE *save, *readTransactions;
	ifstream readBlocks;
	readBlocks.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\bh.dat");
	readTransactions = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\tx.dat", "r");
	vector<int> blocksTimes;
	blocksTimes.clear();
	blocksTimes.resize(600000);
	int blockId, timeStamp, nTxs, txId, nIns, nOuts;
	string hashx;
	while (!readBlocks.eof())
	{
		readBlocks >> blockId >> hashx >> timeStamp >> nTxs;
		blocksTimes[blockId] = timeStamp;
	}
	vector<int> transactionsTimes;
	transactionsTimes.clear();
	transactionsTimes.resize(300000000);
	while (!feof(readTransactions))
	{
		fscanf(readTransactions, "%i", &txId);
		fscanf(readTransactions, "%i", &blockId);
		fscanf(readTransactions, "%i", &nIns);
		fscanf(readTransactions, "%i", &nOuts);
		transactionsTimes[txId] = blocksTimes[blockId];
	}
	save = fopen(pathx.c_str(), "w+");
	edges = graph.getEdges();
	FOREACH(edge, edges)
	{
		fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, long long(edge->weight), transactionsTimes[edge->time]);
	}
	fclose(save);
	readBlocks.close();
	fclose(readTransactions);
}

void Graph::printNeighborsList()
{
	cout << "neighbors list:\n";
	for (int i = 0; i<vertices.size(); i++)
	{
		cout << vertices[i] << ": ";
		FOREACH(v, neighborsList[vertices[i]])cout << *v << " ";
		cout << endl;
	}
}

void Graph::printEdges()
{
	cout << "edges:\n";
	FOREACH(edge, edges)
	{
		cout << "(" << (*edge).u << "," << (*edge).v << "," << (*edge).weight << ")\n";
	}
}

Graph::Graph(vector<int> V, vector<Edge> E, int transactionsNumberx, int usersNumberx, int filesNumberx, long long moreThanPossibleBitcoinsAmountx)
{
	if (E.size() > 0 && V.size() == 0)
	{
		set <int> verticesx;
		FOREACH(edge, E)
		{
			verticesx.insert(edge->u);
			verticesx.insert(edge->v);
		}
		vertices.clear();
		FOREACH(v, verticesx)vertices.push_back(*v);
	}
	else vertices = V;
	edges = E;
	transactionsNumber = transactionsNumberx;
	usersNumber = usersNumberx;
	moreThanPossibleBitcoinsAmount = moreThanPossibleBitcoinsAmountx;
	filesNumber = filesNumberx;
}

vector<int> Graph::getVertices()
{
	return vertices;
}

vector<Edge> Graph::getEdges()
{
	return edges;
}

vector<set<int>> Graph::getNeighborsList()
{
	return neighborsList;
}

vector<int> Graph::getVerticesDegrees()
{
	return verticesDegrees;
}

void Graph::setVertices(vector<int> verts)
{
	vertices = verts;
}

void Graph::setEdges(vector<Edge> eds)
{
	edges = eds;
}

vector<map<pair<int, int>, long long>> Graph::getSnapshots()
{
	return snapshots;
}

void Graph::setFilesNumber(int filesNumberx)
{
	filesNumber = filesNumberx;
}

bool operator<(const Edge & P, const Edge & Q)
{
	pair<int, int> P1 = make_pair(P.u, P.v), P2 = make_pair(P.time, P.weight),
		Q1 = make_pair(Q.u, Q.v), Q2 = make_pair(Q.time, Q.weight);
	if (P1 < Q1)return true;
	if (P1 == Q1 && P2 <= Q2)return true;
	return false;
}

int maxValue(vector<int> vect)
{
	if (vect.size() == 0)return 0;
	int maxIndex = 0;
	for (int i = 1; i < vect.size(); i++)if (vect[i] > vect[maxIndex])maxIndex = i;
	return vect[maxIndex];
}

int maxValueParallel(vector<int> vect, int threadsNumber)
{
	if (vect.size() == 0)return 0;
	int maxVal = -1;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp parallel
		for (int i = 0; i < vect.size(); i++)if (vect[i] > maxVal)maxVal = vect[i];
	}
	return maxVal;
}

map<int, double> Graph::computeClusteringCoefficients(int approximationAccuracy)
{
	map<int, double> result;

	vector< set<int> > undirectedNeighborsSet = createUndirectedNeighborsSet();
	FOREACH(v, vertices)if (*v > 0)result[*v] = 0;
	set<pair<int, int> > adjacencyPairs;
	FOREACH(e, edges)if (adjacencyPairs.find(make_pair(e->u, e->v)) == adjacencyPairs.end())adjacencyPairs.insert(make_pair(e->u, e->v));

	double trianglesNumber, it = 0;
	vector <int> neighborsVector, neighbors2Consider;
	FOREACH(v, vertices)if (*v > 0&& undirectedNeighborsSet[*v].size() >= 2)
	{
		it = max(approximationAccuracy, 1);
		neighborsVector = vector<int>(undirectedNeighborsSet[*v].begin(), undirectedNeighborsSet[*v].end());
		neighbors2Consider.clear();
		for (int i = 0;i < neighborsVector.size();i += it)neighbors2Consider.push_back(neighborsVector[i]);
		trianglesNumber = 0;
		if (neighbors2Consider.size() < 2)neighbors2Consider.push_back(neighborsVector[neighborsVector.size() / 2]);
		for (int i = 0;i < neighbors2Consider.size();i++)for (int j = 0;j < i;j++)
		{
			if (adjacencyPairs.find(make_pair(neighbors2Consider[i], neighbors2Consider[j])) != adjacencyPairs.end())trianglesNumber++;
			if (adjacencyPairs.find(make_pair(neighbors2Consider[j], neighbors2Consider[i])) != adjacencyPairs.end())trianglesNumber++;
		}
		result[*v] = trianglesNumber / (double(neighbors2Consider.size())*double(neighbors2Consider.size() - 1));
	}

	return result;
}

vector<set<int>> Graph::determineConnectedComponents()
{
	vector < set <int> > result;
	vector < set <int> > undirectedNeighborsSet = createUndirectedNeighborsSet();
	set <int> toConsider, currentComponent;
	vector <int> currentLayer, nextLayer;
	vector <bool> consideredVertices(maxValue(vertices) + 1);

	FOREACH(v, vertices)consideredVertices[*v] = false;
	toConsider = set<int>(vertices.begin(), vertices.end());

	while (!toConsider.empty())
	{
		currentLayer = { *toConsider.begin() };
		currentComponent.clear();
		while (!currentLayer.empty())
		{
			nextLayer.clear();
			FOREACH(v, currentLayer)
			{
				consideredVertices[*v] = true;
				toConsider.erase(*v);
				currentComponent.insert(*v);
			}
			FOREACH(v, currentLayer)FOREACH(w, undirectedNeighborsSet[*v])if (!consideredVertices[*w])
			{
				nextLayer.push_back(*w);
				consideredVertices[*w] = true;
			}
			currentLayer = nextLayer;
		}
		result.push_back(currentComponent);
	}

	return result;
}

double Graph::edgesWeightSum()
{
	double result = 0;
	FOREACH(edge, edges)result += edge->weight;
	return result;
}
