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

package icancloud.simulations.TCP;
import inet.applications.ITCPApp;
import inet.nodes.bgp.BGPRouter;
import inet.nodes.inet.StandardHost;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import ned.DatarateChannel;
import inet.nodes.inet.Router;
import inet.util.NetAnimTrace;
import inet.world.httptools.HttpController;

//
// TODO Auto-generated network
//
network TCP_t
{
    parameters:

        // Channel 1 parameters (to be redefined in .ini file)
        double C1_datarate @unit(bps) = default(2Mbps);
        double C1_delay @unit(s) = default(0.1us);
        double C1_per = default(0);
        double C1_ber = default(0);

        // Channel 2 parameters (to be redefined in .ini file)
        double C2_datarate @unit(bps) = default(4Mbps);
        double C2_delay @unit(s) = default(0.1us);
        double C2_per = default(0);
        double C2_ber = default(0);

        // Channel 3 parameters (to be redefined in .ini file)
        double C3_datarate @unit(bps) = default(4Mbps);
        double C3_delay @unit(s) = default(0.1us);
        double C3_per = default(0);
        double C3_ber = default(0);

        @display("bgb=785,403");
    types:
        channel C1 extends DatarateChannel
        {
            datarate = C1_datarate;
            delay = C1_delay;
            per = C1_per;
            ber = C1_ber;
        }
        channel C2 extends DatarateChannel
        {
            datarate = C2_datarate;
            delay = C2_delay;
            per = C2_per;
            ber = C2_ber;
        }
        channel C3 extends DatarateChannel
        {
            datarate = C3_datarate;
            delay = C3_delay;
            per = C3_per;
            ber = C3_ber;
        }
    submodules:
        client1: StandardHost {
            parameters:
                @display("p=50,80");
        }
        client2: StandardHost {
            parameters:
                @display("p=66,202");
        }
        client3: StandardHost {
            parameters:
                @display("p=118,286");
        }
        client4: StandardHost {
            parameters:
                @display("p=196,308");
        }
        server1: StandardHost {
            parameters:
                @display("p=517,41;i=device/server");
        }
        server2: StandardHost {
            parameters:
                @display("p=527,188;i=device/server");
        }
        router1: Router {
            parameters:
                @display("p=235,115");
        }
        router2: Router {
            parameters:
                @display("p=428,120");
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=149,34");
        }
        //        netAnimTrace: NetAnimTrace {
        //            parameters:
        //                @display("p=248,34");
        //        }
        controller: HttpController {
            parameters:
                @display("p=341,34;i=block/cogwheel_s");
        }
    connections:
        client1.pppg++ <--> C1 <--> router1.pppg++;
        client2.pppg++ <--> C1 <--> router1.pppg++;
        client3.pppg++ <--> C1 <--> router1.pppg++;
        client4.pppg++ <--> C1 <--> router1.pppg++;
        router1.pppg++ <--> C2 <--> router2.pppg++;
        router2.pppg++ <--> C3 <--> server1.pppg++;
        router2.pppg++ <--> C3 <--> server2.pppg++;
}
