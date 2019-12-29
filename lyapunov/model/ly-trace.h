#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
namespace ns3{
class LyTrace{
public:
	LyTrace(std::string &name);
	~LyTrace();
	void OnChangeRate(float rate);
	void OnQueue(int queue);
private:
	void OpenTraceRateFile(std::string &name);
	void OpenTraceQueueFile(std::string &name);
	std::fstream m_rate;
	std::fstream m_queue;
};
}
