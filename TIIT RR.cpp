#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

typedef vector<vector<pair<int, int>>> Graph;


Graph readTestFile(string nameOfFile)
{
	fstream test(nameOfFile);

	Graph graph;
	vector<pair<int, int>> edges;

	string line;
	for (int i = 1; getline(test, line); ++i)
	{
		stringstream ss(line);

		int val;
		for (int j = 0; ss >> val; ++j)
		{
			if (val == 0) continue;

			if (j >= edges.size())
				edges.resize(j + 1, pair<int, int>(-1, -1));

			if (edges[j] == pair<int, int>(-1, -1))
			{
				edges[j] = pair<int, int>(i, val);
			}
			else
			{
				int from = i;
				int to = edges[j].first;
				int weight = edges[j].second;

				if (graph.size() <= max(from, to))
					graph.resize(max(from, to) + 1);

				graph[from].push_back(pair<int, int>(to, weight));
				graph[to].push_back(pair<int, int>(from, weight));
			}
		}
	}

	return graph;
}

void formWeightLine(const Graph& graph, int vertex, unordered_set<int>& checked, vector<int>& line)
{
	for (int i = 0; i < graph[vertex].size(); ++i)
	{
		int from = vertex;
		int to = graph[vertex][i].first;
		int weight = graph[vertex][i].second;

		line[to] = min(line[to], line[from] + weight);
	}

	checked.insert(vertex);

	int target = 0;
	for (int i = 1; i < line.size(); ++i)
	{
		if (checked.count(i)) continue;

		target = (line[i] < line[target] ? i : target);
	}

	if (target != 0)
		formWeightLine(graph, target, checked, line);
}

void findPeripheralVertexes(string nameOfFile)
{
	Graph graph = readTestFile(nameOfFile);

	vector<vector<int>> weightMatrix;
	for (int i = 1; i < graph.size(); ++i)
	{
		vector<int> line(graph.size(), INT_MAX);
		line[i] = 0;

		unordered_set<int> checked;
		checked.insert(i);

		formWeightLine(graph, i, checked, line);

		weightMatrix.push_back(line);
	}

	int d = -1;
	for (int i = 0; i < weightMatrix.size(); ++i)
		for (int j = 1; j < weightMatrix[i].size(); ++j)
			d = max(d, weightMatrix[i][j]);


	vector<int> result;
	for (int i = 0; i < weightMatrix.size(); ++i)
		for (int j = 1; j < weightMatrix[i].size(); ++j)
			if (weightMatrix[i][j] == d && find(result.begin(), result.end(), j) == result.end())
				result.push_back(j);

	sort(result.begin(), result.end());

	cout << "Периферийные вершины для графа " << nameOfFile << ":\n";
	for (int i = 0; i < result.size(); ++i)
		cout << result[i] << " ";

	cout << "\n\n";
}

int main()
{
	setlocale(LC_ALL, "Russian");

	findPeripheralVertexes("Test1.txt");
	findPeripheralVertexes("Test2.txt");
	findPeripheralVertexes("Test3.txt");
	findPeripheralVertexes("Test4.txt");
	findPeripheralVertexes("Test5.txt");

	return 0;
}