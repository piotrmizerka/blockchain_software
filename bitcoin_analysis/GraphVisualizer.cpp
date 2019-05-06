#include "stdafx.h"

void GraphVisualizer::printVertex(int vertex)
{
	glColor3f(0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(10.0*scale);
	GLUquadricObj *disk;
	disk = gluNewQuadric();

	string number = dec2String(vertex);

	drawText(number.data(), number.size(),
		(restrictedVerticesCoordinates[vertex].first+0.01)*scale,(restrictedVerticesCoordinates[vertex].second+0.01)*scale,
		0.75, 0.0, 0.25);

	glColor3f(0.75, 0.0, 0.25);
	glTranslatef(restrictedVerticesCoordinates[vertex].first*scale, restrictedVerticesCoordinates[vertex].second*scale, 0);
	gluDisk(disk, 0.0, 0.01*scale, 100, 100);
	glLoadIdentity();
}

void GraphVisualizer::printEdge(Edge edge)
{
	glLineWidth(2.0*scale);
	glColor3f(0.0f, 0.0f, 1.0f);

	int edgeNumber = edgesNumbersVerticesPairsRestrictedGraph[make_pair(edge.u, edge.v)];
	if (edgeNumber % 2 == 0)edgeNumber *= (-1);
	edgesNumbersVerticesPairsRestrictedGraph[make_pair(edge.u, edge.v)]+=10;

	double x1 = restrictedVerticesCoordinates[edge.u].first, y1 = restrictedVerticesCoordinates[edge.u].second,
		   x2 = restrictedVerticesCoordinates[edge.v].first, y2 = restrictedVerticesCoordinates[edge.v].second;

	if (x1 != x2 || y1 != y2)
	{
		double rotationCenterX = (y2 - y1)*(0.3*double(edgeNumber) + 0.7) + 0.5*(x1 + x2),
			rotationCenterY = (x1 - x2)*(0.3*double(edgeNumber) + 0.7) + 0.5*(y1 + y2);
		float radius = sqrt((rotationCenterX - x1)*(rotationCenterX - x1) + (rotationCenterY - y1)*(rotationCenterY - y1));
		double angle1 = vectorAngle(rotationCenterX, rotationCenterY,x1,y1), angle2 = vectorAngle(rotationCenterX, rotationCenterY,x2,y2);
		double normalizedAngle1 = normalizeAngle(angle1), normalizedAngle2 = normalizeAngle(angle2);
		double arcAngle = smallerAngleDifference(normalizedAngle1, normalizedAngle2);
		double startAngle;
		if (abs(normalizeAngle(normalizedAngle1 + arcAngle) - normalizedAngle2) < EPS)startAngle = normalizedAngle1;
		else startAngle = normalizedAngle2;
		drawArc(rotationCenterX*scale, rotationCenterY*scale, radius*scale, startAngle, arcAngle, 100);

		pair<double, double> arcMiddlePointx = arcMiddlePoint(x1, y1, x2, y2, rotationCenterX, rotationCenterY);
		double rotationAngle = vectorAngle(x1, y1, x2, y2) * 180 / PI -90;
		glTranslatef(arcMiddlePointx.first*scale, arcMiddlePointx.second*scale, 0);
		glRotatef(rotationAngle, 0.0, 0.0, 1.0);

		glBegin(GL_TRIANGLES);

		glVertex2f(-0.025*scale, 0);
		glVertex2f(0.025*scale, 0);
		glVertex2f(0.0, 0.0433*scale);

		glEnd();
		glLoadIdentity();

		string number = double2String(edge.weight,0);
		drawText(number.data(), number.size(),
			(arcMiddlePointx.first + 0.01)*scale, (arcMiddlePointx.second + 0.01)*scale,
			0.25, 0.75, 0.0);
	}
	else
	{
		double radius = 0.02*double(edgeNumber/10+1)+0.06;
		GLUquadricObj *disk;
		disk = gluNewQuadric();

		glTranslatef((x1+radius)*scale, y1*scale, 0);
		gluDisk(disk, radius*scale, (radius+0.005)*scale, 100, 1);
		glLoadIdentity();

		glTranslatef((x1+2*radius)*scale, y1*scale, 0);

		glBegin(GL_TRIANGLES);

		glVertex2f(-0.025*scale, 0);
		glVertex2f(0.025*scale, 0);
		glVertex2f(0.0, 0.0433*scale);

		glEnd();
		glLoadIdentity();

		string number = double2String(edge.weight, 0);
		drawText(number.data(), number.size(),
			(x1 + 2 * radius+0.01)*scale, (y1+0.01)*scale,
			0.25, 0.75, 0.0);
	}
}

void GraphVisualizer::createRestrictedGraph()
{
	set <int> vertices2Consider;
	vector <int> restrictedVertices;
	vector <Edge> edges = graph.getEdges();
	vector <Edge> restrictedEdges;
	sortVerticesByDegree();
	for (int i = 0; i < verticesNumber2Consider; i++)
	{
		vertices2Consider.insert(sortedVertices[i].second);
		restrictedVertices.push_back(sortedVertices[i].second);
	}
	FOREACH(e, edges)
	{
		if (vertices2Consider.find(e->u) != vertices2Consider.end() &&
			vertices2Consider.find(e->v) != vertices2Consider.end())
		{
			restrictedEdges.push_back(*e);
		}
	}
	restrictedGraph = Graph(restrictedVertices, restrictedEdges);
}

void GraphVisualizer::sortVerticesByDegree()
{
	graph.computeVerticesDegrees();
	vector <int> verticesDegrees = graph.getVerticesDegrees();
	sortedVertices.clear();
	for (int i = 0; i < verticesDegrees.size(); i++)
	{
		sortedVertices.push_back(make_pair(verticesDegrees[i], i));
	}
	sort(sortedVertices.rbegin(), sortedVertices.rend());
}

void GraphVisualizer::computeRestrictedVerticesCoordinates()
{
	double sqrtx = sqrt(double(verticesNumber2Consider));
	int rowSize = int(sqrtx);
	double columnCounter = 0, rowCounter = 0;
	vector <int> vertices = restrictedGraph.getVertices();
	int maxValx = maxValue(vertices);
	restrictedVerticesCoordinates.resize(maxValx + 1);
	for (int i = 0; i < verticesNumber2Consider; i++)
	{
		restrictedVerticesCoordinates[vertices[i]].first = 0.8*(-1 + 2 * columnCounter / sqrtx);
		restrictedVerticesCoordinates[vertices[i]].second = 0.8*(-1 + 2 * rowCounter / sqrtx);
		columnCounter++;
		columnCounter = double(int(columnCounter) % rowSize);
		if (int(columnCounter) % rowSize == 0)rowCounter++;
	}
}

void GraphVisualizer::prepareEdgesNumbersVerticesPairsRestrictedGraph()
{
	vector <Edge> restrictedEdges = restrictedGraph.getEdges();
	edgesNumbersVerticesPairsRestrictedGraph.clear();
	FOREACH(e, restrictedEdges)
	{
		edgesNumbersVerticesPairsRestrictedGraph[make_pair(e->u, e->v)] = 0;
	}
}

GraphVisualizer::GraphVisualizer(Graph graphx, int number,double scalex)
{
	graph = graphx;
	verticesNumber2Consider = number;
	scale = scalex;
}
