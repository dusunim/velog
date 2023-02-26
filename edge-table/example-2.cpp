#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_set>

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
	auto oriented() const noexcept { return std::tie(lo(), hi()); }
	bool operator<(const Edge& rhs) const noexcept { return oriented() < rhs.oriented(); }
	bool operator==(const Edge& rhs) const noexcept { return oriented() == rhs.oriented(); }
};

template <> struct std::hash<Edge>
{
	size_t operator()(const Edge& edge) const noexcept
	{
		return edge.lo();
	}
};

int main()
{
	std::vector<Face> faces = {{ 4, 3, 2 }, { 1, 6, 0 }, { 3, 5, 6 }, { 4, 5, 3 }};
	std::unordered_set<Edge> edges;

	for (auto& face : faces)
	{
		edges.insert({ face[0], face[1] });
		edges.insert({ face[1], face[2] });
		edges.insert({ face[2], face[0] });
	}

	for (auto& edge : edges)
	{
		printf("%d, %d\n", edge.lo(), edge.hi());
	}

	return 0;
}
