#include "ly-producer.h"
#include "ns3/log.h"
namespace ns3{
NS_LOG_COMPONENT_DEFINE("ly-producer");
LyProducer::LyProducer(float initRate,float maxRate,int L){
	m_minRate=initRate;
	m_rate=initRate;
	m_maxRate=maxRate;
	m_MTU=L;
}
void LyProducer::ChangeRate(float bps){
	if(bps<=m_maxRate&&bps>=m_minRate){
		m_rate=bps;
		//NS_LOG_INFO("rate "<<m_rate/1000);
		if(!m_rateTrace.IsNull()){
			m_rateTrace(m_rate/1000);
		}
	}
	if(bps>m_maxRate||bps<m_minRate){
		NS_LOG_INFO(bps);
	}
}
void LyProducer::SetRateToMin(){
	ChangeRate(m_minRate);
}
void LyProducer::setRateToMax(){
	ChangeRate(m_maxRate);
}
void LyProducer::setConsumer(LyConsumer *con){
	m_consumer=con;
}
void LyProducer::Start(){
	m_running=true;
	GeneratePacket();
	double micro=1.0*1000*1000*m_MTU*8/m_rate;
	int64_t t=micro;
	//NS_LOG_INFO("NEXT "<<micro);
	Time next=MicroSeconds(t);
	m_packetsTimer=Simulator::Schedule(next,
					&LyProducer::Process,this);
	if(!m_rateTrace.IsNull()){
		m_rateTrace(m_rate/1000);
	}
}
void LyProducer::Stop(){
	m_running=false;
}
void LyProducer::Process(){
	if(!m_running){return;}
	if(m_packetsTimer.IsExpired()){
		GeneratePacket();
		double micro=1.0*1000*1000*m_MTU*8/m_rate;
		//NS_LOG_INFO("NEXT "<<micro<<" "<<m_rate/1000);
		int64_t t=micro;
		Time next=MicroSeconds(t);
		m_packetsTimer=Simulator::Schedule(next,
						&LyProducer::Process,this);
	}
}
void LyProducer::GeneratePacket(){
	if(m_consumer){
		m_consumer->Enqueue(m_MTU);
	}
}
}
