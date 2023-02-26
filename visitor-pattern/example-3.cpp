#include <memory>
#include <list>
#include <functional>

struct IElement
{
	virtual const std::string& name() const = 0;

	virtual void visit(std::function<void(std::shared_ptr<IElement>)> visitor) = 0;
};

class Element : public IElement,
	public std::enable_shared_from_this<Element>
{
	Element(const std::string& name) : name_(name) {}

public:
	static auto create(const std::string& name)
	{
		return std::shared_ptr<Element>(new Element(name));
	}

	void visit(std::function<void(std::shared_ptr<IElement>)> visitor) override
	{
		visitor(shared_from_this());
	}

	const std::string& name() const override { return name_; }

private:
	std::string name_;
};

class ElementList : public IElement,
	public std::enable_shared_from_this<ElementList>
{
	ElementList(const std::string& name) : name_(name) {}

public:
	static auto create(const std::string& name)
	{
		return std::shared_ptr<ElementList>(new ElementList(name));
	}

	void visit(std::function<void(std::shared_ptr<IElement>)> visitor) override
	{
		visitor(shared_from_this());
		for (auto element : elements_) element->visit(visitor);
	}

	void add(std::shared_ptr<IElement> element)
	{
		elements_.push_back(element);
	}

	const std::string& name() const override { return name_; }

private:
	std::string name_;
	std::list<std::shared_ptr<IElement>> elements_;
};

int main()
{
	auto root = ElementList::create("root");
	auto elements1 = ElementList::create("element list 1");
	auto elements2 = ElementList::create("element list 2");

	elements1->add(Element::create("element 1-1"));
	elements1->add(Element::create("element 1-2"));
	elements2->add(Element::create("element 2-1"));
	elements2->add(Element::create("element 2-2"));

	root->add(elements1);
	root->add(elements2);
	root->add(Element::create("element 3"));

	root->visit([](auto&& element) {
		printf("%s\n", element->name().c_str());
	});

	return 0;
}
