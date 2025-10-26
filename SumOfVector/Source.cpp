#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<memory>
#include<vector>
using namespace std;

class SumOfElementInVectorUsingMultithreading {
private:
	atomic<int>sum;
public:
	SumOfElementInVectorUsingMultithreading() :sum(0) {}
	void sumofelementinvector(vector<int>& a, int start, int end)
	{
		for (int i = start;i < end;i++)
			this->sum += a[i];
		return;
	}
	int sumofallelement() const
	{
		return this->sum;
	}
};
int main()
{
	int n;
	cout << "Enter the number of element that you want to enter in vector: " << endl;
	cin >> n;
	if (n <= 0)
	{
		cout << "Please enter the valid size of a vector...." << endl;
		cout << "Since you have enter a invalid size of a vector....\nSo we are closing this application....\nPlease enter the valid size...Thanks...." << endl;
		return 0;
	}
	vector<int>a;
	for (int i = 0;i < n;i++)
	{
		int number;
		cin >> number;
		a.emplace_back(number);
	}
	auto sumofallelementinvector = new SumOfElementInVectorUsingMultithreading();
	thread t(&SumOfElementInVectorUsingMultithreading::sumofelementinvector, sumofallelementinvector,ref(a), 0, n/3);
	thread t1(&SumOfElementInVectorUsingMultithreading::sumofelementinvector, sumofallelementinvector, ref(a), n/3, (2*n)/3);
	thread t2(&SumOfElementInVectorUsingMultithreading::sumofelementinvector, sumofallelementinvector,ref(a), (2 * n) / 3, n);
	t.join();
	t1.join();
	t2.join();
	cout << "The sum of all the element in an vector is: " << sumofallelementinvector->sumofallelement() << endl;
	delete sumofallelementinvector;
	return 0;
}