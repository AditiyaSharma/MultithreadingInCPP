#include<iostream>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<chrono>

using namespace std;

class TrafficLight {
private:
	mutex mtx;
	condition_variable cv;
	string traffic_color;
	void CountDown(string const& trafficLightColor, int time) const
	{
		if (traffic_color == "Orange" || traffic_color == "OrangeAfterGreen")
		{
			for (int i = time-1;i > 0;i--)
			{
				cout << "Orange" << " : " << i << endl;
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
		else
		{
			for (int i = time;i >= 5;i--)
			{
				cout << trafficLightColor << " : " << i << endl;
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
	}
public:
	explicit TrafficLight(string const& trafficLight): traffic_color(trafficLight){}

	void RedTraficLight()
	{
		while (true)
		{
			unique_lock<mutex>ul(mtx);
			cv.wait(ul, [this]() {return this->traffic_color == "Red";});
			this->CountDown(this->traffic_color, 55);
			this->traffic_color = "Orange";
			cv.notify_all();
		}
	}
	void OrangeTraficLight()
	{
		unique_lock<mutex>ul(mtx);
		cv.wait(ul, [this] () {return this->traffic_color == "Orange";});
		this->CountDown(this->traffic_color, 5);
		this->traffic_color = "Green";
		cv.notify_all();
	}
	void GreenTraficLight()
	{
		unique_lock<mutex>ul(mtx);
		cv.wait(ul,[this]() {return this->traffic_color == "Green";});
		this->CountDown(this->traffic_color, 55);
		this->traffic_color = "OrangeAfterGreen";
		cv.notify_all();
	}
	void OrangeAfterGreenLight()
	{
		unique_lock<mutex>ul(mtx);
		cv.wait(ul, [this]() {return this->traffic_color == "OrangeAfterGreen";});
		this->CountDown(this->traffic_color, 5);
		this->traffic_color = "Red";
		cv.notify_all();
	}

};
int main()
{
	auto trafficlight = new TrafficLight("Red");
	thread t(&TrafficLight::RedTraficLight, trafficlight);
	thread t1(&TrafficLight::OrangeTraficLight, trafficlight);
	thread t2(&TrafficLight::GreenTraficLight, trafficlight);
	thread t3(&TrafficLight::OrangeAfterGreenLight, trafficlight);

	t.join();
	t1.join();
	t2.join();
	t3.join();
	cout << "Done the traffic Light code" << endl;
	delete trafficlight;
	return 0;
}