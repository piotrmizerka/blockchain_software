#pragma once


struct Transaction
{
	int id;
	vector <int> in;
	vector <int> out;
	map < int, vector<double> > amountsReceived;
	double fee;
};

class TransactionHypergraph
{
	/// parameters
	vector <int> addresses;
	vector <Transaction> transactions;

	/// functional variables
	vector < set<int> > clusters;
	Graph usersGraph;

public:
	/// functionalities
	void cluster();
	void createUsersGraph();
	void saveClustersFrequency(int interval);

	/// printing methods
	void printClusters();
	void printUsersGraph();
	void printTransactions();

	/// constructors, gets and sets
	TransactionHypergraph(vector<int> addrs = vector<int>(0), vector <Transaction> trans = vector<Transaction>(0));
	vector <int> getAddresses();
	vector <Transaction> getTransactions();
	vector < set<int> > getClusters();
	Graph getUsersGraph();
	void setAddresses(vector <int> addr);
	void setTransactions(vector< Transaction> trans);
};

