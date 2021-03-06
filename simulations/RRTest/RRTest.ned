//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package icancloud.simulations.RRTest;
import icancloud.src.Management.NetworkManager.NetworkManager;
import icancloud.src.Architecture.Aggregation.Rack;
import icancloud.src.Virtualization.Hypervisor.HypervisorManagers.H_NetworkManager.NetworkServices.NetworkService.NetworkService;
import icancloud.src.Virtualization.VirtualMachines.SetDefinition.DockerSet;
import icancloud.src.Virtualization.VirtualMachines.VMRequestManager.VMRequestManager;

import inet.transport.tcp.TCP;

import ned.DatarateChannel;
import inet.nodes.ethernet.EtherSwitch;
import icancloud.src.Architecture.Node.NodeVL.NodeVL;
import icancloud.src.Management.CloudManagement.ResourcesProvisioning.**.*;

import icancloud.src.Management.Topology.Topology;

import icancloud.src.Users.UserGenerator.UserGenerator;
import icancloud.src.Virtualization.VirtualMachines.SetDefinition.VmSet;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;

// ---------------------------------------------------------------
//   Definition of channels
// ---------------------------------------------------------------
channel Channel_0_RRTest extends DatarateChannel
{
    delay = 1.25E-4us;
    datarate = 10Gbps;
    ber = 0.0;
    per = 0.0;
}

channel Channel_1_RRTest extends DatarateChannel
{
    delay = 1.25E-4us;
    datarate = 10Gbps;
    ber = 0.0;
    per = 0.0;
}

channel RackChannel_0_RRTest extends DatarateChannel
{
    delay = 1.25E-4us;
    datarate = 10Gbps;// RackChannelBandwidth
    ber = 0.0;
    per = 0.0;
}

// ---------------------------------------------------------------
//   Definition of the cloud
// ---------------------------------------------------------------
network RRTest
{
    @display("bgb=1035,403");
    // -------------------------------------------------------
    //   Definition of main modules
    // -------------------------------------------------------
    submodules:


        // ---------------------------------------------------------------
        //   Network configurator
        // ---------------------------------------------------------------
        configurator: IPv4NetworkConfigurator {
            parameters:
                config = xml("<config><interface hosts='**' address='192.168.x.x' netmask='255.x.x.x'/></config>");
                @display("p=495,67");
        }

        // ---------------------------------------------------------------
        //    Network Manager
        // ---------------------------------------------------------------
        networkManager: NetworkManager {

            @display("p=870,45");
        }
        // ---------------------------------------------------------------
        //   Definition of cloud scheduler
        // ---------------------------------------------------------------
        manager: CloudSchedulerRR {
            parameters:
                numberOfPFSRemoteServers = 2;
                remote_storage_VMs_per_node = 2;
                minimum_percent_storageNode_free = 0.15;
                virtualIPsBasis = "10.0.0.1";
               // timeToStart = 1.0;
                @display("p=445,156");
        }

        // ---------------------------------------------------------------
        //   Definition of user generator
        // ---------------------------------------------------------------
        userGenerator: UserGenerator {
            parameters:
                numCells = 1;
                @display("p=445,318");
        }

        // ---------------------------------------------------------------
        //   Definition of switches
        // ---------------------------------------------------------------
        switch_0: EtherSwitch {
            @display("p=146,203");
        }
        rackSwitch_0: EtherSwitch {
            @display("p=241,117");
        }

        // ---------------------------------------------------------------
        //   Definition of Computing Nodes
        // ---------------------------------------------------------------
        // ---------------------------------------------------------------
        //   Definition of Storage Nodes
        // ---------------------------------------------------------------
        ns_0_NodeStorage_C: NodeVL {
            @display("p=160,318");
        }
        ns_1_NodeStorage_C: NodeVL {
            @display("p=330,230");
        }
        //------------------------------------------------------
        // Modules that defines the topology and the set of VMs 
        //------------------------------------------------------
        vmSet: VmSet {
            @display("p=651,75");
        }
        topology: Topology {
            @display("p=545,204");
        }
        dockerSet: DockerSet {
            @display("p=712,243");
        }
        // ---------------------------------------------------------------
        //   Definition of Racks
        // ---------------------------------------------------------------
      //  rc_0_Rack_A_16[16]: NodeVL {

       //     @display("p=279,45");
      //  }


        rack1: Rack {
            @display("p=78,54;is=l");
        }
        rack2: Rack {
            @display("p=330,44;is=l");
        }
    // ---------------------------------------------------------------
    //   Connections section
    // ---------------------------------------------------------------
    connections allowunconnected:

        // Connections for rack rc_0_Rack_A_16
      //  for i=0..15 {
      ////      rc_0_Rack_A_16[i].ethg++ <--> RackChannel_0_RRTest <--> rackSwitch_0.ethg++;
      //  }

        rackSwitch_0.ethg++ <--> Channel_0_RRTest <--> switch_0.ethg++;
        ns_0_NodeStorage_C.ethg++ <--> Channel_1_RRTest <--> switch_0.ethg++;
        ns_1_NodeStorage_C.ethg++ <--> Channel_1_RRTest <--> switch_0.ethg++;

        rack1.ethg[0] <--> rackSwitch_0.ethg++;
        rack2.ethg[0] <--> rackSwitch_0.ethg++;
}

