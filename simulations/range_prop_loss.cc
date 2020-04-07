#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-helper.h"
#include "ns3/vector.h"






using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MyRangedPropogationLossModel");

void CourseChange (std::string, Ptr<const MobilityModel>);
void LogNodeContainer(std::string, NodeContainer);




int
main (int argc, char *argv[])
{

    // Default values
    uint32_t nJammer = 1;
    uint32_t nBsr = 1;



    // Update defaults per command line
    CommandLine cmd;
    cmd.AddValue("nJammer","Number of jamming nodes",nJammer);
    cmd.AddValue("nBsr","Number of BSR nodes",nBsr);
    cmd.Parse (argc,argv);



    /*
        Don't allow more than 5 jammers otherwise the field will be too crowded
        with interfering signals.
        Don't allow more than 5 BSRs just because...
    */
    if (nJammer > 5 )
    {
        std::cout << "nJammer should be 5 or less; otherwise grid layout exceeds the bounding box" << std::endl;
        return 1;
    }
    if (nBsr > 5 )
    {
        std::cout << "nBsr should be 5 or less; otherwise grid layout exceeds the bounding box" << std::endl;
        return 1;
    }






    // Create jamming node
    // Add transmitter node to receiver network
    NodeContainer jammerNodes;
    jammerNodes.Create(nJammer);


    // Create Receiver Nodes
    NodeContainer bsr;
    bsr.Create(nBsr);

    // TODO: Construct PHY and channel helpers
    //      -- What are appropriate helpers?
    //      -- What makes sense for my model?
    //      -- PathLoss Model here?
    SpectrumChannelHelper channel = SpectrumChannelHelper::Default();



    // TODO: Create mobility model
    //      -- Jammer moves in spherical coordinates
    //      -- BSRs are fixed
    //      -- Log movement of Jammers
    //      -- TODO: probably could use SetMobilityModel=RandomWaypoint
    MobilityHelper mobility;            // mobility model
    mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                  "X",DoubleValue(0.0),
                                  "Y",DoubleValue(0.0),
                                  "Z",DoubleValue(0.0),
                                  "Rho", StringValue ("ns3::UniformRandomVariable[Min=1|Max=2]"),       // don't want them taking up too much space
                                  "Theta", StringValue ("ns3::UniformRandomVariable[Min=0|Max=40]")     // 40 unit radius?
                                  );
    // Set the mobility model type
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                            "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
    // install the mobility model to some nodes
    mobility.Install(jammerNodes);




    // Set the BSR to be fixed...
    //TODO: If multiple BSRs position them differently
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (bsr);





    // log the movement of the nodes!  (Same as with third.cc)
    std::ostringstream oss;
    
    oss <<
    "/NodeList/" << jammerNodes.Get (nJammer - 1)->GetId () <<
    "/$ns3::MobilityModel/CourseChange";

    /*
    for(int i=1; i <= nJammer; i++){
        oss <<
        "/NodeList/" << jammerNodes.Get (nJammer - i)->GetId () <<
        "/$ns3::MobilityModel/CourseChange";
    }
    */

    Config::Connect (oss.str (), MakeCallback(&CourseChange));





    // Simulator stop
    Simulator::Stop (Seconds (10.0));




    // Call my logger functions to test cmd input
    LogNodeContainer("jammerNodes",jammerNodes);
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















void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  NS_LOG_UNCOND (context <<
    " x = " << position.x << ", y = " << position.y << ", z = " << position.z);
}
