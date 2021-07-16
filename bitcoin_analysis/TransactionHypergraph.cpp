#include "stdafx.h"

void TransactionHypergraph::cluster()
{
	vector < Edge > edges;
	Edge edge;
	for (int i = 0; i<transactions.size(); i++)
	{
		for (int j = 0; j<transactions[i].in.size() - 1; j++)
		{
			edge.u = transactions[i].in[j];
			edge.v = transactions[i].in[j + 1];
			edges.push_back(edge);
		}
	}
	Graph graph(addresses, edges);
	graph.createUndirectedNeighborsSet();
	graph.determineConnectedComponents();
	clusters = graph.getConnectedComponents();
}

void TransactionHypergraph::createUsersGraph()
{
	cluster();
	vector < set<int> > idClusters(clusters.size());
	int maxVal = maxValue(addresses);
	vector <int> addressIdClusters(maxVal+1);
	for (int i = 0; i<clusters.size(); i++)
	{
		idClusters[i] = clusters[i];
		for (set<int>::iterator it = clusters[i].begin(); it != clusters[i].end(); ++it)
		{
			addressIdClusters[*it] = i;
		}
	}
	vector <int> vertices(clusters.size());
	vector < Edge > edges;
	Edge edge2Add;
	for (int i = 0; i<clusters.size(); i++)vertices[i] = i;
	for (vector<Transaction>::iterator it = transactions.begin(); it != transactions.end(); ++it)
	{
		for (vector<int>::iterator jt = (*it).out.begin(); jt != (*it).out.end(); ++jt)
		{
			for (vector<double>::iterator kt = (*it).amountsReceived[*jt].begin(); kt != (*it).amountsReceived[*jt].end(); ++kt)
			{
				edge2Add.u = addressIdClusters[(*it).in[0]];
				edge2Add.v = addressIdClusters[*jt];
				edge2Add.weight = *kt;
				edge2Add.time = (*it).id;
				edges.push_back(edge2Add);
			}
		}
	}
	usersGraph = Graph(vertices, edges);
}

void TransactionHypergraph::saveClustersFrequency(int interval)
{
	FILE *read, *save;
	int intervalsNumber = 12500000 / interval;
	int clustersFrequencies[100000];
	for (int i = 0;i < intervalsNumber;i++)clustersFrequencies[i] = 0;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\clusters_sizes.dat", "r");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\clusters_sizes_frequencies.txt", "w");
	int sizex;
	while (!feof(read))
	{
		fscanf(read, "%i", &sizex);
		clustersFrequencies[sizex / interval]++;
	}
	fclose(read);
	for (int i = intervalsNumber - 1;i >= 0;i--)
	{
		if (clustersFrequencies[i] > 0)
		{
			fprintf(save, "%d %d %d\n", i*interval, (i + 1)*interval, clustersFrequencies[i]);
		}
	}
	fclose(save);
}

void TransactionHypergraph::printClusters()
{
	cout << "clusters:\n";
	for (vector< set<int> >::iterator it = clusters.begin(); it != clusters.end(); ++it)
	{
		for (set<int>::iterator jt = (*it).begin(); jt != (*it).end(); ++jt)cout << *jt << " ";
		cout << endl;
	}
}

void TransactionHypergraph::printUsersGraph()
{
	cout << "users graph:\n";
	vector <Edge> edges = usersGraph.getEdges();
	FOREACH(edge, edges)
	{
		cout << "(" << edge->u << "," << edge->v << "," << edge->weight << ")\n";
	}
}

void TransactionHypergraph::printTransactions()
{
	int i = 0;
	cout << "transactions:\n";
	FOREACH(transaction, transactions)
	{
		cout << i << ":\n\tin: ";
		FOREACH(a1, transaction->in)cout << *a1 << " ";
		cout << "\n";
		cout << "\tout: ";
		FOREACH(a2, transaction->out)cout << *a2 << " ";
		cout << endl;
		cout << "\tdistribution of amounts received in out:\n";
		FOREACH(it, transaction->amountsReceived)
		{
			cout << "\t" << it->first << ": ";
			FOREACH(jt, it->second)cout << *jt << " ";
			cout << endl;
		}
		i++;
	}
}

TransactionHypergraph::TransactionHypergraph(vector<int> addrs, vector<Transaction> trans)
{
	addresses = addrs;
	transactions = trans;
}

vector<int> TransactionHypergraph::getAddresses()
{
	return addresses;
}

vector<Transaction> TransactionHypergraph::getTransactions()
{
	return transactions;
}

vector<set<int> > TransactionHypergraph::getClusters()
{
	return clusters;
}

Graph TransactionHypergraph::getUsersGraph()
{
	return usersGraph;
}

void TransactionHypergraph::setAddresses(vector<int> addr)
{
	addresses = addr;
}

void TransactionHypergraph::setTransactions(vector<Transaction> trans)
{
	transactions = trans;
}

