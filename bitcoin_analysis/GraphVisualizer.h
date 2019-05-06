#pragma once

class GraphVisualizer
{
	/// parameters
	Graph graph;
	int verticesNumber2Consider;
	double scale;

	/// inner variables
	Graph restrictedGraph;
	vector < pair<int, int> > sortedVertices;
	vector < pair<double, double> > restrictedVerticesCoordinates;
	map < pair<int, int>, int > edgesNumbersVerticesPairsRestrictedGraph;
public:
	/// inner methods
	// restricted graph
	void createRestrictedGraph();
	// sorting vertices
	void sortVerticesByDegree();
	// resstricted vertices coordinates
	void computeRestrictedVerticesCoordinates();
	// edges numbers vertices pairs restricted graph
	void prepareEdgesNumbersVerticesPairsRestrictedGraph();
	// printing methods
	void printVertex(int vertex);
	void printEdge(Edge edge);

	/// constructor
	GraphVisualizer(Graph graphx = Graph(), int number = 0, double scalex=1);
};