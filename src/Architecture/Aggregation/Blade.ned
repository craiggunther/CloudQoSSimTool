//
// Blade that contains a set of nodes
//
package icancloud.src.Architecture.Aggregation;

import icancloud.src.Architecture.Node.INode;
import inet.nodes.ethernet.EtherSwitch;
import icancloud.src.Architecture.Node.NodeVL.NodeVL;
import icancloud.src.Architecture.Node.Node.Node;
import ned.DatarateChannel;


channel ethernet_bladeChannel extends DatarateChannel
{
    parameters:
        //        delay = default(normal(0.00015, 0.00005)) @unit(s);		// Normal (150us, 50us)
        delay = default(75.0us);
        datarate = default(1000Mbps);					// 1Gbps

        //string nodeType =default ("NodeVL");
}


module Blade

{
    parameters:
        int numNodes;	// Number of nodes.
        @display("i=old/drive2;bgb=405,277");

    gates:
        inout ethg  @labels(EtherFrame-conn);		// Gates


    submodules:

        // Node array
     //   node[numNodes]: <nodeType> like INode {
        node[numNodes]: NodeVL {
     
            parameters:
                @display("i=old/drive1;p=72,40,row,50");
        }
        // Switch
        switch: EtherSwitch {
            parameters:
                @display("i=icons/switch2;p=220,150");
        }

    connections allowunconnected:

        // Connections between Nodes and Switch
        for i=0..numNodes-1 {
            node[i].ethg++ <--> ethernet_bladeChannel <--> switch.ethg++;
        }

        // Connection between switch and outside
        switch.ethg++ <--> ethernet_bladeChannel <--> ethg;
}

