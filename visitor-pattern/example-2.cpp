#include <memory>
#include <list>
#include <functional>

struct IElement
{
	virtual const std::string& name() const = 0;

	virtual void visit(std::function<void(IElement*)> visitor) = 0;
};

struct Element : public IElement
{
	Element(const std::string& name) : name_(name) {}

	void visit(std::function<void(IElement*)> visitor) override { visitor(this); }

	const std::string& name() const override { return name_; }

private:
	std::string name_;
};

struct ElementList : public IElement
{
	ElementList(const std::string& name) : name_(name) {}

	void visit(std::function<void(IElement*)> visitor) override
	{
		visitor(this);
		for (auto element : elements_) element->visit(visitor);
	}

	void add(IElement* element) { elements_.push_back(element); }

	const std::string& name() const override { return name_; }

private:
	std::string name_;
	std::list<IElement*> elements_;
};

int main()
{
	auto root = new ElementList("root");
	auto elements1 = new ElementList("element list 1");
	auto elements2 = new ElementList("element list 2");

	elements1->add(new Element("element 1-1"));
	elements1->add(new Element("element 1-2"));
	elements2->add(new Element("element 2-1"));
	elements2->add(new Element("element 2-2"));

	root->add(elements1);
	root->add(elements2);
	root->add(new Element("element 3"));

	root->visit([](auto&& element) {
		printf("%s\n", element->name().c_str());
	});

	return 0;
}
