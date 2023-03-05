#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <fstream>
#include "Graph.h"

void printMenu()
{
	std::cout << "\n**********************************************************\n";
	std::cout << "1 - get the number of vertices.\n";
	std::cout << "2 - parse (iterate) the set of vertices.\n";
	std::cout << "3 - given two vertices, find out whether there is an edge from the first one to the second one.\n";
	std::cout << "4 - get the in degree of a specified vertex.\n";
	std::cout << "5 - get the out degree of a specified vertex.\n";
	std::cout << "6 - parse the set of outbound edges of a specified vertex.\n";
	std::cout << "7 - parse the set of inbound edges of a specified vertex.\n";
	std::cout << "8 - generate a random graph.\n";
	std::cout << "9 - modify the cost of an edge.\n";
	std::cout << "10 - add an edge.\n";
	std::cout << "11 - remove an edge.\n";
	std::cout << "12 - add a vertex.\n";
	std::cout << "13 - remove a vertex.\n";
	std::cout << "14 - read the graph from a file.\n";
	std::cout << "15 - write the graph to a file.\n";
	std::cout << "16 - read the graph from a file using the second format.\n";
	std::cout << "17 - write the graph to a file using the second format.\n";
	std::cout << "18 - display the minimum cost path between two vertices and the corresponding cost.\n";
	std::cout << "0 - to exit.\n";
	std::cout << "**********************************************************\n";
}

int readIntegerNumber(std::string message) {
	std::string str;
	int flag = 0;
	while (flag == 0)
	{
		std::cout << message;
		std::cin >> str;
		flag = 1;
		for (unsigned int i = 0; i < str.length(); i++)
			if (isdigit(str[i]) == false)
				flag = 0;;
		if (flag == 0)
			std::cout << "Error reading number!\n";
	}
	return stoi(str);
}

int validCommand(int command)
{
	if (command >= 0 && command <= 18)
		return 1;
	return 0;
}

Graph buildRandomGraph(int v, int e)
{
	if (v * v < e)
	{
		std::cout << "Impossible graph.\n";
		return Graph();
	}

	std::vector<int> vertices;
	std::map< int, std::pair<int, int> > edgesindexed;
	std::map<int, int> costsindexed;
	int x, y;
	bool ok;

	for (int i = 0; i < v; i++)
	{
		vertices.push_back(i);
	}

	for (int i = 0; i < e; i++)
	{

		do {
			x = rand() % (v - 1 - 0 + 1) + 0;
			y = rand() % (v - 1 - 0 + 1) + 0;
			ok = true;
			for (int j = 0; j < i && ok == true; j++)
				if (edgesindexed[j].first == x && edgesindexed[j].second == y)
					ok = false;
		} while (ok == false);

		edgesindexed[i].first = x;
		edgesindexed[i].second = y;
		costsindexed[i] = rand() % (100 - 0 + 1) + 0;
	}
	return Graph(vertices, edgesindexed, costsindexed);
}

Graph readGraphFile(std::string filename)
{
	int n, m;
	int x, y, c;
	std::vector<int> vertices;
	std::map< int, std::pair<int, int> > edgesindexed;
	std::map<int, int> costsindexed;
	std::ifstream f;
	f.open(filename);
	if (f.is_open())
	{
		f >> n >> m;
		for (int i = 0; i < n; i++)
		{
			vertices.push_back(i);
		}

		for (int i = 0; i < m; i++)
		{
			f >> x >> y >> c;
			edgesindexed[i].first = x;
			edgesindexed[i].second = y;
			costsindexed[i] = c;
		}
		f.close();
		return Graph(vertices, edgesindexed, costsindexed);
	}

	else { std::cout << "Unable to open file\n"; return Graph(); }
}

void writeGraphFile(Graph g, std::string filename)
{
	int m=0;
	std::ofstream f;

	f.open(filename);
	if (f.is_open())
	{
		for (unsigned int i = 0; i < g.parse_vertices().size(); i++)
		{
			m = m + g.vertex_out_degree(g.parse_vertices()[i]);
		}

		f << g.numberofvertices() << " " << m << "\n";

		for (unsigned int i = 0; i < g.parse_vertices().size(); i++)
		{
			for (unsigned int j = 0; j < g.parse_nout(g.parse_vertices()[i]).size(); j++)
				f << g.parse_vertices()[i] << " " << g.parse_nout(g.parse_vertices()[i])[j] << " " << g.get_cost(g.parse_vertices()[i], g.parse_nout(g.parse_vertices()[i])[j]) << "\n";
		}
		f.close();
	}

	else { std::cout << "Unable to open file\n"; }
}

Graph readGraphFile2(std::string filename)
{
	int n, m, v;
	int x, y, c;
	std::vector<int> vertices;
	std::map< int, std::pair<int, int> > edgesindexed;
	std::map<int, int> costsindexed;
	std::ifstream f;
	f.open(filename);
	if (f.is_open())
	{
		f >> n >> m;
		for (int i = 0; i < n; i++)
		{
			f >> v;
			vertices.push_back(v);
		}

		for (int i = 0; i < m; i++)
		{
			f >> x >> y >> c;
			edgesindexed[i].first = x;
			edgesindexed[i].second = y;
			costsindexed[i] = c;
		}
		f.close();
		return Graph(vertices, edgesindexed, costsindexed);
	}

	else { std::cout << "Unable to open file\n"; return Graph(); }
}

