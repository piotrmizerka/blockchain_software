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
	vector < set <int> > connectedComponents;
	vector< vector<int> >stronglyConnectedComponents;
	vector<long long> verticesBalances;
	vector <int> verticesDegrees;
	vector<map<pair<int, int>, long long> > snapshots;
	
	/// inner variables
	vector < set <int> > neighborsList;
	vector <long long> transactionsTimes;
	vector <bool> consideredVertices;
	set <int> toConsider, currentComponent;
	// data for SCC
	stack<int> S;
	vector<int> indices, lowlinks;
	vector<bool> onStack;
	int index;

public:
	/// functionalities

	// Computes the clustering coefficients vor each vertex. This is an approximate solution.
	// The approximation paramater, an integer "approximationAccuracy", specifies to consider only every approximationAccuracy neighbor for each vertex into account. For example, if the vertex v0 has neihbors v1, v2, v3, and v4, appearing in that order on the neighbors list of v0, and approximationAccuracy = 2, the function below takes into account the neihbors v1 and v3 only.
	map<int, double> computeClusteringCoefficients(int approximationAccuracy = 1);

	// connected components
	void determineConnectedComponents();
	void printConnectedComponents();
	// SCCs using Tarjans algorithm, source: https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm#Complexity
	void computeStronglyConnectedComponents();/*TO VERIFY*/
	void savePercentageVerticesBiggestSCCOverTime();/*TO VERIFY*/
	// vertices balances
	void saveVerticesBalancesOverTime();
	void saveAverageVerticesBalancesOverTime();
	void saveMedianVerticesBalancesOverTime();
	void saveBuyersSellersNumberOverTime();
	// vertices degrees
	void computeVerticesDegrees();
	double averageDirectedDegree();/*TO VERIFY*/
	double directedDegreeSum();
	void saveAverageDegreeOverTime();/*degree here donotes number of out-neighbors*/
	// transaction amount
	void saveTotalTransactionAmountOverTime();
	// Gini coefficient
	void saveGiniCoefficientUsersBalancesOverTime();
	// long-term core and active core
	void saveLongTermUsersSubgraph(int minimalIntervalInDays, int minimalTransactionsNumber);
	void saveActiveUsersSubgraph(int usersNumber2Consider);
	void saveLongtermUsersSubgraphContractedEdges();
	void saveActiveUsersSubgraphContractedEdges();
	// vertices and edges number
	double verticesNumber();
	void saveVerticesEdgesOverTime();
	// edge weight sum
	double edgesWeightSum();
	// graph snapshots
	void createGraphSnapshots(string inputPath, string outputPath, int intervalInDays, string parameter = "transaction_number");
	void normalizeSnapshots(string directoryPath, int intervalInDays);/*normalizes snapshots so that they include all possible edges (i.e. occuring in the last snapshots) and ids are from the set {0,...,n}*/
	void normalizeSnapshotsParallel(string directoryPath, int intervalInDays, int threadsNumber = omp_get_max_threads());
	void createGraphSnapshotsParallel(string inputPath, string outputPath, int intervalInDays,
		string parameter = "transactions_number", int threadsNumber = omp_get_max_threads(), bool performSave = false);
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
	// bfs
	void BFS(vector <int> firstLayer);
	// a.d. strongly connected components
	void strongConnect(int v);/*TO VERIFY*/
	// contracting edges
	Graph contractEdges(bool weightAsEdgesNumber = false);/*ADD EDGES COUNTER*/
	void contractEdgesSeparated();
	// other file operations
	void saveUsersGraph(int begTime, int endTime);
	Graph loadUsersGraph(string pathx, bool considerTimes = true);
	void loadUsersGraphParallel(string pathx, int threadsNumber = omp_get_max_threads());
	Graph loadUsersSubgraph(int n, bool multipleEdges = true);
	void saveGraph(string pathx);/*the best way to do this and the most general*/
	void createUsersIdDictionaries();/*normalizes ids of core networks to lie in the set {0,...,n}*/
	void saveSubgraphTimeInterval(string sourceGraphPath, string destinationGraphPath, int begTime, int endTime);/*saves subgraph of a given graph in a given time interval*/

	/// printing methods
	void printNeighborsList();
	void printEdges();

	/// constructurs, gets and sets
	Graph(vector <int> V = vector <int>(0), vector < Edge > E = vector<Edge>(0), 
		int transactionsNumberx = 300000000, int usersNumberx = 1500000, int filesNumberx = 100, long long moreThanPossibleBitcoinsAmountx = 3000000000000000);
	vector <int> getVertices();
	vector < Edge > getEdges();
	vector < set <int> > getConnectedComponents();
	vector < set<int> > getNeighborsList();
	vector <int> getVerticesDegrees();
	vector< vector<int> > getStronglyConnectedComponents();
	void setVertices(vector <int> verts);
	void setEdges(vector< Edge > eds);
	vector<map<pair<int, int>, long long> > getSnapshots();
	void setFilesNumber(int filesNumberx);
};