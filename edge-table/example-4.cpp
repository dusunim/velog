#include <iostream>
#include <tuple>
#include <vector>
#include <functional>

struct Face
{
	int vertices[3];

	int& operator[](size_t n) { return vertices[n]; }
	int operator[](size_t n) const { return vertices[n]; }
};

struct Edge
{
	int vertices[2];

	auto& lo() const noexcept { return vertices[vertices[0] >= vertices[1]]; }
	auto& hi() const noexcept { return vertices[vertices[0] < vertices[1]]; }
};

class EdgeTable
{
	struct Node
	{
		int hi = -1;
		int next = -1;
	};
	
	std::vector<int> nexts_;
	std::vector<Node> nodes_;

public:
	void insert(const Edge& e)
	{
		if (nexts_.size() <= size_t(e.lo()))
		{
			nexts_.resize(e.lo() + 1, -1);
		}

		for (auto next = nexts_[e.lo()]; next > -1; next = nodes_[next].next)
		{
			if (nodes_[next].hi == e.hi())
			{
				return;
			}
		}

		nodes_.push_back(Node{ e.hi(), nexts_[e.lo()] });
		nexts_[e.lo()] = int(nodes_.size() - 1);
	}

	void for_each(std::function<void(const Edge&)> visitor)
	{
		for (int lo = 0; lo != nexts_.size(); ++lo)
		{
			auto next = nexts_[lo];
			while (next > -1)
			{
				visitor(Edge{ lo, nodes_[next].hi });
				next = nodes_[next].next;
			}
		}
	}
};

int main()
{
	std::vector<Face> faces = {{ 4, 3, 2 }, { 1, 6, 0 }, { 3, 5, 6 }, { 4, 5, 3 }};
	EdgeTable edges;

	for (auto& face : faces)
	{
		edges.insert({ face[0], face[1] });
		edges.insert({ face[1], face[2] });
		edges.insert({ face[2], face[0] });
	}

	edges.for_each([](auto&& e) { printf("%d, %d\n", e.lo(), e.hi()); });

	return 0;
}
