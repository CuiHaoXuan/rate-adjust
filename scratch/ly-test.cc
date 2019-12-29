#include "ns3/core-module.h"
#include "ns3/log.h"
#include "ns3/lyapunov-module.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE("ly-test");
static double simDuration=200;
int main(int argc, char *argv[]){
	LogComponentEnable("ly-queue",LOG_LEVEL_ALL);
	//LogComponentEnable("ly-producer",LOG_LEVEL_ALL);
    std::string name=std::string("ly_queue");
    float initRate=1000000;
    float maxRate=5000000;
    int MTU=500;
    float beta=10;
    float V=50;
    float UE=10;
    float cosumerate=2000000;
    int slot=100; //in milliseconds;

    Ptr<LyProducer> producer=CreateObject<LyProducer>(initRate,maxRate,MTU);
    Ptr<LyQueue> queue=CreateObject<LyQueue>(beta,V,UE,cosumerate,slot,MTU);
    LyTrace *trace=new LyTrace(name);
    producer->setRateTraceFun(MakeCallback(&LyTrace::OnChangeRate,trace));
    queue->setQueueTraceFun(MakeCallback(&LyTrace::OnQueue,trace));
    LyProducer *producer_ptr=PeekPointer(producer);
    queue->setVisitor(producer_ptr);
    LyQueue *queue_ptr=PeekPointer(queue);
    producer->setConsumer(queue_ptr);
    producer->Start();
    Simulator::Stop (Seconds(simDuration));
    Simulator::Run ();
    Simulator::Destroy();
    producer->Stop();
    queue->Stop();
    delete trace;
    return 0;
}
