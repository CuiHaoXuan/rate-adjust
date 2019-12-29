#include "ly-trace.h"
#include <unistd.h>
#include "ns3/core-module.h"
namespace ns3{
LyTrace::LyTrace(std::string &name){
	OpenTraceRateFile(name);
	OpenTraceQueueFile(name);
}
LyTrace::~LyTrace(){
	if(m_rate.is_open()){
		m_rate.close();
	}
	if(m_queue.is_open()){
		m_queue.close();
	}
}
void LyTrace::OnChangeRate(float rate){
	if(m_rate.is_open()){
		double now=Simulator::Now().GetSeconds();
		m_rate<<now<<"\t"<<rate<<std::endl;
	}
}
void LyTrace::OnQueue(int queue){
	if(m_queue.is_open()){
		double now=Simulator::Now().GetSeconds();
		m_queue<<now<<"\t"<<queue<<std::endl;
	}
}
void LyTrace::OpenTraceRateFile(std::string &name){
	char buf[FILENAME_MAX];
	memset(buf,0,FILENAME_MAX);
	std::string path = std::string (getcwd(buf, FILENAME_MAX)) + "/traces/"
			+name+"_ly_rate.txt";
	m_rate.open(path.c_str(), std::fstream::out);
}
void LyTrace::OpenTraceQueueFile(std::string &name){
	char buf[FILENAME_MAX];
	memset(buf,0,FILENAME_MAX);
	std::string path = std::string (getcwd(buf, FILENAME_MAX)) + "/traces/"
			+name+"_ly_queue.txt";
	m_queue.open(path.c_str(), std::fstream::out);
}
}

