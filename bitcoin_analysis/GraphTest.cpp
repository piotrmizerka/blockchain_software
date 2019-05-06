#include "stdafx.h"

void GraphTest::computeClusteringCoefficientsTest(bool smallTests, bool bigTests)
{
	cout << "computeClusteringCoefficients test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "computeClusteringCoefficients\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			computeClusteringCoefficientsVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "computeClusteringCoefficients\\big_in_";
		loadTestIn(relativePath);
		computeClusteringCoefficientsVerify(false);
	}
}

void GraphTest::loadTestIn(string relativePath, bool readWeights, bool readVertices)
{
	FILE *readIn;
	string pathxx;
	vector <int> vertices;
	vector <Edge> edges;
	Edge edge;
	pathxx = pathx + relativePath+"edges.txt";
	readIn = fopen(pathxx.c_str(), "r");
	edges.clear();
	if (readWeights == false)
	{
		while (!feof(readIn))
		{
			fscanf(readIn, "%i %i", &edge.u, &edge.v);
			edges.push_back(edge);
		}
	}
	else
	{
		while (!feof(readIn))
		{
			fscanf(readIn, "%i %i %lf", &edge.u, &edge.v, &edge.weight);
			edges.push_back(edge);
		}
	}
	fclose(readIn);
	if (readVertices == true)
	{
		pathxx = pathx + relativePath + "vertices.txt";
		readIn = fopen(pathxx.c_str(), "r");
		vertices.clear();
		while (!feof(readIn))
		{
			fscanf(readIn, "%i", &edge.u);
			vertices.push_back(edge.u);
		}
		fclose(readIn);
	}
	testGraph = Graph(vertices, edges, 2 * edges.size(), 2 * vertices.size(), 100000);
}

void GraphTest::computeClusteringCoefficientsVerify(bool smallTests, int smallId)
{
	string pathxx;
	FILE *readOut;
	map<int, double> clusteringCoeffs;
	vector <int> vertices;
	double averageRelativeError;
	double relativeErrorSum = 0;
	double coeffx;
	int v;
	if (smallTests == true)pathxx = pathx + "computeClusteringCoefficients\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "computeClusteringCoefficients\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testGraph.computeClusteringCoefficients();
	clusteringCoeffs = testGraph.getClusteringCoefficients();
	while (!feof(readOut))
	{
		fscanf(readOut, "%i %lf", &v, &coeffx);
		if (abs(coeffx-clusteringCoeffs[v])>0.001)
		{
			relativeErrorSum += abs(coeffx - clusteringCoeffs[v]) / max(coeffx, clusteringCoeffs[v]);
		}
	}
	fclose(readOut);
	vertices = testGraph.getVertices();
	averageRelativeError = relativeErrorSum / double(vertices.size());
	if (smallTests == true) cout << "small test " << smallId << " average relative error: " << averageRelativeError << endl;
	else cout << "big test average relative error: " << averageRelativeError << endl;
}

void GraphTest::computeVerticesDegreesVerify(bool smallTests, int smallId)
{
	string pathxx;
	FILE *readOut;
	vector<int> verticesDegrees, verticesDegrees2;
	vector <int> vertices;
	int v, degree;
	if (smallTests == true)pathxx = pathx + "computeVerticesDegrees\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "computeVerticesDegrees\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testGraph.computeVerticesDegrees();
	verticesDegrees = testGraph.getVerticesDegrees();
	verticesDegrees2.resize(2*verticesDegrees.size());
	bool check = true;
	while (!feof(readOut))
	{
		fscanf(readOut, "%i %i", &v, &degree);
		if (degree!=verticesDegrees[v])check = false;
		verticesDegrees2[v] = degree;
	}
	fclose(readOut);
	if (smallTests == true)
	{
		if (check == true)cout << "small test " << smallId << ": OK\n";
		else
		{
			vertices = testGraph.getVertices();
			cout << "small test " << smallId << ": FAILED\n";
			cout << "Proper degrees:\n";
			FOREACH(v, vertices)cout << *v << ": " << verticesDegrees2[*v] << endl;
			cout << "Obtained degrees:\n";
			FOREACH(v, vertices)cout << *v << ": " << verticesDegrees[*v] << endl;
		}
	}
	else
	{
		if (check == true)cout << "big test: OK\n";
		else cout << "big test: FAILED";
	}
}

