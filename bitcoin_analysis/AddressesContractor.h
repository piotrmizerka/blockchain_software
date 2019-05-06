#pragma once

class AddressesContractor
{
	/// parameters
	int transactionsNumber = 300000000;
	int addressesNumber = 400000000;
	int usersNumber = 160000000;
	int filesNumber = 4000;
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\";

	/// functional variables
	vector < set<int> > contractedAddresses;
	vector < vector<int> > transactionsAddresses;
	vector <int> clustersSizes;

	/// innner variables
	vector <int> transactionsRepresentatives;
	vector < set<int> > neighborsList; 
	vector <int> allTransactions;
	vector < short int > consideredAddresses;
	set <int> currentComponent;
	vector <int> addressesVertices;
	set <int> currentLayer, nextLayer, layer;
	vector <bool> consideredTransactions;
	vector< pair<int, int> > edges;
	vector<int> addresses;
	vector<bool> starx, single;
	vector<int> componentRepresentatives; /*assigns every vertex a component representative vertex*/
	vector<int> P; /*temporary copy of componentRepresentatives*/
	vector<int> normalizedLabels;

public:

	bool forestCheck; //TO ERASE IN FUTURE

	/// functionalities
	// contracting addresses
	void contractAddresses(string relativePath = "");
	void contractAdddressesFromRepresentatives(); // TO VERIFY
	void contractAddressesParallel(string relativePath = "", int threadsNumber = omp_get_max_threads(), bool loadEdges = false); // TODO - not working properly due to map usage
	void restrictContractedAddresses(int minimalClusterSize);
	// transaction addresses
	void restrictTransactionsAddresses();
	void restrictTransactionsAddressesParallel(); // TODO
	void createTransactionsAddresses();
	void createTransactionsAddressesParallel(string relativePath = "", int threadsNumber = omp_get_max_threads(), bool performSave = true); //CAUTION: map used
	void loadTransactionsAddresses();
	void loadTransactionsAddressesParallel(string relativePath = "", int threadsNumber = omp_get_max_threads());
	// cluster sizes
	void saveBiggestClustersSizes();
	// creating users graph
	void createUsersGraph();
	void createUsersGraphParallel(string relativePath = "", int threadsNumber = omp_get_max_threads()); // TO VERIFY - not working properly

	/// inner methods
	// neighbors list
	void createNeighborsList();
	void createNeighborsListParallel(string relativePath = "", int threadsNumber = omp_get_max_threads());
	void createNeighborsListFromEdges(); // TO VERIFY
	void loadNeighborsList();
	void loadNeighborsListParallel(string relativePath = "", int threadsNumber = omp_get_max_threads());
	// edges
	void createEdgesParallel(string relativePath = "", int threadsNumber = omp_get_max_threads(), bool performSave = true);
	void loadEdgesParallel(string relativePath = "", int threadsNumber = omp_get_max_threads());
	// other methods
	void restrictOut();
	void contractAddressesBFS();
	void BFS();
	void labelConnectedComponentsParallel(int threadsNumber = omp_get_max_threads()); /* https://edux.fit.cvut.cz/oppa/PI-PPA/prednasky/PI-PPA2011-5.pdf */
	void hook(int u, int v);
	void star(int v);
	void normalizeLabelsParallel(int threadsNumber = omp_get_max_threads()); // TO VERIFY

	/// constructor, gets
	AddressesContractor(int transactionsNumberx = 300000000, int addressesNumberx = 400000000, int usersNumberx = 160000000,
		int filesNumberx = 4000, string pathxx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\");
	vector < vector<int> > getTransactionsAddresses();
	vector< set<int> > getNeighborsList();
	vector<pair<int, int> > getEdges();
	vector<int> getAddresses();
	vector<set<int> > getContractedAddresses();
	vector<int> getComponentRepresentatives();
};