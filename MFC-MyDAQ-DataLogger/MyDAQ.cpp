#include "stdafx.h"
#include "MyDAQ.h"
#include <NIDAQmx.h>
#include <string>
#include <list>

using namespace std;

#define MINSAMPLINGRATE 5
#define MAXSAMPLINGRATE 10000
#define MINV -10
#define MAXV 10

MyDAQ::MyDAQ()
{
	minV = -10;
	maxV = 10;
	samplingRate = 1000;
	reading = false;
}


MyDAQ::~MyDAQ()
{
}

int MyDAQ::getSamplingRate()
{
	return samplingRate;
}

void MyDAQ::setSamplingRate(int sr)
{
	if (sr >= MINSAMPLINGRATE && sr <= MAXSAMPLINGRATE)
		samplingRate = sr;
	else if (sr < MINSAMPLINGRATE)
		samplingRate = MINSAMPLINGRATE;
	else if (sr > MAXSAMPLINGRATE)
		samplingRate = MAXSAMPLINGRATE;
}

float MyDAQ::getMinV()
{
	return minV;
}

void MyDAQ::setMinV(float mv)
{
	if (mv >= MINV && mv <= MAXV)
		minV = mv;
	else if (mv < MINV)
		minV = MINV;
	else if (mv > MAXV)
		minV = MAXV;
}

float MyDAQ::getMaxV()
{
	return maxV;
}

void MyDAQ::setMaxV(float mv)
{
	if (mv >= MINV && mv <= MAXV)
		maxV = mv;
	else if (mv < MINV)
		maxV = MINV;
	else if (mv > MAXV)
		maxV = MAXV;
}


list<string> MyDAQ::getConnectedDevices() 
{
	char namesBuffer[1000] = { '\0' };
	DAQmxGetSysDevNames(namesBuffer, 1000);
	string names(namesBuffer);
	list<string> devices = splitString(names, ',', 2);
	return devices;
}


list<string> MyDAQ::splitString(string value, char separator, int spacing)
{
	list<string> values;
	if (value.length() > 0) {
		size_t found = value.find_first_of(separator);
		while (found != string::npos)
		{
			values.push_back(value.substr(0, found));
			value = value.substr(found + spacing);
			found = value.find_first_of(separator);
		}
		values.push_back(value);
	}
	return values;
}


int MyDAQ::readChannels()
{
	while (updating); // NOTE: wait, while updating
	reading = true;

	int32 err = 0, read = 0, result = 0;
	DWORD start, end;
	start = GetTickCount();
	CTime startTime = CTime::GetCurrentTime();

	float64* samples = new float64[(samplingRate / 5)*channels.size()];

	err = DAQmxStartTask(handle);
	if (err != 0)
		result = -1; // error on start task

	err = DAQmxReadAnalogF64(handle, (samplingRate / 5), 10.0, DAQmx_Val_GroupByChannel, samples, (samplingRate / 5)*channels.size(), &read, NULL);
	if (err != 0)
		result = -2; // error on read ai

	err = DAQmxWaitUntilTaskDone(handle, 10.0);
	if (err != 0)
		result = -3; // error while wait until done

	err = DAQmxStopTask(handle);
	if (err != 0)
		result = -4; // error on stop task

	CTime endTime = CTime::GetCurrentTime();

	list<MyDAQChannel>::iterator it;
	for (it = channels.begin(); it != channels.end(); it++) {
		(*it).start = startTime;
		(*it).end = endTime;
		int idx = distance(channels.begin(), it);
		for (int i = 0; i < (samplingRate / 5) * 4; i++) {
			(*it).samples[i] = (*it).samples[i + (samplingRate / 5)];
		}
		for (int i = 0; i < (samplingRate / 5); i++) {
			(*it).samples[i+(samplingRate / 5) * 4] = samples[i+(idx*(samplingRate / 5))];
		}
	}

	end = GetTickCount();
	reading = false;

	if (result < 0)
		return result;
	return (end - start);
}

int MyDAQ::setChannels(CString myDAQ, list<CString> c)
{
	while (reading); // NOTE: wait, while reading
	updating = true;

	channels.clear();
	CString physicalChannel("");

	list<CString>::iterator it;
	for (it = c.begin(); it != c.end(); it++) {
		if (physicalChannel.GetLength() > 0)
			physicalChannel += ", ";
		physicalChannel += (myDAQ + "/" + (*it));

		MyDAQChannel m;
		m.name = string((CT2CA) (*it));
		m.samples = new float64[samplingRate];
		channels.push_back(m);
	}

	int32 err = 0;
	int result = 0;

	err = DAQmxClearTask(handle);
	if (err != 0)
		result = -1; // Error on clear task

	err = DAQmxCreateTask(NULL, &handle);
	if (err != 0)
		result = -2; // Error on create task

	err = DAQmxCreateAIVoltageChan(handle, (CT2CA)physicalChannel, NULL, DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL);
	if (err != 0)
		result = -3; // Error on create channel

	err = DAQmxCfgSampClkTiming(handle, NULL, samplingRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, (samplingRate / 5));
	if (err != 0)
		result = -4; // Error on cfg samp timing

	updating = false;
	if (result != 0)
		channels.clear();
	return result;
}


list<MyDAQChannel> MyDAQ::getChannels()
{
	return channels;
}