void GraphTest::averageDirectedDegreeVerify(bool smallTests, int smallId)
{
	string pathxx;
	FILE *readOut;
	vector <int> vertices;
	int v;
	if (smallTests == true)pathxx = pathx + "averageDirectedDegree\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "averageDirectedDegree\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	double averageDirectedDegree = testGraph.averageDirectedDegree();
	double averageDirectedDegree2;
	fscanf(readOut, "%lf", &averageDirectedDegree2);
	fclose(readOut);
	bool check = true;
	if (abs(averageDirectedDegree2 - averageDirectedDegree) > 0.001)check = false;
	if (smallTests == true)
	{
		if (check == true)cout << "small test " << smallId << ": OK\n";
		else
		{
			cout << "small test " << smallId << ": FAILED\n";
			cout << "Proper average directed degree: " << averageDirectedDegree2 << endl;
			cout << "Obtained average directoed degree: " << averageDirectedDegree << endl;
		}
	}
	else
	{
		if (check == true)cout << "big test: OK\n";
		else cout << "big test: FAILED\n";
	}
}

void GraphTest::directedDegreeSumVerify(bool smallTests, int smallId)
{
	string pathxx;
	FILE *readOut;
	vector <int> vertices;
	int v;
	if (smallTests == true)pathxx = pathx + "directedDegreeSum\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "directedDegreeSum\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	double directedDegreeSum = testGraph.directedDegreeSum();
	double directedDegreeSum2;
	fscanf(readOut, "%lf", &directedDegreeSum2);
	fclose(readOut);
	bool check = true;
	if (abs(directedDegreeSum2 - directedDegreeSum) > 0.001)check = false;
	if (smallTests == true)
	{
		if (check == true)cout << "small test " << smallId << ": OK\n";
		else
		{
			cout << "small test " << smallId << ": FAILED\n";
			cout << "Proper directed degree sum: " << directedDegreeSum2 << endl;
			cout << "Obtained directed degree sum: " << directedDegreeSum << endl;
		}
	}
	else
	{
		if (check == true)cout << "big test: OK\n";
		else cout << "big test: FAILED\n";
	}
}

void GraphTest::edgesWeightSumVerify(bool smallTests, int smallId)
{
	string pathxx;
	FILE *readOut;
	vector <int> vertices;
	int v;
	if (smallTests == true)pathxx = pathx + "edgesWeightSum\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "edgesWeightSum\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	double edgesWeightSum = testGraph.edgesWeightSum();
	double edgesWeightSum2;
	fscanf(readOut, "%lf", &edgesWeightSum2);
	fclose(readOut);
	bool check = true;
	if (abs(edgesWeightSum2 - edgesWeightSum) > 0.001)check = false;
	if (smallTests == true)
	{
		if (check == true)cout << "small test " << smallId << ": OK\n";
		else
		{
			cout << "small test " << smallId << ": FAILED\n";
			cout << "Proper edges weight sum: " << edgesWeightSum2 << endl;
			cout << "Obtained edges weight sum: " << edgesWeightSum << endl;
		}
	}
	else
	{
		if (check == true)cout << "big test: OK\n";
		else cout << "big test: FAILED\n";
	}
}

GraphTest::GraphTest(int filesNumberx, string pathxx)
{
	pathx = pathxx;
	filesNumber = filesNumberx;
}

