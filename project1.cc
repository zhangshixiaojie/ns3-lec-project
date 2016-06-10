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

#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HelloSimulator");
std::string name,num;        // 全局变量 姓名和学号
int freq;                       //全局变量 输出频率
static void printHello() 
{
	std::cout<<Simulator::Now()<<" "<<name<<" "<<num<<std::endl;
	Simulator::Schedule(Seconds(freq),&printHello);  //插入事件
}
// ./test arg0 arg1 arg2
// argc=4
// argv[0] -> "test"
// argv[1] -> "arg0"
// argv[2] -> "arg1"
// argv[3] -> "arg2"
// argv[4] -> NULL

int main (int argc, char *argv[])
{
	CommandLine cmd;
	cmd.AddValue ("name", "my name", name);
        cmd.AddValue ("num", "my num", num);
        cmd.AddValue ("freq", "freq", freq);
	cmd.Parse(argc,argv);
        //标准输入输出
        //std::cout<<Simulator::Now()<<" 请输入姓名："<<std::endl;
        //std::cin>>name;
        //std::cout<<Simulator::Now()<<" 请输入学号："<<std::endl;
        //std::cin>>number;
        //std::cout<<Simulator::Now()<<" 请输入频率："<<std::endl;
        //std::cin>>freq;       
        printHello();
	Simulator::Stop(Seconds(100));
	Simulator::Run ();
	Simulator::Destroy ();
}
