#include <iostream>
#include "Graph.h"

Graph::Graph()
{

}

Graph::Graph(std::vector<int> vertices, std::map< int, std::pair<int, int>> edgesindexed, std::map<int, int> costsindexed)
{
	int x, y, c;

	for (unsigned int i = 0; i < vertices.size(); i++) 
	{
		this->nin[i];
		this->nout[i];
	}

	for (unsigned int i = 0; i < edgesindexed.size(); i++)
	{
		x = edgesindexed[i].first;
		y = edgesindexed[i].second;
		c = costsindexed[i];
		this->nout[x].push_back(y);
		this->nin[y].push_back(x);
		this->costs[std::make_pair(x, y)] = c;
	}
}

int Graph::numberofvertices() 
{
	return this->nout.size();
}

bool Graph::is_edge(int x, int y)
{
	if (this->nout.find(x) != this->nout.end() && this->nout.find(y) != this->nout.end())
	{
		for (unsigned int i = 0; i < this->nout[x].size(); i++)
		{
			if (y == this->nout[x][i])
				return true;
		}

		return false;
	}
	else
	{
		throw "Invalid vertices";
	}
	
}

int Graph::vertex_out_degree(int x)
{
	if (this->nout.find(x) != this->nout.end())
	{
		return this->nout[x].size();
	}
	else
	{
		throw "Invalid vertex";
	}
}

int Graph::vertex_in_degree(int x)
{
	if (this->nin.find(x) != this->nin.end())
	{
		return this->nin[x].size();
	}
	else
	{
		throw "Invalid vertex";
	}
}

std::vector<int> Graph::parse_vertices()
{
	std::vector<int> lst;
	for (const auto& myPair : this->nout) {
		lst.push_back(myPair.first);
	}
	return lst;
}

std::vector<int> Graph::parse_nout(int x)
{
	if (this->nout.find(x) != this->nout.end())
	{
		std::vector<int> lst{ this->nout[x] };
		return lst;
	}
	else
	{
		throw "Invalid vertex";
	}
}

std::vector<int> Graph::parse_nin(int x)
{
	if (this->nin.find(x) != this->nin.end())
	{
		std::vector<int> lst{ this->nin[x] };
		return lst;
	}
	else
	{
		throw "Invalid vertex";
	}
}

bool Graph::modify_cost(int x, int y, int c)
{
	if (is_edge(x, y))
	{
		this->costs[std::make_pair(x, y)] = c;
		return true;
	}
	return false;
}

bool Graph::add_edge(int x, int y, int c)
{
	if (is_edge(x, y))
	{
		return false;
	}
	this->nout[x].push_back(y);
	this->nin[y].push_back(x);
	this->nout[y];
	this->costs[std::make_pair(x, y)] = c;
	return true;
}

bool Graph::remove_edge(int x, int y)
{
	if (!is_edge(x, y))
	{
		return false;
	}
	auto it = find(this->nout[x].begin(), this->nout[x].end(), y);
	if (it != this->nout[x].end())
	{
		int index = it - this->nout[x].begin();
		this->nout[x].erase(this->nout[x].begin()+index);
	}
	it = find(this->nin[y].begin(), this->nin[y].end(), x);
	if (it != this->nin[y].end())
	{
		int index = it - this->nin[y].begin();
		this->nin[y].erase(this->nin[y].begin()+index);
	}
	this->costs.erase(std::make_pair(x, y));
	return true;
}

void Graph::add_vertex(int x)
{
	if (this->nout.find(x) != this->nout.end() || this->nin.find(x) != this->nin.end())
	{
		throw "Vertex already in graph";
	}
	else
	{
		this->nin[x];
		this->nout[x];
	}
}

void Graph::remove_vertex(int x)
{
	if (this->nout.find(x) != this->nout.end() || this->nin.find(x) != this->nin.end())
	{
		for (const auto& myPair : this->nout) {
			this->costs.erase(std::make_pair(x, myPair.first));
			this->costs.erase(std::make_pair(myPair.first, x));
			auto it = find(this->nout[myPair.first].begin(), this->nout[myPair.first].end(), x);
			if (it != this->nout[myPair.first].end())
			{
				int index = it - this->nout[myPair.first].begin();
				this->nout[myPair.first].erase(this->nout[myPair.first].begin() + index);
			}
			it = find(this->nin[myPair.first].begin(), this->nin[myPair.first].end(), x);
			if (it != this->nin[myPair.first].end())
			{
				int index = it - this->nin[myPair.first].begin();
				this->nin[myPair.first].erase(this->nin[myPair.first].begin() + index);
			}
		}
		this->nout.erase(x);
		this->nin.erase(x);
	}
	else
	{
		throw "Vertex not in graph";
	}
}
Graph& Graph::copy_graph(const Graph& g)
{
	this->nout = g.nout;
	this->nin = g.nin;
	this->costs = g.costs;
	return *this;
}

int Graph::get_cost(int x, int y)
{	
	if (is_edge(x, y))
		return costs[std::make_pair(x, y)];
	else
		throw "Invalid edge";
}

std::vector<int> Graph::DAG()
{
	std::vector<int> sorted;
	std::queue<int> q;
	std::map<int, int> count;
	for (int x : parse_vertices())
	{
		count[x] = vertex_in_degree(x);
		if (count[x] == 0)
			q.push(x);
	}
	while (!q.empty())
	{
		int x = q.front();
		q.pop();
		sorted.push_back(x);
		for (int y : parse_nout(x))
		{
			count[y] = count[y] - 1;
			if (count[y] == 0)
				q.push(y);
		}
	}
	if (sorted.size() < numberofvertices())
		sorted.clear();
	return sorted;
}

std::tuple<std::map<int, int>, std::map<int, int>, std::vector<int>> Graph::earliestAndLatestStartingPoint()
{
	std::vector<int> sorted = DAG();
	if (sorted.size() == 0)
		throw "Graph is not a DAG";
	std::map<int, int> tm1;
	std::map<int, int> tm2;
	std::vector<int> critical;
	/*
	tm1[std::make_pair(sorted[0],sorted[1])] = 0;
	tm2[std::make_pair(sorted[0], sorted[1])] = get_cost(sorted[0], sorted[1]);
	for (int j = 2; j <= costs.size(); j++) // costs contains all vertices
	{
	}*/
	tm1[sorted[0]] = 0;
	for(int j=1; j<sorted.size();j++)
	{ 
		tm1[sorted[j]] = 0;
		for(int i: parse_nin(sorted[j]))
			if (tm1[i] + get_cost(i, sorted[j]) > tm1[sorted[j]])
			{
				tm1[sorted[j]] = tm1[i] + get_cost(i, sorted[j]);
			}
	}
	tm2[sorted.size()-1] = tm1[sorted.size()-1];
	for (int i = sorted.size()-2; i >= 0; i--)
	{
		tm2[sorted[i]] = INT_MAX;
		for (int j : parse_nout(sorted[i]))
			if (tm2[j] - get_cost(sorted[i], j) < tm2[sorted[i]])
				tm2[sorted[i]] = tm2[j] - get_cost(sorted[i], j);
	}
	for (int i = 0; i < sorted.size(); i++)
	{
		if (tm1[sorted[i]] == tm2[sorted[i]])
			critical.push_back(sorted[i]);
	}
	return make_tuple(tm1, tm2, critical);
}