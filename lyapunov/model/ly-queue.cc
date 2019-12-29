#include "ly-queue.h"
#include "ns3/log.h"
#include <math.h>
#include <time.h>
#include <stdlib.h> //RAND_MAX
namespace ns3{
NS_LOG_COMPONENT_DEFINE("ly-queue");
const float kLn2=0.6932;
const int KQueueExpect=5;
const int KQueueMax=10;
//exponential distribution;
static uint8_t seed_set_flag=0;
void set_seed(){
	if(!seed_set_flag){
	srand((unsigned)time(NULL));
	}
	seed_set_flag=1;
}
double e_random(double lambda){
    double ret=0.0;
    double u=0.0;
    do{
        u=(double)rand()/(double)RAND_MAX;;
    }while(u<=0||u>1);
    ret=(-1.0/lambda)*log(u);
    return ret;
}
LyQueue::LyQueue(float beta,float v,float uE,
		float bps,int slot,int L){
	m_beta=beta;
	m_v=v;
	m_uE=uE;
	m_rate=bps;
	m_slot=slot;
	m_MTU=L;
	m_interval=((double)m_MTU*8*1000)/(m_rate);
	m_lambda=1.0/m_interval;
	set_seed();
	m_running=true;
	m_targetQ=KQueueExpect*m_MTU;
	m_maxQ=KQueueMax*m_MTU;
}
void LyQueue::Start(){}
void LyQueue::Stop(){
	m_running=false;
}
void LyQueue::setVisitor(Visitor*visitor){
	m_visitor=visitor;
}
void LyQueue::Enqueue(int len){
	if(!m_running){
		return;
	}
	m_count++;
	//NS_LOG_INFO("enque "<<m_count);
	m_queue+=len;
	if(m_firstPacket){
		ConsumePacket();
		double micro=e_random(m_lambda)*1000;
		Time next=MicroSeconds(micro);
		m_packetsDequeueTimer=Simulator::Schedule(next,
						&LyQueue::ProcessPackets,this);
		m_firstPacket=false;
		Time next2=MilliSeconds(m_slot);
		m_rateAdjustTimer=Simulator::Schedule(next2,
				&LyQueue::AdjustRateEvent,this);
	}
}
void LyQueue::ProcessPackets(){
	if(!m_running){
		return ;
	}
	if(m_packetsDequeueTimer.IsExpired()){
		ConsumePacket();
		double micro=e_random(m_lambda)*1000;
		Time next=MicroSeconds(micro);
		m_packetsDequeueTimer=Simulator::Schedule(next,
						&LyQueue::ProcessPackets,this);
	}
}
void LyQueue::ConsumePacket(){
	if(m_queue>=m_MTU){
		m_queue-=m_MTU;
	}
    if(m_queue>m_targetQ){
        AdjustRate();
    }
}
void LyQueue::AdjustRateEvent(){
	if(!m_running){
		return ;
	}
	if(m_rateAdjustTimer.IsExpired()){
		AdjustRate();
		Time next2=MilliSeconds(m_slot);
		m_rateAdjustTimer=Simulator::Schedule(next2,
				&LyQueue::AdjustRateEvent,this);
		if(!m_queueTrace.IsNull()){
			m_queueTrace(m_queue/m_MTU);
		}
	}
}
void LyQueue::AdjustRate(){
	float previous=m_visitor->getCurrentRate();
	if(m_queue>0){
		//float bit=m_v/(m_queue*8);
		//float rate=bit*1000/m_slot;
		if(m_visitor){
			if(m_queue<m_maxQ){
			float ratio=(float)m_targetQ/(1.0*m_queue);
			if(ratio>1.25){ratio=1.25;}
			float targetRate=previous*ratio;
			m_visitor->ChangeRate(targetRate);
			NS_LOG_INFO("adjust "<<targetRate);
			}else{
				m_visitor->SetRateToMin();
			}
		}
	}else{
		float targetRate=previous*1.125;
		if(m_visitor){
			m_visitor->ChangeRate(targetRate);
		}
	}
}
}
