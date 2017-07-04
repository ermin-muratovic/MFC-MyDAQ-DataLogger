#pragma once
#include <NIDAQmx.h>
#include <string>
#include <list>

using namespace std;

struct MyDAQChannel {
	string name;
	float64* samples;
	CTime start;
	CTime end;
};

class MyDAQ
{
private:
	float minV;
	float maxV;
	int samplingRate;
	bool reading, updating;
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

	list<string> getConnectedDevices();
	list<string> splitString(string value, char separator, int spacing);
	int readChannels();
	int setChannels(CString myDAQ, list<CString> c);
	list<MyDAQChannel> getChannels();
};

