#include<iostream>
using namespace std;

class Constructor_class {
private:
	int* p;

public:
	// This is the default constructor
	Constructor_class() 
	{
		this->p = NULL;
		cout << "This is the Default Constructor" << endl;
	}
	// This is the parametric constructor.
	Constructor_class(int* p) : p(p)
	{
		cout << "This is th Parametric Constructor" << endl;
	}

	//This is the copy Constructor
	Constructor_class(const Constructor_class& other)
	{
		this->p = new int(*other.p);
		cout << "This is the Copy Constructor" << endl;
	}

	// This is the move constructor

	Constructor_class(Constructor_class&& other)
	{
		p = other.p;
		other.p = NULL;
		cout << "This is the move constructor" << endl;
	}
	// This is the destructor
	~Constructor_class() {
		delete p;
		cout << "This is the default destructor" << endl;
	}

};
int main()
{
	int* p = new int(10);
	Constructor_class* C1 = new Constructor_class();
	Constructor_class* C2 = new Constructor_class(p);
	Constructor_class C3(*C2);
	Constructor_class* C4 = new Constructor_class(std::move(*C2));
	return 0;
}