void GraphTest::computeStronglyConnectedComponentsVerify(bool smallTests, int smallId)
{
	vector <int> vertices;
	set<int> verticesx;
	vector <Edge> edges;
	Edge edge;
	vector < set <int> > connectedComponents, connectedComponents2;
	vector < vector<int> > connectedComponentsx;
	set<int> component;
	vector <int> componentIds, componentIds2;
	string pathxx;
	FILE *readOut;
	if (smallTests == true)pathxx = pathx + "computeStronglyConnectedComponents\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "computeStronglyConnectedComponents\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testGraph.computeStronglyConnectedComponents();
	connectedComponentsx = testGraph.getStronglyConnectedComponents();
	connectedComponents.clear();
	FOREACH(componentx, connectedComponentsx)
	{
		component.clear();
		FOREACH(v, *componentx)component.insert(*v);
		connectedComponents.push_back(component);
	}
	connectedComponents2.clear();
	int n, v;
	while (!feof(readOut))
	{
		component.clear();
		fscanf(readOut, "%i", &n);
		for (int i = 0;i < n;i++)
		{
			fscanf(readOut, "%i", &v);
			component.insert(v);
		}
		connectedComponents2.push_back(component);
	}
	fclose(readOut);
	vertices = testGraph.getVertices();
	componentIds.resize(10 * vertices.size());
	componentIds2.resize(10 * vertices.size());
	for (int i = 0;i < connectedComponents.size();i++)FOREACH(v, connectedComponents[i])componentIds[*v] = i;
	for (int i = 0;i < connectedComponents2.size();i++)FOREACH(v, connectedComponents2[i])componentIds2[*v] = i;
	bool check = true;
	for (int i = 0;i < vertices.size();i++)
	{
		if (connectedComponents[componentIds[vertices[i]]].size() != connectedComponents2[componentIds2[vertices[i]]].size())
		{
			check = false;
		}
		else
		{
			FOREACH(v, connectedComponents[componentIds[vertices[i]]])
			{
				if (connectedComponents2[componentIds2[vertices[i]]].find(*v) == connectedComponents2[componentIds2[vertices[i]]].end())
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
			cout << "FAILED\n\Proper strongly connected components:\n";
			FOREACH(component, connectedComponents2)
			{
				FOREACH(v, *component)cout << *v << " ";
				cout << endl;
			}
			cout << "Obtained strongly connected components:\n";
			FOREACH(component, connectedComponents)
			{
				FOREACH(v, *component)cout << *v << " ";
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

void GraphTest::determineConnectedComponentsVerify(bool smallTests, int smallId)
{
	vector <int> vertices;
	set<int> verticesx;
	vector <Edge> edges;
	Edge edge;
	vector < set <int> > connectedComponents, connectedComponents2;
	set<int> component;
	vector <int> componentIds, componentIds2;
	string pathxx;
	FILE *readOut;
	if(smallTests==true)pathxx = pathx + "determineConnectedComponents\\small" + dec2String(smallId) + "_out.txt";
	else pathxx = pathx + "determineConnectedComponents\\big_out.txt";
	readOut = fopen(pathxx.c_str(), "r");
	testGraph.determineConnectedComponents();
	connectedComponents = testGraph.getConnectedComponents();
	connectedComponents2.clear();
	int n, v;
	while (!feof(readOut))
	{
		component.clear();
		fscanf(readOut, "%i", &n);
		for (int i = 0;i < n;i++)
		{
			fscanf(readOut, "%i", &v);
			component.insert(v);
		}
		connectedComponents2.push_back(component);
	}
	fclose(readOut);
	vertices = testGraph.getVertices();
	componentIds.resize(10 * vertices.size());
	componentIds2.resize(10 * vertices.size());
	for (int i = 0;i < connectedComponents.size();i++)FOREACH(v, connectedComponents[i])componentIds[*v] = i;
	for (int i = 0;i < connectedComponents2.size();i++)FOREACH(v, connectedComponents2[i])componentIds2[*v] = i;
	bool check = true;
	for (int i = 0;i < vertices.size();i++)
	{
		if (connectedComponents[componentIds[vertices[i]]].size() != connectedComponents2[componentIds2[vertices[i]]].size())
		{
			check = false;
		}
		else
		{
			FOREACH(v, connectedComponents[componentIds[vertices[i]]])
			{
				if (connectedComponents2[componentIds2[vertices[i]]].find(*v) == connectedComponents2[componentIds2[vertices[i]]].end())
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
			cout << "FAILED\n\Proper connected components:\n";
			FOREACH(component, connectedComponents2)
			{
				FOREACH(v, *component)cout << *v << " ";
				cout << endl;
			}
			cout << "Obtained connected components:\n";
			FOREACH(component, connectedComponents)
			{
				FOREACH(v, *component)cout << *v << " ";
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

void GraphTest::determineConnectedComponentsGenerateBigTest(int verticesNumber, int edgesNumber)
{
	cout << "determineConnectedComponents big test generating...\n";
	string relativePath = "determineConnectedComponents\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	testGraph.determineConnectedComponents();
	vector< set<int> > connectedComponents;
	connectedComponents = testGraph.getConnectedComponents();
	pathxx = pathx + "determineConnectedComponents\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	FOREACH(component, connectedComponents)
	{
		fprintf(saveOut, "%d ", component->size());
		FOREACH(v,*component)fprintf(saveOut, "%d ", *v);
		fprintf(saveOut, "\n");
	}
	fclose(saveOut);
}

void GraphTest::generateBigTestIn(string relativePath, int verticesNumber, int edgesNumber, bool weights, bool verts)
{
	FILE *saveIn;
	string pathxx;
	set<int> verticesx;
	vector<int> vertices;
	set< pair<int, int> > edgesx;
	srand(time(NULL));
	int v;
	if (verts==true)
	{
		pathxx = pathx + relativePath + "vertices.txt";
		saveIn = fopen(pathxx.c_str(), "w");
		for (int i = 0;i < verticesNumber;i++)
		{
			v = rand() % (3 * verticesNumber) + 1;
			while (verticesx.find(v) != verticesx.end())v = rand() % (3 * verticesNumber) + 1;
			verticesx.insert(v);
			fprintf(saveIn, "%d\n", v);
		}
		fclose(saveIn);
	}
	else
	{
		for (int i = 0;i < verticesNumber;i++)
		{
			v = rand() % (3 * verticesNumber) + 1;
			while (verticesx.find(v) != verticesx.end())v = rand() % (3 * verticesNumber) + 1;
			verticesx.insert(v);
		}
	}
	pathxx = pathx + relativePath+"edges.txt";
	saveIn = fopen(pathxx.c_str(), "w");
	FOREACH(v, verticesx)vertices.push_back(*v);
	int u;
	if (weights == false)
	{
		for (int i = 0;i < edgesNumber;i++)
		{
			v = vertices[rand() % verticesNumber];
			u = vertices[rand() % verticesNumber];
			while (edgesx.find(make_pair(u, v)) != edgesx.end())
			{
				v = vertices[rand() % verticesNumber];
				u = vertices[rand() % verticesNumber];
			}
			edgesx.insert(make_pair(u, v));
			fprintf(saveIn, "%d %d\n", u, v);
		}
	}
	else
	{
		Edge edge;
		for (int i = 0;i < edgesNumber;i++)
		{
			edge.v = vertices[rand() % verticesNumber];
			edge.u = vertices[rand() % verticesNumber];
			while (edgesx.find(make_pair(u, v)) != edgesx.end())
			{
				edge.v = vertices[rand() % verticesNumber];
				edge.u = vertices[rand() % verticesNumber];
			}
			edge.weight = double(rand() % 100000)/ 100;
			edgesx.insert(make_pair(edge.u, edge.v));
			fprintf(saveIn, "%d %d %lf\n", edge.u, edge.v, edge.weight);
		}
	}
	fclose(saveIn);
}

void GraphTest::determineConnectedComponentsTest(bool smallTests, bool bigTests)
{
	cout << "determineConnectedComponents test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "determineConnectedComponents\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			determineConnectedComponentsVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "determineConnectedComponents\\big_in_";
		loadTestIn(relativePath);
		determineConnectedComponentsVerify(false);
	}
}

void GraphTest::computeStronglyConnectedComponentsTest(bool smallTests, bool bigTests)
{
	cout << "computeStronglyConnectedComponents test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "computeStronglyConnectedComponents\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			computeStronglyConnectedComponentsVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "computeStronglyConnectedComponents\\big_in_";
		loadTestIn(relativePath);
		computeStronglyConnectedComponentsVerify(false);
	}
}

void GraphTest::computeVerticesDegreesTest(bool smallTests, bool bigTests)
{
	cout << "computeVerticesDegrees test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "computeVerticesDegrees\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			computeVerticesDegreesVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "computeVerticesDegrees\\big_in_";
		loadTestIn(relativePath);
		computeVerticesDegreesVerify(false);
	}
}

void GraphTest::computeVerticesDegreesGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "computeVerticesDegrees big test generating...\n";
	string relativePath = "computeVerticesDegrees\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	testGraph.computeVerticesDegrees();
	vector<int> verticesDegrees;
	vector<int> vertices;
	verticesDegrees = testGraph.getVerticesDegrees();
	vertices = testGraph.getVertices();
	pathxx = pathx + "computeVerticesDegrees\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	FOREACH(v, vertices)fprintf(saveOut, "%d %d\n", *v, verticesDegrees[*v]);
	fclose(saveOut);
}

void GraphTest::averageDirectedDegreeTest(bool smallTests, bool bigTests)
{
	cout << "averageDirectedDegree test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "averageDirectedDegree\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			averageDirectedDegreeVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "averageDirectedDegree\\big_in_";
		loadTestIn(relativePath);
		averageDirectedDegreeVerify(false);
	}
}

void GraphTest::averageDirectedDegreeGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "averageDirectedDegree big test generating...\n";
	string relativePath = "averageDirectedDegree\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	double averageDirectedDegree = testGraph.averageDirectedDegree();
	pathxx = pathx + "averageDirectedDegree\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	fprintf(saveOut, "%lf", averageDirectedDegree);
	fclose(saveOut);
}

void GraphTest::directedDegreeSumTest(bool smallTests, bool bigTests)
{
	cout << "directedDegreeSum test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "directedDegreeSum\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath);
			directedDegreeSumVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "directedDegreeSum\\big_in_";
		loadTestIn(relativePath);
		directedDegreeSumVerify(false);
	}
}

void GraphTest::directedDegreeSumGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "directedDegreeSum big test generating...\n";
	string relativePath = "directedDegreeSum\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	double directedDegreeSum = testGraph.directedDegreeSum();
	pathxx = pathx + "directedDegreeSum\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	fprintf(saveOut, "%lld", long long(directedDegreeSum));
	fclose(saveOut);
}

void GraphTest::edgesWeightSumTest(bool smallTests, bool bigTests)
{
	cout << "edgesWeightSum test...\n";
	string relativePath;
	if (smallTests == true)
	{
		for (int i = 1;i < 4;i++)
		{
			relativePath = "edgesWeightSum\\small" + dec2String(i) + "_in_";
			loadTestIn(relativePath,true,false);
			edgesWeightSumVerify(true, i);
		}
	}
	if (bigTests == true)
	{
		relativePath = "edgesWeightSum\\big_in_";
		loadTestIn(relativePath,true,false);
		edgesWeightSumVerify(false);
	}
}

void GraphTest::edgesWeightSumGenerateBigTest(int edgesNumber)
{
	cout << "edgesWeightSum big test generating...\n";
	string relativePath = "edgesWeightSum\\big_in_";
	generateBigTestIn(relativePath,1000,edgesNumber,true,false);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath, true, false);
	double edgesWeightSum = testGraph.edgesWeightSum();
	pathxx = pathx + "edgesWeightSum\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	fprintf(saveOut, "%lf", edgesWeightSum);
	fclose(saveOut);
}

