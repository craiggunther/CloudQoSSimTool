// Module that implements a data center scheduler fifo.
//
//	It is under developement..
//
// @authors Gabriel Gonz&aacute;lez Casta&ntilde;&eacute
// @date 2014-06-10

// disabled print energy part by default value to false
//@author bing gao
//@date 2016-04-14

package icancloud.src.Management.DataCenterManagement.ResourcesProvisioning.DataCenterSchedulers.DataCenterSchedulerFIFO;

import icancloud.src.Management.DataCenterManagement.ResourcesProvisioning.IDataCenterManager;


module DataCenterSchedulerFIFO like IDataCenterManager
{        
		double timeToStart; 										// The time that cloud waits until it starts
        int numberOfPFSRemoteServers;								// The number of parallel servers used as remote storage
        int timeBetweenScheduleEvents_s= default(1);				// Time until the scheduler is activated again 
        double timeBetweenLogResults_s= default(1.0);				// Time between all data center is checked extracting the energy measurements.
        bool printEnergyToFile = default(true);						// print the energy to a log file.     
        bool printEnergyTrace = default(true);						// print each schedule event the consumption values of the nodes    	
        bool printNodePowerConsumed = default (false);				// print Node instant consumption
    	bool printNodeEnergyConsumed = default (false);				// print Node energy consumed
    	bool printComponentsPowerConsumed = default (false);			// print main subsytems instant consumption
		bool printComponentsEnergyConsumed = default (false);		// print main subsytems instant consumption     
    	bool printDataCenterPowerConsumed = default (false);			// print all the data-center energyConsumed
    	bool printDataCenterEnergyConsumed = default (false);		// print all the data-center instant consumption
        double minimum_percent_storageNode_free= default(0.5);		// The percent minimum that represents the amount of storage in a data server before select other.
        bool memorization;											// Parameter pending to test. Do not change the value            	        
        bool outputCompression;    		    						// compress the output files
        @class(DataCenterSchedulerFIFO);
        @display("i=msg/job");
        
}
