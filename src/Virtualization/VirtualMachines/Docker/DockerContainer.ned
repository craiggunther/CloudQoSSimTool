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

package icancloud.src.Virtualization.VirtualMachines.Docker;


import inet.linklayer.queue.DropTailQueue;
import inet.networklayer.xMIPv6Support_None;


module DockerContainer

{
    parameters:


      //  int numCores;												// Number of CPUs
      //  double memorySize_MB;										// Amount of memory in MiB
        //int storageSize_GB;											// Amount of storage in GiB

		int numStorageSystems = default(1);							// Number of Block Servers     
        int numNetworkInterfaces = default(1);						// Number of network interfaces
	
        string ContainerName = default("");
        string hostName = default("");								// Node's hostname
        string osModuleType = default("VmOperatingSystemModule");  	// Operating System Module type            	
        string cpuModuleType = default("CPU_Module");				// CPU Module type               
        
        string ip = default("");								    // VM IP
        bool IPForward = default(false);							// IP Forward
        int namid = default(-1);									// Nam ID
        string routingFile = default("");							// Routing File

    


        //string osModuleType = default("VmOperatingSystemModule");  	// Operating System Module type            	
       // string cpuModuleType = default("CPU_Module");				// CPU Module type               

      
      

        @dynamic(true);
        @class("DockerContainer");
        @display("i=block/container");

    gates:
        //Storage system
       // input fromNodeStorageSystem[numStorageSystems] @loose;
       // output toNodeStorageSystem[numStorageSystems] @loose;

        //cpu        
       // input fromNodeCPU @loose;
       // output toNodeCPU @loose;

		//memory
        input fromNodeMemoryI @loose;
        input fromNodeMemoryO @loose;
        output toNodeMemoryI @loose;
        output toNodeMemoryO @loose;

        // network 

        input fromDockerDaemon @loose;
        output toDockerDaemon @loose;

   
    connections allowunconnected:
}