void GraphTest::loadUsersGraphParallelTest()
{
	cout << "loadUsersGraphParallel testing...\n";
	string pathxx = pathx + "loadUsersGraphParallel\\";
	testGraph = Graph(vector<int>(),vector<Edge>(),300000,600000,filesNumber,21341);
	vector<int> V1, V2;
	vector<Edge> E1, E2;
	time_t start = time(0);
	testGraph.loadUsersGraphParallel(pathxx, omp_get_max_threads());
	double seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	V1 = testGraph.getVertices();
	E1 = testGraph.getEdges();
	start = time(0);
	testGraph.loadUsersGraphParallel(pathxx, 1);
	seconds_since_start = difftime(time(0), start);
	cout << "Serlialized execution time: " << seconds_since_start << "s\n";
	V2 = testGraph.getVertices();
	E2 = testGraph.getEdges();
	if (V1.size() == V2.size() && E1.size() == E2.size())
	{
		bool check = true;
		sort(V1.begin(), V1.end());
		sort(V2.begin(), V2.end());
		for(int i=0;i<V1.size();i++)
		{
			if(V1[i] != V2[i])
			{
				check = false;
				break;
			}
		}
		sort(E1.begin(), E1.end());
		sort(E2.begin(), E2.end());
		for (int i = 0;i < E1.size();i++)
		{
			if (E1[i].u != E2[i].u || E1[i].v != E2[i].v || E1[i].weight != E2[i].weight || E1[i].time != E2[i].time)
			{
				check = false;
				break;
			}
		}
		if (check)
		{
			cout << "OK\n";
		}
		else cout << "FAILED\n";
	}
	else
	{
		cout << "FAILED\n";
	}
}

