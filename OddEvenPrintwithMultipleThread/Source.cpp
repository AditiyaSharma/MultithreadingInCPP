#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<memory>
using namespace std;

// In this we create a class and over there we have to different method one is for print odd and another is for print even for the same.
// We use class over method i have not create a method directly, just create a class for the same.
class OddAndEven {
private:
	mutex m;
	condition_variable cv;
	bool isEven;
public:
	OddAndEven(bool isEven)
	{
		this->isEven = isEven;
	}
	void printEven(int number)
	{
		for (int i = 2;i <= number;i += 2)
		{
			unique_lock<mutex> ul(m);
			cv.wait(ul, [this] {return this->isEven;});
			cout << "Even: " << i << endl;
			isEven = false;
			cv.notify_one();
		}
	}
	void printOdd(int number)
	{
		for (int i = 1;i <= number;i += 2)
		{
			unique_lock<mutex> ul(m);
			cv.wait(ul, [this] {return !this->isEven;});
			cout << "Odd : " << i << endl;
			isEven = true;
			cv.notify_one();
		}
	}
};
int main() {
	int number;
	cout << "Enter the number: " << endl;
	cin >> number;
	auto oddandeven = new OddAndEven(false);
	thread t(&OddAndEven::printEven, oddandeven, number); // this is the way to crate a thread for a class method
	thread t1(&OddAndEven::printOdd, oddandeven, number);
	t.join();
	t1.join();
	cout << "Done the printing the odd and even with the help of threading" << endl;
	return 0;
}
