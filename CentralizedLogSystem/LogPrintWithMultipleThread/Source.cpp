#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<string>
#include<direct.h>
#include<fstream>
#include<shared_mutex>
#include<vector>
#include<queue>
#include <filesystem>

using namespace std;

class CentralizedLoggingSystem {
private:
	mutex m;
	condition_variable cv;
	ofstream fp;
	shared_mutex sm;
	queue<string>logQueue;
	bool isDone = false;
	thread worker;
	void dataInsertinQueue()
	{
		while ( true)
		{
			unique_lock<mutex>ul(m);
			cv.wait(ul, [this] {return !logQueue.empty() || this->isDone;});
			if (this->isDone && logQueue.empty())
				break;
			std::queue<std::string> localQueue;
			std::swap(localQueue, logQueue);
			ul.unlock();
			while (!localQueue.empty())
			{
				fp << localQueue.front() << endl;
				localQueue.pop();
			}
		}
	}
public:
	CentralizedLoggingSystem()
	{
		const char* path = "G:\\CentrilizedLog\\";
		_mkdir(path);
		//std::filesystem::create_directories("G:\\CentrilizedLog");  // this is been using when you have a c++ version >= c++17
		string logfilepath = path;
		logfilepath += "Logs.txt";
		fp.open(logfilepath, ios::app);
		if (!fp.is_open())
			cout << "Unable to open a file.....\nPlease try again after some time....\n" << endl;
		worker = thread(&CentralizedLoggingSystem::dataInsertinQueue, this);
	}
	void WriteinQueue(string message)
	{
		lock_guard<mutex> lg(m);
		logQueue.push(message);
		cv.notify_one();
	}
	void ReadFile(const char* path) {
		WriteinQueue("File Open for Reading...");
		WriteinQueue("Path is: " + string(path) );
		shared_lock<shared_mutex> sl(sm);
		ifstream rf(path);
		string line;
		while (getline(rf, line));
		rf.close();
		WriteinQueue("File Reading is complete...");
	}
	void WriteFile(const char* path)
	{
		WriteinQueue("File Open for Writting...");
		WriteinQueue("Path is: " + string(path) );
		unique_lock<shared_mutex> ul(sm);
		ofstream wf(path);
		wf << "Witting is been in processing" << endl;
		wf << "Thanks for watting now the writting processing is been done....." << endl;
		wf.close();
		WriteinQueue("Writting is been done...");
	}
	~CentralizedLoggingSystem() {
		cout << "Destorctor is called" << endl;
		isDone = true;
		cv.notify_one();
		worker.join();
		try {
			fp.close();
		}
		catch (...) {
			cout << "Fail to insert data in log file" << endl;
		}
	}
};
int main()
{
	vector<thread>threads;
	const char* path = "G:\\hello.txt";
	auto centralizedLoggingSystem = new CentralizedLoggingSystem();
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::WriteFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	threads.emplace_back(&CentralizedLoggingSystem::ReadFile, centralizedLoggingSystem, path);
	for (auto& t : threads)
		t.join();
	cout << "The process is been done" << endl;
	delete centralizedLoggingSystem;
	return 0;
}