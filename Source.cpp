#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

struct Edge
{
	std::string src_name;
	std::string dest_name;
	int src, dest, weight;
	const bool operator==(Edge comp)
	{
		return (src_name == comp.src_name && dest_name == comp.dest_name && src == comp.src &&
			dest == comp.dest && comp.weight == weight);
	}
};


class Graph
{
public:
	Graph(int vertices, std::vector<Edge> list)
	{
		edges = new std::vector<Edge>[vertices];
		this->vertices = vertices;
		for (auto var : list)
		{
			pushEdge(var);
		}
	}
	~Graph()
	{
		delete[] edges;
	}
	void pushEdge(Edge edge)
	{
		Edge inverted = invertedEdge(edge);
		edges[edge.src].push_back(edge);
		edges[edge.dest].push_back(inverted);
	}
	void printAdjList()
	{
		for (int i = 0; i < vertices; i++)
		{
			std::cout << "Node>>" << edges[i][0].src_name << std::endl;
			for (auto var : edges[i])
			{
				std::cout << "Node: " << var.dest_name << " Weight: " << var.weight << std::endl;
			}
		}
	}
	void bfs(Edge start)
	{
		std::queue<Edge> queue;
		std::unordered_map<std::string, int> visited;
		Edge current = start;
		auto has_visited = [](std::unordered_map<std::string, int>& visited, Edge& current)
		{
			auto var = visited.find(current.src_name);
			if (var != visited.cend()) //We visited this node before
				return true;
			else
				return false;
		};

		queue.push(start);
		while (!queue.empty())
		{
			current = queue.front();
			if (!has_visited(visited, current))
			{
				std::cout << "Current: " << current.src_name << std::endl;
				std::cout << "Neighbors: ";
				for (auto itr = edges[current.src].begin(); itr != edges[current.src].end(); ++itr)
				{
					Edge inverted = invertedEdge(*itr);
					std::cout << itr->dest_name << " ";
					queue.push(inverted);
				}
				std::cout << std::endl;
				visited.emplace(std::make_pair(current.src_name, current.src));
			}
			queue.pop();
		}
	}
	//Leave their dest and weight empty
	std::pair<int, std::vector<Edge>> cheapestPath(Edge start, Edge end)
	{
		int min_weight = std::numeric_limits<int>::max();
		std::vector<Edge> path;
		Edge current = start;
		bool found = false;
		int current_weight = 0;
		path.push_back(start);
		if (start.src == end.src)
		{
			return std::make_pair(current_weight, path);
		}
		while (!found)
		{
			auto check = edges[current.src];
			Edge next;
			for (auto edge : check)
			{
				if (min_weight >= edge.weight)
				{
					min_weight = edge.weight;
					next = invertedEdge(edge);
				}
				if (edge.dest == end.src)
				{
					min_weight = edge.weight;
					found = true;
				}
			}
			current = next;
			current_weight += min_weight;
			path.push_back(current);
			min_weight = std::numeric_limits<int>::max();
		}
		return std::make_pair(current_weight, path);
	}
private:
	Edge invertedEdge(Edge edge)
	{
		return { edge.dest_name,edge.src_name,edge.dest,edge.src,edge.weight };
	}
private:
	std::vector<Edge>* edges;
	int vertices;
};

int main()
{
	std::vector<Edge> edges = {
	{"Moreno Valley","Riverside", 0, 1, 16},
	{ "Moreno Valley","Perris", 0, 2, 18 },
	{ "Moreno Valley","Hemet", 0, 3, 26 },
	//{ "Riverside","Moreno Valley", 1, 0, 16 },
	{ "Riverside", "Perris", 1, 2, 24 },
	{ "Riverisde","Hemet", 1, 3, 33 },
	//{ "Perris","Moreno Valley", 2, 0, 18 },
	//{ "Perris","Riverside", 2, 1, 24 },
	{ "Perris","Hemet", 2, 3, 30 },
	//{ "Hemet","Moreno Valley", 3, 0, 26 },
	//{ "Hemet","Riverside", 3, 1, 33 },
	//{ "Hemet","Perris",3, 2, 30 } };
	};
	Graph graph(4, edges);

	graph.printAdjList();
	graph.bfs({ "Moreno Valley","Riverside", 0, 1, 16 });

	auto pair = graph.cheapestPath({ "Moreno Valley","Riverside", 0, 1, 16 }, { "Riverside","Moreno Valley", 1, 0, 16 });
	std::cout << "Weight: " << pair.first << std::endl;
	std::cout << "Path:";
	for (auto p : pair.second)
	{
		std::cout << " " << p.src_name << "(" << p.weight << ")" << " ->";
	}
	std::cout << std::endl;
	return 0;
}