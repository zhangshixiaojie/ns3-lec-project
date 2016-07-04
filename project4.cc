#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
//#include "ns3/lte-module.h"
//#include <ns3/point-to-point-epc-helper.h>

#include <iostream>
// Default Network Topology
//榛樿缃戠粶鎷撴墤
// Number of wifi or csma nodes can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//   Wifi1 10.1.1.0
//                         AP
//  *      *      *     *
//  |      |      |     |      10.1.3.0
// n5   n6   n7   n0 ------------------ n1   n2   n3   n4
//                                       |    |    |    |
//                                             *      *      *     *
//                                                wifi2 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("project2");		//瀹氫箟璁板綍缁勪欢

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi1 = 6;			//csma鑺傜偣鏁伴噺
  uint32_t nWifi2 = 6;				//wifi鑺傜偣鏁伴噺
   bool tracing = true;


  CommandLine cmd;
  cmd.AddValue ("nWifi2", "Number of wifi2 STA devices", nWifi2);
  cmd.AddValue ("nWifi1", "Number of wifi1 STA devices", nWifi1);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue		//鍒ゆ柇鏄惁瓒呰繃浜?50涓紝瓒呰繃鎶ラ敊 , 鍘熷洜锛?  
  if (nWifi1 > 250 || nWifi2 > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//鍚姩璁板綍缁勪欢
    }


  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  //创建信道，设置信道参数，在设备安装到节点上
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

////////////////////////////////////////////////////////////////
  //鍒涘缓wifi1sta鏃犵嚎缁堢锛孉P鎺ュ叆鐐?  
  NodeContainer wifi1StaNodes;
  wifi1StaNodes.Create (nWifi1);
  //NodeContainer ap1;
  //ap1.Create(1);
  NodeContainer ap1 = p2pNodes.Get (0);

  //纭繚鎵€鏈夌墿鐞嗗眰瀵硅薄浣跨敤鐩稿悓鐨勫簳灞備俊閬擄紝鍗虫棤绾夸俊閬?  
  YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default ();
  channel1.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default ();
  phy1.SetChannel (channel1.Create ());

  //閰嶇疆閫熺巼鎺у埗绠楁硶锛孉ARF绠楁硶
  WifiHelper wifi1 = WifiHelper::Default ();
  wifi1.SetRemoteStationManager ("ns3::AarfWifiManager");

  //閰嶇疆mac绫诲瀷涓簊ta妯″紡锛屼笉鍙戦€佹帰娴嬭姹?
  NqosWifiMacHelper mac1 = NqosWifiMacHelper::Default (); 
  Ssid ssid1 = Ssid ("ns-3-ssid1"); 
  mac1.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (false));
  
  //鍒涘缓鏃犵嚎璁惧锛屽皢mac灞傚拰phy灞傚畨瑁呭埌璁惧涓?  
  NetDeviceContainer staDevices1;
  staDevices1 = wifi1.Install (phy1, mac1, wifi1StaNodes);

  //閰嶇疆AP鑺傜偣鐨刴ac灞備负AP妯″紡锛屽垱寤篈P璁惧
  mac1.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid1));
  NetDeviceContainer apDevices1;
  apDevices1 = wifi1.Install (phy1, mac1, ap1);

  //////////////////////////////////////////////////////////////
  //鍒涘缓wifi2sta鏃犵嚎缁堢锛孉P鎺ュ叆鐐?  
  NodeContainer wifi2StaNodes;
  wifi2StaNodes.Create (nWifi2);
  //NodeContainer ap2;
  //ap2.Create (1);
  NodeContainer ap2 = p2pNodes.Get (1);

  //鍒涘缓鏃犵嚎璁惧浜庢棤绾胯妭鐐逛箣闂寸殑浜掕仈閫氶亾锛屽苟灏嗛€氶亾瀵硅薄涓庣墿鐞嗗眰瀵硅薄鍏宠仈
  //纭繚鎵€鏈夌墿鐞嗗眰瀵硅薄浣跨敤鐩稿悓鐨勫簳灞備俊閬擄紝鍗虫棤绾夸俊閬?  
  YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default ();
  channel2.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default ();
  phy2.SetChannel (channel2.Create ());

  //閰嶇疆閫熺巼鎺у埗绠楁硶锛孉ARF绠楁硶
  WifiHelper wifi2 = WifiHelper::Default ();
  wifi2.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac2 = NqosWifiMacHelper::Default ();

  //閰嶇疆mac绫诲瀷涓簊ta妯″紡锛屼笉鍙戦€佹帰娴嬭姹?  
  Ssid ssid2 = Ssid ("ns-3-ssid2");
  mac2.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid2),
               "ActiveProbing", BooleanValue (false));

  //鍒涘缓鏃犵嚎璁惧锛屽皢mac灞傚拰phy灞傚畨瑁呭埌璁惧涓?  
  NetDeviceContainer staDevices2;
  staDevices2 = wifi2.Install (phy2, mac2, wifi2StaNodes);

  //閰嶇疆AP鑺傜偣鐨刴ac灞備负AP妯″紡锛屽垱寤篈P璁惧
  mac2.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid2));

  NetDeviceContainer apDevices2;
  apDevices2 = wifi2.Install (phy2, mac2, ap2);


