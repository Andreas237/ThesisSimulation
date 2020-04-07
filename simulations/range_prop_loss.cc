#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/spectrum-channel.h"






using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MyRangedPropogationLossModel");

void
LogNodeContainer(std::string, NodeContainer);




int
main (int argc, char *argv[])
{

    // Default values
    uint32_t nJammer = 1;
    uint32_t nBsr = 1;



    // Update defaults per command line
    CommandLine cmd;
    cmd.AddValue("nJammers","Number of jamming nodes",nJammer);
    cmd.AddValue("nBsr","Number of BSR nodes",nBsr);
    cmd.Parse (argc,argv);

    // Create jamming node
    // Add transmitter node to receiver network
    NodeContainer jammer;
    jammer.Create(nJammer);


    // Create Receiver Nodes
    NodeContainer bsr;
    bsr.Create(nBsr);

    // Set channel attributes
    // data rate
    // delay
    // Create station node (Tut 3, for WiFi)


    // Construct PHY and channel helpers
    //      -- What are appropriate helpers?
    //      -- What makes sense for my model?



    LogNodeContainer("jammer",jammer);
    LogNodeContainer("bsr",bsr);


    Simulator::Run ();
    Simulator::Destroy ();
    return 0;



}// end main







void
LogNodeContainer(std::string ContainerName, NodeContainer container) {
    std::ostringstream oss;
    oss <<
    "Container=" << ContainerName <<
    "\tContainer this many nodes=" << container.GetN();
    NS_LOG_UNCOND(oss.str());
}// end LogNodeContainer