void GraphTest::loadUsersGraphParallelGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "loadUsersGraphParallel test generating...\n";
	string relativePath = "loadUsersGraphParallel\\";
	FILE **save;
	save = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + dec2String(i);
		save[i] = fopen(pathxx.c_str(), "w");
	}
	double period = double(edgesNumber) / double(filesNumber);
	srand(time(0));
	for (int i = 0;i < edgesNumber;i++)
	{
		fprintf(save[int(double(i) / double(period))], "%d %d %d %d\n", bigRandom(verticesNumber), bigRandom(verticesNumber),
			bigRandom(1000000), 1232390000+bigRandom(250000000));
	}
	for (int i = 0;i < filesNumber;i++)fclose(save[i]);
}

void GraphTest::createTransactionsTimesParallelTest()
{
	cout << "createTransactionsTimesParallel testing...\n";
	string pathxx = pathx + "loadUsersGraphParallel\\";
	testGraph = Graph(vector<int>(), vector<Edge>(), 3000000, 6000000, filesNumber, 21341);
	testGraph.loadUsersGraphParallel(pathxx, omp_get_max_threads());
	pathxx = pathx + "createTransactionsTimesParallel\\";
	time_t start = time(0);
	testGraph.createTransactionsTimesParallel(pathxx, omp_get_max_threads());
	double seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<Edge> E1, E2;
	E1 = testGraph.getEdges();
	pathxx = pathx + "loadUsersGraphParallel\\";
	Graph testGraph2 = Graph(vector<int>(), vector<Edge>(), 3000000, 6000000, filesNumber, 21341);
	testGraph2.loadUsersGraphParallel(pathxx, omp_get_max_threads());
	pathxx = pathx + "createTransactionsTimesParallel\\";
	start = time(0);
	testGraph2.createTransactionsTimesParallel(pathxx, 1);
	seconds_since_start = difftime(time(0), start);
	cout << "Serialized execution time: " << seconds_since_start << "s\n";
	E2 = testGraph2.getEdges();
	sort(E1.begin(), E1.end());
	sort(E2.begin(), E2.end());
	bool check = true;
	for (int i = 0;i < E1.size();i++)
	{
		if (E1[i].u != E2[i].u || E1[i].v != E2[i].v || E1[i].weight != E2[i].weight || E1[i].time != E2[i].time)
		{
			check = false;
			break;
		}
	}
	if (check)
	{
		cout << "OK\n";
	}
	else cout << "FAILED\n";
}

