#include<iostream>
using namespace std;
class SingleTonClass {
private:
	SingleTonClass() {
		cout << "This is the SingleTonClass Constructor" << endl;
	}
public:
	SingleTonClass(const SingleTonClass&) = delete;
	SingleTonClass& operator = (const SingleTonClass&) = delete;
	static SingleTonClass* getInstance()
	{
		static SingleTonClass sc;
		return &sc;
	}
	void getMessage()
	{
		cout << "This is the method for priting" << endl;
	}
};

int main()
{
	SingleTonClass* sc = SingleTonClass::getInstance();
	SingleTonClass* sc1 = SingleTonClass::getInstance();
	sc->getMessage();
	sc1->getMessage();

	if (sc == sc1)
	{
		cout << "Same Instance " << endl;
	}
	else
		cout << "Different instance" << endl;
	return 0;
}