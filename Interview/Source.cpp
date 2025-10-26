#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>
using namespace std;

mutex m;
condition_variable cv;
bool isEven = true;
void Even(int n)
{
    for (int i = 0;i <= n;i += 2)
    {
        unique_lock<mutex>ul(m);
        cv.wait(ul, [] {return isEven;});
        cout << "Even: " << i << endl;
        isEven = false;
        cv.notify_one();
    }
}
void Odd(int n)
{
    for (int i = 1;i <= n;i += 2)
    {
        unique_lock<mutex>ul(m);
        cv.wait(ul, [] {return !isEven;});
        cout << "Odd:  " << i << endl;
        isEven = true;
        cv.notify_one();
    }
}
class A {
public:
    virtual void Hello() {
        cout << "This is the parent class" << endl;
    }
    virtual~A(){
        cout << "This is the main class Destroctor" << endl;
    }
};
class B : public A {
public:
    void Hello() {
        cout << "This is the child class" << endl;
    }
    ~B(){
        cout << "This is the virtual child class Destroctor" << endl;
    }
};

class Car {
protected:
    int numberOfWheels, numberOfDoors;
    string hoursePower;
public:
    void BreaksOfCar() {
        cout << "Every CAR haing a Brakes" << endl;
    }
    void havingSheringWeels() {
        cout << "Every CAR having a Shering Weels" << endl;
    }
};
class BMW : public Car {
public:
    BMW(string hoursePower)
    {
        this->numberOfWheels = 5;
        this->numberOfDoors = 5;
        this->hoursePower = hoursePower;
    }
    void DetailsOfCar()
    {
        cout << "Number of weels in BMW is : " << this->numberOfWheels << " One wheel for stepny" << endl;
        cout << "Number of Doors in BMW is : " << this->numberOfDoors << " One Door for Decky" << endl;
        cout << "The power of BMW is : " << this->hoursePower << endl;
    }
};
class ShallowCopy {
private:
    int* ptr;
public:
    ShallowCopy(int* ptr)
    {
        this->ptr = ptr;
    }
    ShallowCopy(const ShallowCopy& cp)
    {
        this->ptr = cp.ptr;
    }
    ~ShallowCopy() {
        delete ptr;
    }
};

class DeepCopy {
private:
    int* ptr;
public:
    DeepCopy(int* ptr)
    {
        this->ptr = ptr;
    }
    DeepCopy(const DeepCopy& cp)
    {
        this->ptr = new int(*cp.ptr);
    }
    ~DeepCopy() {
        delete ptr;
    }
};

int main()
{
    int n;
    cin >> n;
    thread t(Even, n);
    thread t1(Odd, n);
    t.join();
    t1.join();
    cout << "End termination" << endl;
    auto ptr = make_unique<int>(1001);
    auto sp = make_shared<int>(1020);
    cout << *ptr << endl;
    cout << *sp << endl << sp.use_count() << endl;

    auto a = new B();
    a->Hello();
    //delete a;
    auto bmw = new BMW("5000 hp");
    bmw->DetailsOfCar();
    bmw->BreaksOfCar();
    bmw->havingSheringWeels();
    return 0;
}