void GraphTest::createTransactionsTimesParallelGenerateBigTest(int edgesNumber, int verticesNumber)
{
	loadUsersGraphParallelGenerateBigTest(edgesNumber, verticesNumber);
	cout << "createTransactionsTimesParallel test generating...\n";
	string relativePath = "createTransactionsTimesParallel\\";
	FILE **save;
	save = new FILE*[filesNumber];
	string pathxx = pathx + relativePath + "tx_separated";
	//mkdir(pathxx.c_str());
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "tx_separated\\"+dec2String(i);
		save[i] = fopen(pathxx.c_str(), "w");
	}
	srand(time(0));
	int transactionsNumber = 5 * verticesNumber;
	int blocksNumber = transactionsNumber / 500;
	double period = double(transactionsNumber) / double(filesNumber);
	for (int i = 0;i < transactionsNumber;i++)
	{
		fprintf(save[int(double(i) / double(period))], "%d %d %d %d\n", i, bigRandom(blocksNumber),
			0, 0);
	}
	for (int i = 0;i < filesNumber;i++)fclose(save[i]);
	ofstream saveBlocks;
	saveBlocks.open(pathx + relativePath + "bh.dat");
	for (int i = 0;i < blocksNumber;i++)saveBlocks << i << " 0 " << 1000000000 + bigRandom(500000000) << " 0\n";
	saveBlocks.close();
}

