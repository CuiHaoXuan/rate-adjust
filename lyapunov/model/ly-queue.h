#pragma once
#include "ns3/core-module.h"
#include "ns3/event-id.h"
#include "ns3/callback.h"
#include "ly-consumer.h"
namespace ns3{
class LyQueue:public Object,public LyConsumer{
public:
	class Visitor{
	public:
		virtual void ChangeRate(float){}
		virtual void SetRateToMin(){}
		virtual void setRateToMax(){}
		virtual float getCurrentRate() const{ return 0.0;}
		virtual ~Visitor(){}
	};
	LyQueue(float beta,float v,float uE,
			float bps,int slot,int L);
	~LyQueue(){}
	void Start();
	void Stop();
	void setVisitor(Visitor*visitor);
	void Enqueue(int len) override;
	typedef Callback<void,int> TraceQueue;
	void setQueueTraceFun(TraceQueue cb){m_queueTrace=cb;}
private:
	void ProcessPackets();
	void ConsumePacket();
	void AdjustRateEvent();
	void AdjustRate();
	float m_beta;
	float m_v;
	float m_uE;
	float m_rate;
	int m_slot;
	int m_MTU;
	double m_interval;
	double m_lambda;
	Visitor *m_visitor{nullptr};
	int m_queue{0};//in packets
	bool m_firstPacket{true};
	bool m_running{false};
	EventId m_packetsDequeueTimer;
	EventId m_rateAdjustTimer;
	TraceQueue m_queueTrace;
	int m_count{0};
	int m_targetQ;
	int m_maxQ;
};
}
