#pragma once
namespace ns3{
class LyConsumer{
public:
	virtual void Enqueue(int len){};
	virtual ~LyConsumer(){}
};
}
