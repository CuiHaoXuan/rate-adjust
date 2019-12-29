#pragma once
#include "ly-consumer.h"
#include "ly-queue.h"
namespace ns3{
class LyProducer:public Object,public LyQueue::Visitor{
public:
	LyProducer(float initRate,float maxRate,int L);
	~LyProducer(){}
	void ChangeRate(float bps) override;
	void SetRateToMin() override;
	void setRateToMax() override;
	float getCurrentRate() const override{ return m_rate;}
	void setConsumer(LyConsumer *con);
	void Start();
	void Stop();
	typedef Callback<void,float> TraceRate;
	void setRateTraceFun(TraceRate cb){m_rateTrace=cb;}
	void Process();
private:
	void GeneratePacket();
	bool m_running{false};
	LyConsumer *m_consumer{nullptr};
	float m_minRate;
	float m_rate;
	float m_maxRate;
	int m_MTU;
	EventId m_packetsTimer;
	TraceRate m_rateTrace;
};
}