//////////////////////////////////////////////////////////////////////
 /*NodeContainer epcNodes;
 epcNodes.Create (2);
 ap1.Add (epcNodes.Get(0));
 ap2.Add (epcNodes.Get(1));
 NetDeviceContainer epcDevices;
 PointToPointEpcHelper epc; 
 epc.AddX2Interface (epcNodes.Get(0), epcNodes.Get(1));*/
 
 //////////////////////////////////////////////////////////////////////
 //纭繚鎵€鏈夌墿鐞嗗眰瀵硅薄浣跨敤鐩稿悓鐨勫簳灞備俊閬擄紝鍗虫棤绾夸俊閬?  
 /* YansWifiChannelHelper channel3 = YansWifiChannelHelper::Default ();
  channel3.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default ();
  phy3.SetChannel (channel3.Create ());

  //閰嶇疆閫熺巼鎺у埗绠楁硶锛孉ARF绠楁硶
  WifiHelper wifi3 = WifiHelper::Default ();
  wifi3.SetRemoteStationManager ("ns3::AarfWifiManager");

  //閰嶇疆mac绫诲瀷涓簊ta妯″紡锛屼笉鍙戦€佹帰娴嬭姹?
  NqosWifiMacHelper mac3 = NqosWifiMacHelper::Default (); 
  //Ssid ssid3 = Ssid ("ns-3-ssid3"); 
  //mac3.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid3));
  //mac3.SetType ("ns3::ApWifiMac")   //,"Ssid", SsidValue (ssid3));
  apDevices1 = wifi3.Install (phy3, mac3, ap1);
  apDevices2 = wifi3.Install (phy3, mac3, ap2);*/

  //////////////////////////////////////////////////////////////// 
  //wifi1閰嶇疆STA绉诲姩鏂瑰紡锛孯andomWalk2dMobilityModel锛岄殢鏈烘父璧版ā鍨?    
  MobilityHelper mobility1;
  mobility1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250)));
  mobility1.Install (wifi1StaNodes.Get(0));
  Ptr<RandomWalk2dMobilityModel> mob1 = wifi1StaNodes.Get(0)->GetObject<RandomWalk2dMobilityModel>();
  mob1->SetPosition(Vector(-15, 0, 0.0)); 
mobility1.Install (wifi1StaNodes.Get(1));
  Ptr<RandomWalk2dMobilityModel> mob3 = wifi1StaNodes.Get(1)->GetObject<RandomWalk2dMobilityModel>();
  mob3->SetPosition(Vector(-20, 5*sqrt(3), 0.0));
mobility1.Install (wifi1StaNodes.Get(2));
  Ptr<RandomWalk2dMobilityModel> mob4 = wifi1StaNodes.Get(2)->GetObject<RandomWalk2dMobilityModel>();
  mob4->SetPosition(Vector(-30, 5*sqrt(3), 0.0));
mobility1.Install (wifi1StaNodes.Get(3));
  Ptr<RandomWalk2dMobilityModel> mob5 = wifi1StaNodes.Get(3)->GetObject<RandomWalk2dMobilityModel>();
  mob5->SetPosition(Vector(-35, 0, 0.0));
mobility1.Install (wifi1StaNodes.Get(4));
  Ptr<RandomWalk2dMobilityModel> mob6 = wifi1StaNodes.Get(4)->GetObject<RandomWalk2dMobilityModel>();
  mob6->SetPosition(Vector(-30, -5*sqrt(3), 0.0));
mobility1.Install (wifi1StaNodes.Get(5));
  Ptr<RandomWalk2dMobilityModel> mob7 = wifi1StaNodes.Get(5)->GetObject<RandomWalk2dMobilityModel>();
  mob7->SetPosition(Vector(-20, -5*sqrt(3), 0.0));

//閰嶇疆AP绉诲姩鏂瑰紡锛孋onstantPositionMobilityModel锛屽浐瀹氫綅缃ā鍨?  
mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility1.Install (ap1);
  Ptr<ConstantPositionMobilityModel> mob = ap1.Get(0)->GetObject<ConstantPositionMobilityModel>();
  mob->SetPosition(Vector(-25, 0, 0.0)); 


  //閰嶇疆绉诲姩妯″瀷锛岃捣濮嬩綅缃?  
MobilityHelper mobility2;
mobility2.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 150, -50, 150)));
  mobility2.Install (wifi2StaNodes.Get(0));
  Ptr<RandomWalk2dMobilityModel> mod1 = wifi2StaNodes.Get(0)->GetObject<RandomWalk2dMobilityModel>();
  mod1->SetPosition(Vector(15, 0, 0.0)); 
