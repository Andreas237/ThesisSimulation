#include "ns3/core-module.h"
#include "ns3/netanim-module.h"
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
    int xMax = 10000;           // X-axis bound for position allocator
    int yMax = 10000;           // Y-axis bound for position allocator
    int zMax = 10000;           // Z-axis bound for position allocator



    // Update defaults per command line
    CommandLine cmd;
    //***  Should only have one jammer and one receiver BSR
    //cmd.AddValue("nJammer","Number of jamming nodes",nJammer);
    //cmd.AddValue("nBsr","Number of BSR nodes",nBsr);
    cmd.AddValue("xMax","Maximum boundary for X-axis", xMax);
    cmd.AddValue("yMax","Maximum boundary for Y-axis", yMax);
    cmd.AddValue("zMax","Maximum boundary for Z-axis", zMax);
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
    NodeContainer bsrNodes;
    bsrNodes.Create(nBsr);

    // TODO: Construct PHY and channel helpers
    //      -- What are appropriate helpers?
    //      -- What makes sense for my model?
    //      -- PathLoss Model here?


    SpectrumChannelHelper channel = SpectrumChannelHelper::Default();
    // Propogation Loss Model https://www.nsnam.org/doxygen/classns3_1_1_log_distance_propagation_loss_model.html
    channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel",
                                "Exponent",DoubleValue(2.5),
                                "ReferenceDistance",DoubleValue(2),
                                //TODO: change ReferenceLoss based on ReferenceDistance calcs from experiment data
                                "ReferenceLoss",DoubleValue(10));

    /*
    // Per the example https://www.nsnam.org/wiki/Wireless_jamming_model#Jammer
    // Setup the wireless channel first
    JammerHelper jammerHelper;
    jammerHelper.SetJammerType ("ns3::ConstantJammer");             // This is what we do
    */










    // TODO: Create mobility model
    //      -- Jammer moves in spherical coordinates
    //      -- BSRs are fixed
    //      -- Log movement of Jammers
    //      -- TODO: probably could use SetMobilityModel=RandomWaypoint
    MobilityHelper mobility;            // mobility model

    // Gauss-Markov example from repositorio.cedia.org.ec/bitstream/123456789/960/12/T12_Mobilityenns3_vf.pdfs
    // Position of the Jammer
    mobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                  "X",DoubleValue(xMax),
                                  "Y",DoubleValue(yMax),
                                  "Z",DoubleValue(zMax),
                                  "Rho", StringValue ("ns3::UniformRandomVariable[Min=1|Max=2]"),       // don't want them taking up too much space
                                  "Theta", StringValue ("ns3::UniformRandomVariable[Min=0|Max=40]")     // 40 unit radius?
                                  );
    // Set the mobility model type

    /*
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                            "Bounds", RectangleValue (Rectangle (-xMax, xMax, -yMax, yMax)));
    */

    ///*
    mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel","Bounds",
                            BoxValue (Box (0, xMax, 0, xMax, 0, zMax)),
                            "TimeStep", TimeValue (Seconds (0.5)),
                            "Alpha", DoubleValue (0.85),                    // Memory
                            "MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=500|Max=1200]"),
                            "MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=10]"),
                            "MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
                            "NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=1.0|Variance=1.0|Bound=3.0]"),
                            "NormalDirection",StringValue ("ns3::NormalRandomVariable[Mean=0.3|Variance=0.2|Bound=0.6]"),
                            "NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"));

    //*/


    // install the mobility model to some nodes
    mobility.Install(jammerNodes);




    // Set the BSR to be fixed with a PositionAllocator
    //TODO: If multiple BSRs position them differently
    Ptr<ListPositionAllocator> positionAllocS = CreateObject<ListPositionAllocator> ();
    positionAllocS->Add(Vector((int)xMax/2, (int)yMax/2, (int)zMax/2));// node
    MobilityHelper mobilityS;
    mobilityS.SetPositionAllocator(positionAllocS);
    mobilityS.SetMobilityModel("ns3::ConstantPositionMobilityModel"); //whatever it is
    mobilityS.Install (bsrNodes);





    // log the movement of the nodes to CLI!  (Same as with third.cc)
    std::ostringstream oss;

    oss <<
    "/NodeList/" << jammerNodes.Get (nJammer - 1)->GetId () <<
    "/$ns3::MobilityModel/CourseChange";

    Config::Connect (oss.str (), MakeCallback(&CourseChange));





    // Simulator stop
    Simulator::Stop (Seconds (120.0));




    // Call my logger functions to test cmd input
    LogNodeContainer("jammerNodes",jammerNodes);
    LogNodeContainer("bsrNodes",bsrNodes);





    // Generate output for NetAnim - see for almost correct example repositorio.cedia.org.ec/bitstream/123456789/960/12/T12_Mobilityenns3_vf.pdf
    AnimationInterface anim ("jammer-adhoc-animation.xml"); // Mandatory
    anim.UpdateNodeDescription (jammerNodes.Get(0), "JAMMER");//Optional
    anim.UpdateNodeColor (jammerNodes.Get(0), 0, 255, 0); // Optional (green)
    anim.UpdateNodeSize (jammerNodes.Get(0)->GetId(),  xMax*.02,yMax*.02); // Optional (green)
    anim.UpdateNodeDescription (bsrNodes.Get(0), "BSR");//Optional
    anim.UpdateNodeColor (bsrNodes.Get(0), 200, 0, 0); // Optional (red)
    anim.UpdateNodeSize (bsrNodes.Get(0)->GetId(), xMax*.05,yMax*.05); // Optional (green)
    anim.EnablePacketMetadata (true); // Optional







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
