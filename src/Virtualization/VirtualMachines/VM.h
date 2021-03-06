//
// This module interface defines the main parameters of a virtual machine
//
// A virtual machine is defined to iCanCloud simulator as a type of machine without physical resources.
// The physical resources are managed by hypervisor and the virtual machine is linked to the hypervisor in order
// to perform those tasks.
// The main parameters that define a virtual machine are:
//      - numCores
//      - memorySize_MB
//      - storageSize_GB
//
// @author Gabriel Gonz&aacute;lez Casta&ntilde;&eacute
// @date 2012-10-23
// @author Bing, Zahra
// update VM to support docker container technology
// @date 2016-08-02

#ifndef VM_H_
#define VM_H_

#include "Machine.h"
#include "ElementType.h"
#include "VMSyscallManager.h"

//#include "DockerContainer.h"

#include "VMRequestManager.h"
#include "DockerDaemon.h"

class VMRequestManager;
class DockerContainer;
class DockerDaemon;


class VM : public Machine{

protected:

     int pending_operation;             // defined as: PENDING_STORAGE || PENDING_SHUTDOWN || PENDING_STARTUP
     string vmName;                     // Virtual machine type


     //identify where is the VM and who is its property.
     //for example rack1.blade[0].node[0]. the nodesetname is node, nodename is 0.
     int nodeName;											    // To identify which node is.
     string nodeSetName;										// To identify the set.
     string ip;                                                 // IP address of the VM

     vector <vmStatesLog_t*> states_log;    // To log the states of the vms (composed by the code of the state and when a change is perfomed (in minutes).

     vector<DockerContainer*>  dockerset;                                       // all containers exist in vm

    // vector<Docker*>  dockerset;                                       // Each state changed will generate a new vmStatesLog entry-


public:
     int userID;                        // User identification obtained from getId() from omnetpp.
     bool is_freezed;                     // flag to detect the VM is freeze or not
     DockerDaemon* dockerDaemon;
     bool has_dockers;                  // flag to indicate this vm run docker or not
     VM();
    VM( elementType* el);
    /*
    * Destructor
    */
    virtual ~VM();
    VMRequestManager* vmreqmgr;
 //   VMSchedulerRR* vmschrr;
    /*
    * Initialization of the module
    */
    void initialize();

    /*
    * Finalization of the module
    */
    void finish();

    /*
     * Getter and setter of a pending operation
     */
    int getPendingOperation(){return pending_operation;};
    void setPendingOperation(int newPendingOperation){pending_operation = newPendingOperation;};

    /*
     * Getter of user id owner of the VM
     */
    void setUid(int user){userID = user;};
    int getUid (){return userID;}
    string getName () {return vmName;}
    void setName(string  newName){vmName = newName;};

    void changeState(string newState);
    bool isAppRunning(int pId);

    /*
     * Getter and setter for the Node type and index where the VM is placed into
     */
    int getNodeName (){return nodeName;};
    void setNodeName (int newNodeName){nodeName = newNodeName;};
    string getNodeSetName (){return nodeSetName;};
    void setNodeSetName (string newNodeSetName){nodeSetName = newNodeSetName;};

    /*
     * Destroy the VM module and free the resources to be requested by other users to allocate new virtual machines
     */
    void shutdownVM ();

    /*
     *  Operate with log states
     */
    int getLogStatesQuantity (){return states_log.size();};
    vmStatesLog_t* getLogState (int i){return (*(states_log.begin() + i));};

    /*
     * Setter for manager
     */

    void setManager(icancloud_Base* manager);

    /*
            * input: docker container id
            * output None
            * this function will make docker container process go to sleep and release all the resources.
            */
    void sleep(string containerID);
    /*
           * input : docker container id
           * output : None
           * this function will restart previous sleep container
           */
    void wakeup(string containerID);

    //TCP
    void send_tcp_msg(string* msg, string* dest_ip);
    void receive_tcp_msg();


};

#endif /* VM_H_ */