mobility2.Install (wifi2StaNodes.Get(1));
  Ptr<RandomWalk2dMobilityModel> mod2 = wifi2StaNodes.Get(1)->GetObject<RandomWalk2dMobilityModel>();
  mod2->SetPosition(Vector(20, 5*sqrt(3), 0.0));
mobility2.Install (wifi2StaNodes.Get(2));
  Ptr<RandomWalk2dMobilityModel> mod3 = wifi2StaNodes.Get(2)->GetObject<RandomWalk2dMobilityModel>();
  mod3->SetPosition(Vector(30, 5*sqrt(3), 0.0));
mobility2.Install (wifi2StaNodes.Get(3));
  Ptr<RandomWalk2dMobilityModel> mod4 = wifi2StaNodes.Get(3)->GetObject<RandomWalk2dMobilityModel>();
  mod4->SetPosition(Vector(35, 0, 0.0));
mobility2.Install (wifi2StaNodes.Get(4));
  Ptr<RandomWalk2dMobilityModel> mod5 = wifi2StaNodes.Get(4)->GetObject<RandomWalk2dMobilityModel>();
  mod5->SetPosition(Vector(30, -5*sqrt(3), 0.0));
mobility2.Install (wifi2StaNodes.Get(5));
  Ptr<RandomWalk2dMobilityModel> mod6 = wifi2StaNodes.Get(5)->GetObject<RandomWalk2dMobilityModel>();
  mod6->SetPosition(Vector(20, -5*sqrt(3), 0.0));

//閰嶇疆AP绉诲姩鏂瑰紡锛孋onstantPositionMobilityModel锛屽浐瀹氫綅缃ā鍨?  
mobility2.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility2.Install (ap2);
  Ptr<ConstantPositionMobilityModel> mob2 = ap2.Get(0)->GetObject<ConstantPositionMobilityModel>();
  mob2->SetPosition(Vector(25, 0, 0.0)); 
 
wifi1.SetStandard(WIFI_PHY_STANDARD_80211b);  //默认为802.11a
wifi1.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate6Mbps"),                                     "RtsCtsThreshold", UintegerValue (0));
wifi2.SetStandard(WIFI_PHY_STANDARD_80211b);  //默认为802.11a
wifi2.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate6Mbps"),                                       "RtsCtsThreshold", UintegerValue (0));


  //宸茬粡鍒涘缓浜嗚妭鐐癸紝璁惧锛屼俊閬撳拰绉诲姩妯″瀷锛屾帴涓嬫潵閰嶇疆鍗忚鏍?  
  InternetStackHelper stack;
  stack.Install (ap2);
  stack.Install (wifi2StaNodes);
  stack.Install (ap1);
  stack.Install (wifi1StaNodes);

  //鍒嗛厤IP鍦板潃
  Ipv4AddressHelper address;
 //wifi1淇￠亾
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer wifi1staipadd;
  wifi1staipadd = address.Assign (staDevices1);
  address.Assign (apDevices1);
 //wifi2淇￠亾
  address.SetBase ("10.1.2.0", "255.255.255.0");
 Ipv4InterfaceContainer wifi2staipadd;
  address.Assign (staDevices2);
  address.Assign (apDevices2);
//p2p淇￠亾
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  //鏀剧疆echo鏈嶅姟绔▼搴忓湪鏈€鍙宠竟鐨剋ifi1鑺傜偣,绔彛涓?
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (wifi2StaNodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //鍥炴樉瀹㈡埛绔斁鍦╳ifi2鏈€鍚庣殑STA鑺傜偣锛屾寚鍚慍SMA缃戠粶鐨勬湇鍔″櫒锛屼笂闈㈢殑鑺傜偣鍦板潃锛岀鍙ｄ负9
  UdpEchoClientHelper echoClient (Ipv4Address("10.1.2.1"), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (8));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));

  ApplicationContainer clientApps = echoClient.Install (wifi1StaNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

   /*for(uint k=0; k < nWifi1; k++)
  {
      ApplicationContainer clientApps = echoClient.Install (wifi1StaNodes.Get (k));
      clientApps.Start (Seconds (2.0));
      clientApps.Stop (Seconds (10.0));
  }*/

  //鍚姩浜掕仈缃戠粶璺敱
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AsciiTraceHelper ascii;             //以ASCII格式的信息输出
  mobility1.EnableAsciiAll (ascii.CreateFileStream ("wifi1.tr"));
  mobility2.EnableAsciiAll (ascii.CreateFileStream ("wifi2.tr"));
  phy1.EnablePcapAll ("wifi1", false);
  phy2.EnablePcapAll ("wifi2", false);
  

  Simulator::Stop (Seconds (10.0));
  if (tracing == true)
    {
      //pointToPoint.EnablePcapAll ("project2");
      phy1.EnablePcap ("project2", apDevices1.Get (0));
      phy2.EnablePcap ("project2", apDevices2.Get (0));
    }
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

