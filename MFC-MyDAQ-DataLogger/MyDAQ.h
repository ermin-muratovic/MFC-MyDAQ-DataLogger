#pragma once
#include <NIDAQmx.h>
#include <string>
#include <list>

using namespace std;

struct MyDAQSample {
	float64 value;
	string time;
};
struct MyDAQChannel {
	string name;
	list<MyDAQSample> samples;
};

class MyDAQ
{
private:
	float minV;
	float maxV;
	int samplingRate;
	int sampsToRead;
	bool reading, updating;
	string myDAQname;
	TaskHandle handle;
	list<MyDAQChannel> channels;

public:
	MyDAQ();
	~MyDAQ();

	int getSamplingRate();
	void setSamplingRate(int sr);
	float getMinV();
	void setMinV(float mv);
	float getMaxV();
	void setMaxV(float mv);
	string getMyDAQName();

	list<string> getConnectedDevices();
	list<string> splitString(string value, char separator, int spacing);
	int readChannels();
	int setChannels(CString myDAQ, list<CString> c);
	list<MyDAQChannel> getChannels();
	int getReadSamps();
};

