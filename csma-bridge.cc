/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// Network topology
//
//        n0     n1
//        |      |    csma
//       ----------
//       | Switch |   bridge
//       ----------
//       |   |    | 
//       n2  n3   n4  10.1.1.0
//
//
// - CBR/UDP flows from n0 to n1 and from n3 to n0
// - DropTail queues 
// - Tracing of queues and packet receptions to file "csma-bridge.tr"

#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/bridge-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaBridgeExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  
  CommandLine cmd;
  cmd.Parse (argc, argv);
 
  if (verbose)                //启动记录组件
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	
    }


  // Explicitly create the nodes required by the topology (shown above).
  NS_LOG_INFO ("Create nodes.");
  NodeContainer terminals;
  terminals.Create (5);

  NodeContainer csmaSwitch;
  csmaSwitch.Create (1);

  NS_LOG_INFO ("Build Topology");
  CsmaHelper csma;                                     //csma信道
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  // Create the csma links, from each terminal to the switch

  NetDeviceContainer terminalDevices;
  NetDeviceContainer switchDevices;

  for (int i = 0; i < 5; i++)
    {
      NetDeviceContainer link = csma.Install (NodeContainer (terminals.Get (i), csmaSwitch));
      terminalDevices.Add (link.Get (0));
      switchDevices.Add (link.Get (1));
    }

  // Create the bridge netdevice, which will do the packet switching
  Ptr<Node> switchNode = csmaSwitch.Get (0);
  BridgeHelper bridge;                                 //bridge信道
  bridge.Install (switchNode, switchDevices);

  // Add internet stack to the terminals
  InternetStackHelper internet;
  internet.Install (terminals);

  //We've got the "hardware" in place.  Now we need to add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (terminalDevices);

  // Create an echoserver application to send UDP datagrams from n0 to n1.  
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (terminals.Get(0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //
  // Create an optional packet sink n1 to receive these packets 
  UdpEchoClientHelper echoClient (Ipv4Address("10.1.1.1"), 9);  //目标节点IP
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));              
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
 
  //安装其他节点应用程序
  for (int i = 1; i < 5; i++)
  {  
     ApplicationContainer clientApps = echoClient.Install (terminals.Get(i));      
     clientApps.Start (Seconds (2.0));     //应用程序开始时间
     clientApps.Stop (Seconds (10.0));     //应用程序结束时间
  }

  //启动互联网络路由
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Simulator::Stop (Seconds (10.0));    //仿真截止时间

  NS_LOG_INFO ("Configure Tracing.");
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