void GraphTest::createGraphSnapshotsParallelTest(string parameter, bool performSave)
{
	cout << "createGraphSnapshotsParallel testing...\n";
	string pathxx = pathx + "createGraphSnapshotsParallel\\";
	testGraph = Graph(vector<int>(), vector<Edge>(), 300000, 600000, filesNumber, 21341);
	time_t start = time(0);
	testGraph.createGraphSnapshotsParallel(pathx + "loadUsersGraphParallel\\", pathxx, 1,
		parameter, omp_get_max_threads(), performSave);
	double seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<map<pair<int, int>, long long> > snapshots1 = testGraph.getSnapshots();
	start = time(0);
	testGraph.createGraphSnapshotsParallel(pathx + "loadUsersGraphParallel\\", pathxx, 1, parameter,1,performSave);
	seconds_since_start = difftime(time(0), start);
	cout << "Sequential execution time: " << seconds_since_start << "s\n";
	vector<map<pair<int, int>, long long> > snapshots2 = testGraph.getSnapshots();
	if (snapshots1.size() == snapshots2.size())
	{
		bool check = true;
		for (int i = 0;i < snapshots1.size();i++)
		{
			if (snapshots1[i].size() == snapshots2[i].size())
			{
				FOREACH(elt, snapshots1[i])
				{
					if (snapshots2[i].find(elt->first) != snapshots2[i].end())
					{
						if ((elt->second) != snapshots2[i][elt->first])
						{
							check = false;
							break;
						}
					}
					else
					{
						check = false;
						break;
					}
				}
			}
			else check = false;
			if (check == false)break;
		}
		if (check)cout << "OK\n";
		else cout << "FAILED\n";
	}
	else cout << "FAILED\n";
}

void GraphTest::createGraphSnapshotsParallelGenerateBigTest(int edgesNumber, int verticesNumber)
{
	loadUsersGraphParallelGenerateBigTest(edgesNumber, verticesNumber);
}

void GraphTest::testAll(bool smallTests, bool bigTests)
{
	computeClusteringCoefficientsTest(smallTests, bigTests);
	determineConnectedComponentsTest(smallTests, bigTests);
	computeStronglyConnectedComponentsTest(smallTests, bigTests);
	computeVerticesDegreesTest(smallTests, bigTests);
	averageDirectedDegreeTest(smallTests, bigTests);
	directedDegreeSumTest(smallTests, bigTests);
	edgesWeightSumTest(smallTests, bigTests);
}

void GraphTest::generateBigTestAll(int edgesNumber, int verticesNumber)
{
	computeClusteringCoefficientsGenerateBigTest(edgesNumber, 2*verticesNumber);
	determineConnectedComponentsGenerateBigTest(edgesNumber/20, verticesNumber/2);
	computeStronglyConnectedComponentsGenerateBigTest(edgesNumber/10, verticesNumber/4);
	computeVerticesDegreesGenerateBigTest(edgesNumber, verticesNumber);
	averageDirectedDegreeGenerateBigTest(edgesNumber, verticesNumber);
	directedDegreeSumGenerateBigTest(edgesNumber, verticesNumber);
	edgesWeightSumGenerateBigTest(edgesNumber);
}

void GraphTest::computeStronglyConnectedComponentsGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "computeStronglyConnectedComponents big test generating...\n";
	string relativePath = "computeStronglyConnectedComponents\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	FILE *saveOut;
	string pathxx;
	loadTestIn(relativePath);
	testGraph.computeStronglyConnectedComponents();
	vector< vector<int> > connectedComponents;
	connectedComponents = testGraph.getStronglyConnectedComponents();
	pathxx = pathx + "computeStronglyConnectedComponents\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	FOREACH(component, connectedComponents)
	{
		fprintf(saveOut, "%d ", component->size());
		FOREACH(v, *component)fprintf(saveOut, "%d ", *v);
		fprintf(saveOut, "\n");
	}
	fclose(saveOut);
}

void GraphTest::computeClusteringCoefficientsGenerateBigTest(int edgesNumber, int verticesNumber)
{
	cout << "computeClusteringCoefficients big test generating...\n";
	FILE *saveOut;
	string pathxx;
	string relativePath = "computeClusteringCoefficients\\big_in_";
	generateBigTestIn(relativePath, verticesNumber, edgesNumber);
	loadTestIn(relativePath);
	testGraph.computeClusteringCoefficients();
	map<int, double> clusteringCoeffs;
	clusteringCoeffs = testGraph.getClusteringCoefficients();
	pathxx = pathx + "computeClusteringCoefficients\\big_out.txt";
	saveOut = fopen(pathxx.c_str(), "w");
	FOREACH(elt, clusteringCoeffs)
	{
		fprintf(saveOut, "%d %.3lf\n", elt->first, elt->second);
	}
	fclose(saveOut);
}
