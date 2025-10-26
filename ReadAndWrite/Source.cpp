#include<iostream>
#include<thread>
#include<mutex>
#include<shared_mutex>
#include<condition_variable>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

class ReadAndWrire {
private:
	shared_mutex sm;
public:
	void Readfiles(const string &path)
	{
		shared_lock<shared_mutex> sl(sm);
		const char* pathOffile = path.c_str();
		ifstream fp(pathOffile);
		if (!fp)
		{
			cout << "There is no file present in given location i.e " << path << endl;
			return;
		}
		string line;
		cout << "Reading File Start......" << endl;
		while (getline(fp, line));
		cout << "Reading File complete....." <<  endl;
		fp.close();
		return;
	}
	void WriteInFile(const string &path) {
		unique_lock<shared_mutex> ul(sm);
		const char* pathOfFile = path.c_str();
		cout << "Writting File Started...." << endl;
		ofstream fp(pathOfFile);
		fp << "Shubham Kuamr this side and i am trying multithreading to read data from multiple thread and write data in single thread" << endl;
		fp << "Please tell me is it working or not??" << endl;
		fp.close();
		cout << "Writting File Complete...." << endl;
		return;
	}
};
int main()
{
	string path;
	cout << "Please enter the path from where we need to read that path...." << endl;
	cin >> path;
	auto readAndWrite = new ReadAndWrire();
	vector<thread>threads;
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::WriteInFile, readAndWrite,path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	threads.emplace_back(&ReadAndWrire::Readfiles, readAndWrite, path);
	for (auto& t : threads)
		t.join();
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "Now it is been closed please check if it is work sucessfully or not...." << endl;
	delete readAndWrite;
	return 0;
}