#pragma once
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

class Graph
{

private:
	std::map<int, std::vector<int> > nin;
	std::map<int, std::vector<int> > nout;
	std::map<std::pair<int, int>, int> costs;

public:
	Graph();
	Graph(std::vector<int> vertices, std::map< int, std::pair<int, int>> edgesindexed, std::map<int, int> costsindexed);
	int numberofvertices();
	bool is_edge(int x, int y);
	int vertex_out_degree(int x);
	int vertex_in_degree(int x);
	std::vector<int> parse_vertices();
	std::vector<int> parse_nout(int x);
	std::vector<int> parse_nin(int x);
	bool modify_cost(int x, int y, int c);
	bool add_edge(int x, int y, int c);
	bool remove_edge(int x, int y);
	void add_vertex(int x);
	void remove_vertex(int x);
	Graph& copy_graph(const Graph& g);
	int get_cost(int x, int y);
	std::pair<std::map<int, int>, int> TSP();
	void back(std::map<int, int> &path, std::map<int, bool> &visited, int pos, int cost, int& minCost, std::map<int, int> &minPath);
};

