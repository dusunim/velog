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
		Node* next = nullptr;
	};
	
	std::vector<Node*> nexts_;

public:
	~EdgeTable()
	{
		for (int lo = 0; lo != nexts_.size(); ++lo)
		{
			for (auto next = nexts_[lo]; next;)
			{
				auto current = next;
				next = next->next;
				delete current;
			}
		}
	}

	void insert(const Edge& e)
	{
		if (nexts_.size() <= size_t(e.lo()))
		{
			nexts_.resize(e.lo() + 1, nullptr);
		}

		for (auto next = nexts_[e.lo()]; next; next = next->next)
		{
			if (next->hi == e.hi())
			{
				return;
			}
		}

		nexts_[e.lo()] = new Node{ e.hi(), nexts_[e.lo()] };
	}

	void for_each(std::function<void(const Edge&)> visitor)
	{
		for (int lo = 0; lo != nexts_.size(); ++lo)
		{
			for (auto next = nexts_[lo]; next; next = next->next)
			{
				visitor(Edge{ lo, next->hi });
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
