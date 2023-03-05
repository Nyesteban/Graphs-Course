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

struct secondValueComp {
	constexpr bool operator()(
		std::pair<int, int> const& a,
		std::pair<int, int> const& b)
		const noexcept
	{
		return a.second > b.second;
	}
};

std::pair<std::map<int, int>, int> Graph::djikstra(int s, int t)
{
	if (this->nout.find(s) != this->nout.end() && this->nout.find(t) != this->nout.end()) // check if the two given vertices exist
	{
		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, secondValueComp> q; // create a min heap priority queue, 
		// sorted by the second element of the pair, which is the priority 
		std::map<int, int> prev; // a map that associates, to each accessible vertex, the cost of the minimum cost walk from s to it
		std::map<int, int> dist; // a map that maps each accessible vertex to its predecessor on a path from s to it
		int x;
		q.push(std::make_pair(s, 0));
		dist[s] = 0;
		bool found = false;
		while (!q.empty() && !found)
		{
			x = q.top().first; // gets the element with minimum value of priority
			q.pop(); // then dequeues it
			for (int y : this->parse_nout(x))
			{
				if (dist.find(y) == dist.end() || dist[x] + this->get_cost(x, y) < dist[y])
				{
					dist[y] = dist[x] + this->get_cost(x, y);
					q.push(std::make_pair(y, dist[y]));
					prev[y] = x;
				}
			}
			if (x == t)
			{
				found = true;
			}
		}
		if (found == true) // check if there is a path between the two vertices
			return std::make_pair(prev, dist[t]); // returns prev and the the cost of the minimum cost walk from s to t
		else
			throw "No path found";
	}
	else
	{
		throw "Invalid vertices";
	}
}