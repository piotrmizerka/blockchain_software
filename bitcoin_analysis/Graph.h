#pragma once

struct Edge
{
	int u, v, time;
	double weight;
};

bool operator < (const Edge &P, const Edge &Q);

int maxValue(vector <int> vect);
int maxValueParallel(vector<int> vect, int threadsNumber = omp_get_num_threads()); // TO VERIFY

class Graph
{
	/// parameters
	vector <int> vertices;
	vector < Edge > edges;
	int transactionsNumber = 300000000;
	int usersNumber = 1500000; // max users number bound
	long long moreThanPossibleBitcoinsAmount = 3000000000000000;
	int filesNumber;

	/// functional variables
	vector<map<pair<int, int>, long long> > snapshots;
	
	/// inner variables
	vector < set <int> > neighborsList;
	vector <long long> transactionsTimes;

public:
	/// functionalities
	// long-term core and active core
	void saveLongTermUsersSubgraph(int minimalIntervalInDays, int minimalTransactionsNumber);
	void saveActiveUsersSubgraph(int usersNumber2Consider);
	void saveLongtermUsersSubgraphContractedEdges();
	void saveActiveUsersSubgraphContractedEdges();

	// vertices and edges number
	double verticesNumber();
	void saveVerticesEdgesOverTime();

	// graph snapshots
	void createGraphSnapshots(string inputPath, string outputPath, int intervalInDays, string parameter = "transaction_number");
	void normalizeSnapshots(string directoryPath, int intervalInDays);/*normalizes snapshots so that they include all possible edges (i.e. occuring in the last snapshots) and ids are from the set {0,...,n}*/
	void normalizeSnapshotsParallel(string directoryPath, int intervalInDays, int threadsNumber = omp_get_max_threads());
	//void createGraphSnapshotsParallel(string inputPath, string outputPath, int intervalInDays,
	//	string parameter = "transactions_number", int threadsNumber = omp_get_max_threads(), bool performSave = false);
	void separeteUsersGraph();/*separating users graph into 120 subgraphs according to transaction date (more or less :))*/
	void saveTransactionTimeSeries(string directoryPath, int intervalInDays = 1);
	void saveTransactionTimeSeriesComponent(string snapshotsPath, 
		string componentsPathTransNumber, string componentsPathTransValue,
		int componentId, int intervalInDays = 1);
	void saveTransactionTimeSeriesMoreComponentsParallel(string snapshotsPath, 
		string componentsPathTransNumber, string componentsPathTransValue,
		int lastComponentId = 5, int intervalInDays = 1, int threadsNumber = omp_get_max_threads());

	// pca
	void saveVarianceByIntervalsParallel(string componentsPath, string snapshotsPath, int threadsNumber = omp_get_max_threads(),
		int componentId = 0, int intervalInDays = 365, int begTime = 1367107200); /*componentId = -1 means calculating total variance*/

	/// inner methods
	// neighbors list
	vector < set <int> > createUndirectedNeighborsSet();
	void createDirectedNeighborsList();

	// transaction times
	void saveTransactionsTimes();
	void loadTransactionsTimes();
	void convertUsersGraphTimes();
	void createTransactionsTimesParallel(string pathx, int threadsNumber = omp_get_max_threads());

	// contracting edges
	Graph contractEdges(bool weightAsEdgesNumber = false);/*ADD EDGES COUNTER*/
	void contractEdgesSeparated();

	// other file operations
	void saveUsersGraph(int begTime, int endTime);
	Graph loadUsersGraph(string pathx, bool considerTimes = true);
	//void loadUsersGraphParallel(string pathx, int threadsNumber = omp_get_max_threads());
	Graph loadUsersSubgraph(int n, bool multipleEdges = true);
	void saveGraph(string pathx);/*the best way to do this and the most general*/
	void createUsersIdDictionaries();/*normalizes ids of core networks to lie in the set {0,...,n}*/
	void saveSubgraphTimeInterval(string sourceGraphPath, string destinationGraphPath, int begTime, int endTime);/*saves subgraph of a given graph in a given time interval*/

	// printing methods
	void printNeighborsList();
	void printEdges();

	/// constructurs, gets and sets
	Graph(vector <int> V = vector <int>(0), vector < Edge > E = vector<Edge>(0), 
		int transactionsNumberx = 300000000, int usersNumberx = 1500000, int filesNumberx = 100, long long moreThanPossibleBitcoinsAmountx = 3000000000000000);
	vector <int> getVertices();
	vector < Edge > getEdges();
	vector < set<int> > getNeighborsList();
	void setVertices(vector <int> verts);
	void setEdges(vector< Edge > eds);
	vector<map<pair<int, int>, long long> > getSnapshots();
	void setFilesNumber(int filesNumberx);
};