#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<future>
#include<vector>
#include<filesystem>
#include<direct.h.>
#include<fstream>

using namespace std;
// For this we are restrict the number of async task to run only 100 at a time
class Semaphore {
private:
	mutex mtx;
	condition_variable cv;
	int count;
public:
	explicit Semaphore(int intial_count) : count(intial_count) {}  // here we are using explicit so that we can restric call like Semaphore obj = 1500; from this only way to inlization the count is to use Semaphore sem(100);
	void acquire()
	{
		unique_lock<mutex>ul(mtx);
		cv.wait(ul, [this] () {return count > 0;});
		--count;
	}
	void release()
	{
		lock_guard<mutex>lg(mtx);
		++count;
		cv.notify_one();
	}
};
class FactorialAndPrime {
public:
	long long int factorial(int number)
	{
		long long int mul = 1;
		for (int i = 2;i <= number;i++)
			mul *= i;
		return mul;
	}
	long long int countprime(int end_number) 
	{
		bool flag = true;
		if (end_number <= 1)
			return -1;
		for (int j = 2;j * j <= end_number;j++)
		{
			if (end_number % j == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
			return end_number;
		return -1;
	}
};
int main()
{
	int n;
	cout << "Enter the number whose factorial you want to find:  ";
	cin >> n;
	auto factorialandprime = new FactorialAndPrime();
	vector<future<long long int>>fq;
	Semaphore sem(100); // with this line only 100 concurrent call for async 
	//fq.emplace_back(async(launch::async, &FactorialAndPrime::factorial, factorialandprime,n));
	//fq.emplace_back(async(launch::async, &FactorialAndPrime::factorial, factorialandprime,n+5));
	//fq.emplace_back(async(launch::async, &FactorialAndPrime::factorial, factorialandprime,n+8));
	fq.emplace_back(async(launch::async, [&factorialandprime, &sem, n]() {
		auto result = factorialandprime->factorial(n);
		sem.release();
		return result;
		}));
	fq.emplace_back(async(launch::async, [&factorialandprime, &sem, n]() {
		auto result = factorialandprime->factorial(n+5);
		sem.release();
		return result;
		}));
	fq.emplace_back(async(launch::async, [&factorialandprime, &sem, n]() {
		auto result = factorialandprime->factorial(n+10);
		sem.release();
		return result;
		}));
	int start_number, end_number;
	cout << "Enter the starting range for find the prime number: ";
	cin >> start_number;
	cout << "Enter the end range for find the prime number: ";
	cin >> end_number;
	for (int i = start_number; i <= end_number;i++)
	{
		sem.acquire();
		fq.emplace_back(async(launch::async, [&factorialandprime, &sem, i]() {
			auto result = factorialandprime->countprime(i);
			sem.release();
			return result;
			}));
	}
	const char* path = "G:\\PrimeAndFactorialAsyncAndFuture\\";
	_mkdir(path);
	string file_path = (string)path + "PrimeAndFactorialAsyncAndFuture.txt";
	ofstream file(file_path, ios::app);
	file << "Factorial of the numbers " << endl;
	int count = 0;
	for(auto &f:fq)
	{
		long long int value = f.get();
		if (value != -1)
		{
			count++;
			if(count == 4 )
				file << "Prime number in the range of 100 and 100000" << endl;
			file << value << endl;
		}
	}
	delete factorialandprime;
	return 0;
}