void writeGraphFile2(Graph g, std::string filename)
{
	int m = 0;
	std::ofstream f;

	f.open(filename);
	if (f.is_open())
	{
		for (unsigned int i = 0; i < g.parse_vertices().size(); i++)
		{
			m = m + g.vertex_out_degree(g.parse_vertices()[i]);
		}

		f << g.numberofvertices() << " " << m << "\n";

		for (unsigned int i = 0; i < g.parse_vertices().size(); i++)
		{
			f << g.parse_vertices()[i] << " ";
		}

		f << "\n";

		for (unsigned int i = 0; i < g.parse_vertices().size(); i++)
		{
			for (unsigned int j = 0; j < g.parse_nout(g.parse_vertices()[i]).size(); j++)
				f << g.parse_vertices()[i] << " " << g.parse_nout(g.parse_vertices()[i])[j] << " " << g.get_cost(g.parse_vertices()[i], g.parse_nout(g.parse_vertices()[i])[j]) << "\n";
		}
		f.close();
	}

	else { std::cout << "Unable to open file\n"; }
}

void startUI()
{
	Graph g;

	while (1)
	{
		printMenu();
		int command = readIntegerNumber("Input command: ");
		while (validCommand(command) == 0)
		{
			printf("Please input a valid command!\n");
			command = readIntegerNumber("Input command: ");
		}
		if (command == 0)
			break;
		switch (command)
		{
		case 1:
		{
			std::cout << (g.numberofvertices());
			break;
		}
		case 2:
		{
			std::vector<int> lst = g.parse_vertices();
			for (unsigned int i = 0; i < lst.size(); i++)
				std::cout << lst[i] << " ";
			std::cout << "\n";
			break;
		}
		case 3:
		{
			int x, y;
			x = readIntegerNumber("Input x: ");
			y = readIntegerNumber("Input y: ");
			try {
				bool res = g.is_edge(x, y);
				if (res == true)
					printf("Yes.\n");
				else
					printf("No.\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}
		case 4:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				std::cout << g.vertex_in_degree(x) << "\n";
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}
		case 5:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				std::cout << g.vertex_out_degree(x) << "\n";
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}
		case 6:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				std::vector<int> lst = g.parse_nout(x);
				for (unsigned int i = 0; i < lst.size(); i++)
					std::cout << lst[i] << " ";
				std::cout << "\n";
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}
		case 7:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				std::vector<int> lst = g.parse_nin(x);
				for (unsigned int i = 0; i < lst.size(); i++)
					std::cout << lst[i] << " ";
				std::cout << "\n";
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}
		case 8:
		{
			int v, e;
			v = readIntegerNumber("Input number of vertices: ");
			e = readIntegerNumber("Input number of edges: ");
			g = buildRandomGraph(v, e);
			break;
		}

		case 9:
		{
			int x, y, c;
			x = readIntegerNumber("Input x: ");
			y = readIntegerNumber("Input y: ");
			c = readIntegerNumber("Input the new cost: ");
			try {
				bool res = g.modify_cost(x, y, c);
				if (res == true)
					printf("Cost modified.\n");
				else
					printf("Invalid edge.\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		case 10:
		{
			int x, y, c;
			x = readIntegerNumber("Input x: ");
			y = readIntegerNumber("Input y: ");
			c = readIntegerNumber("Input the cost: ");
			try {
				bool res = g.add_edge(x, y, c);
				if (res == true)
					printf("Edge added.\n");
				else
					printf("Edge already exists in the graph!\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		case 11:
		{
			int x, y;
			x = readIntegerNumber("Input x: ");
			y = readIntegerNumber("Input y: ");
			try {
				bool res = g.remove_edge(x, y);
				if (res == true)
					printf("Edge removed.\n");
				else
					printf("Edge already not in the graph!\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		case 12:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				g.add_vertex(x);
				printf("Vertex added.\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		case 13:
		{
			int x;
			x = readIntegerNumber("Input vertex: ");
			try {
				g.remove_vertex(x);
				printf("Vertex removed.\n");
			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		case 14:
		{
			try {
				std::string filename;
				std::cout << "Please enter the file's name: ";
				std::cin.ignore();
				std::getline(std::cin, filename);
				g = readGraphFile(filename);
			}
			catch (std::exception)
			{
				std::cout << "Error\n";
			}
			break;
		}

		case 15:
		{
			try {
				std::string filename;
				std::cout << "Please enter the file's name: ";
				std::cin.ignore();
				std::getline(std::cin, filename);
				writeGraphFile(g, filename);
			}
			catch (std::exception)
			{
				std::cout << "Error\n";
			}
			break;
		}

		case 16:
		{
			try {
				std::string filename;
				std::cout << "Please enter the file's name: ";
				std::cin.ignore();
				std::getline(std::cin, filename);
				g = readGraphFile2(filename);
			}
			catch (std::exception)
			{
				std::cout << "Error\n";
			}
			break;
		}

		case 17:
		{
			try {
				std::string filename;
				std::cout << "Please enter the file's name: ";
				std::cin.ignore();
				std::getline(std::cin, filename);
				writeGraphFile2(g, filename);
			}
			catch (std::exception)
			{
				std::cout << "Error\n";
			}
			break;
		}

		case 18:
		{
			int x, y;
			x = readIntegerNumber("Input x: ");
			y = readIntegerNumber("Input y: ");
			try {
				std::map<int, int> prev;
				int dist;
				auto p = std::make_pair(prev, dist);
				p = g.djikstra(x, y);
				std::cout << p.second << "\n";
				int i = y;
				std::vector<int> lst;
				lst.push_back(i);
				do
				{
					i = p.first[i];
					lst.push_back(i);
				} while (x != i);
				std::for_each(lst.rbegin(),
					lst.rend(),
					[](const auto& elem) {
						std::cout << elem << " ";
					});
				std::cout << "\n";

			}
			catch (const char* e) {
				std::cout << e << "\n";
			}
			break;
		}

		}
	}
}

int main()
{
	startUI();
	return 0;
}