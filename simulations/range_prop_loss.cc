#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/spectrum-channel.h"






using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MyRangedPropogationLossModel");




int
main (int argc, char *argv[])
{

    uint32_t nReceivers = 4;
    uint32_t transmitter = 1;

    // Create Tx node
    // Add transmitter node to receiver network

    // Create Receiver Nodes

    // Set channel attributes
    // data rate
    // delay
    // Create station node (Tut 3, for WiFi)


    // Construct PHY and channel helpers
    //      -- What are appropriate helpers?
    //      -- What makes sense for my model?
    




    Simulator::Run ();
    Simulator::Destroy ();
    return 0;



}// end main
