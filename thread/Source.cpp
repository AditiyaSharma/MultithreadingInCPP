#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<vector>
#include<memory>
#include<future>
using namespace std;
class A {
private:
	queue<int>q;
	mutex m;
public:
	void PushData(int number)
	{
		lock_guard<mutex>lg(m);
		q.push(number);
	}
	void PopData() {
		lock_guard<mutex> lg(m);
		if (!q.empty())
		{
			cout << "The pop element is " << q.front() << endl;
			q.pop();
		}
		else
			cout << "Insert some data \nQueue is empty" << endl;
		return;
	}
	void FrontData() {
		lock_guard<mutex>lg(m);
		if (!q.empty())
			cout << "The pop element is " << q.front() << endl;
		else
			cout << "Insert some data \nQueue is empty" << endl;
	}
	void DisplayData() {
		lock_guard<mutex>lg(m);
		cout << "The size of queue is " << q.size() << endl;
		queue<int> temp(q); // this is the deep copy
		while (!temp.empty())
		{
			cout << "The element in Queue is: " << temp.front() << endl;
			temp.pop();
		}
	}
	int SizeOfQueue()
	{
		lock_guard<mutex>lg(m);
		cout << "The size of queue is " << q.size() << endl;
		queue<int>temp(q);  // this is the deep copy
		int sizeOfQueue = 0;
		while (!temp.empty())
		{
			sizeOfQueue++;
			temp.pop();
		}
		return sizeOfQueue;
	}
	
};
int main()
{
	vector<thread>threads;
	auto a = make_unique<A>();
	for (int i = 0;i <= 10;i++)
	{
		threads.emplace_back(&A::PushData, a.get(), i + 5);
		threads.emplace_back(&A::PushData, a.get(), i + 100);
	}
	for (auto& t : threads)
		t.join();
	a->DisplayData();
	cout << "Before Deletion" << endl;
	thread t(&A::PopData, a.get());
	cout << "After Deleteion" << endl;
	t.join();
	a->DisplayData();
	thread t1(&A::FrontData, a.get());
	t1.join();
	future<int> f = async(&A::SizeOfQueue, a.get());
	//t.join();
	cout << f.get() << endl;
	return 0;
}