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

#include "GeneralUser.h"

Define_Module(GeneralUser);

GeneralUser::~GeneralUser() {

    // TODO Auto-generated destructor stub
}

void GeneralUser::initialize() {

    printVMs = par("printVMs").boolValue();
    printJobs = par("printJobs").boolValue();
    AbstractCloudUser::initialize();
    print = false;
}

void GeneralUser::finish() {
    AbstractCloudUser::finish();
}

void GeneralUser::userInitialization() {

    // Define ..
    AbstractRequest* vms;

    // Select the vms to start (all) and then send the request to the cloud manager.
    vms = selectVMs_ToStartUp();

    // Start all vms
    if (vms != NULL)
        startVMs(vms);
}

void GeneralUser::userFinalization() {

    // Define ..
    int finishQSize, i, k;
    string jobs;
    ostringstream userData;
    ostringstream userJobsData;
    ostringstream userVmsData;
    ofstream resFile;
    vmStatesLog_t* vm_state;
    vector<cModule*> mods;
    VM* vm;
    // Init ..

    // Begin..
    //Open the file
    if (print) {
        resFile.open(file.c_str(), ios::app);
    }

    // Get the user data
    // UserName - startTime - totalTime -
    userData << userName.c_str() << ";" << startTime.dbl() << ";"
            << endTime.dbl() << endl;

    if (printJobs) {
        // Get the jobs
        finishQSize = getJobResultsSize();

        for (i = 0; i < finishQSize; i++) {
            jobs = printJobResults(getJobResults(i));
            userJobsData << jobs;
        }
    }

    // Get the vms
    if (printVMs) {

        int size = wastedVMs.size();

        for (i = 0; i < size; i++) {

            vm = (*(wastedVMs.begin()));

            userVmsData << "#" << vm->getTypeName() << "[" << vm->getName()
                    << "]:";

            for (k = 0; k < vm->getLogStatesQuantity(); k++) {
                vm_state = vm->getLogState(k);

                if (strcmp(vm_state->vm_state.c_str(), MACHINE_STATE_OFF) == 0)
                    userVmsData << (vm_state->init_time_M) << ";";
                userVmsData << (vm_state->vm_state) << ":";
                userVmsData << (vm_state->init_time_M) << ";";
            }

        }
        userVmsData << endl;
    }

    //Print everything and close the file
    if (print) {
        resFile << userData.str().c_str();
        if (printJobs)
            resFile << userJobsData.str().c_str();
        if (printVMs)
            resFile << userVmsData.str().c_str();
        resFile << endl;                       // Put a blank line between users
        resFile.close();
    }

}
/* --------------------------------------------------------------------
 * ------------------------ Interact with VMs -------------------------
 * --------------------------------------------------------------------
 */

AbstractRequest* GeneralUser::selectVMs_ToStartUp() {

    //Define...
    unsigned int i;
    unsigned int setSize;
    bool reqFilled;
    int maxNumVMsToRequest;
    int size;
    RequestVM* req;

    cout <<"-------------GeneralUser::selectVMs_ToStartUp()--------------------"<< endl;
    //Initialize...
    size = 0;
    maxNumVMsToRequest = getWQ_size();
    setSize = vmsToBeSelected.size();
    req = new RequestVM();
    reqFilled = false;

    /*
     *  Obtain all the VMs of the user. Initially all are into VM_NOT_REQUESTED state.
     *  Then, user request for the VMs change state to free, and that is task of the cloud manager.
     *  If user request a VM that is running, the cloud manager discard that request.
     */
    for (i = 0; (i != setSize) && (maxNumVMsToRequest != 0); i++) {
        size = (((*(vmsToBeSelected.begin() + i))->quantity)
                - maxNumVMsToRequest);
        if (size <= 0) {
            req->setNewSelection(
                    (*(vmsToBeSelected.begin() + i))->type->getType(),
                    (*(vmsToBeSelected.begin() + i))->quantity);

            (*(vmsToBeSelected.begin() + i))->quantity =
                    (*(vmsToBeSelected.begin() + i))->quantity - size;
            reqFilled = true;
        } else {
            req->setNewSelection(
                    (*(vmsToBeSelected.begin() + i))->type->getType(),
                    (*(vmsToBeSelected.begin() + i))->quantity - size);
            reqFilled = true;
            (*(vmsToBeSelected.begin() + i))->quantity =
                    (*(vmsToBeSelected.begin() + i))->quantity
                            - maxNumVMsToRequest;
            break;
        }

    }

    if (!reqFilled)
        req = NULL;

    AbstractRequest* reqA;
    reqA = dynamic_cast<AbstractRequest*>(req);

    return reqA;

}

AbstractRequest* GeneralUser::selectResourcesJob(jobBase* job) {

    // Define ..
    VM* vm;
    Machine* machine;
    vector<VM*> selectedVMs;
    bool found;
    unsigned int i = 0;
    unsigned int j = 0;
    AbstractRequest* aReq;
    RequestVM* reqVM;
    // Init...
    found = false;
    vm = NULL;
    found = false;
    selectedVMs.clear();
    reqVM = new RequestVM();
    cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  selectResourcesJob   &&&&&&&&&&&&&&&&&&&&&&&&&7"<<endl;

    // The behavior of the selection of VMs is to get the first VM in free state.
    for (i = 0; i < machinesMap->size() && (!found); i++) {
        cout<<"machinesMap->size()--->"<<machinesMap->size()<<endl;
        for (j = 0; (j < (unsigned int) machinesMap->getSetQuantity(i)) && (!found);  j++) {
            cout<< "in loop:  "<< j <<"found="<<found <<endl;
            machine = machinesMap->getMachineByIndex(i, j);
            vm = dynamic_cast<VM*>(machine);
            cout<< "vm->getPendingOperation()--->"<<vm->getPendingOperation()<<endl;
            if (vm->getPendingOperation() == NOT_PENDING_OPS) {
                if (!vm->is_freezed){
                    if (vm->getState() == MACHINE_STATE_IDLE) {
                        cout<<"(vm->getVmState() == MACHINE_STATE_IDLE) and NOT_PENDING_OPS"<<endl;

                        //if ((vm->getVmState() == MACHINE_STATE_IDLE) || (vm->getVmState() == MACHINE_STATE_RUNNING)) {
                        selectedVMs.insert(selectedVMs.begin(), vm);
                        cout<<"vselected vm-->"<<vm->getUid()<<":"<<vm->getPid()<<endl;
                        //selectedVMs.insert(selectedVMs.begin(), vm);
                        found = true;

                    }
                }
            }
        }
    }

	//	machine = machinesMap->getMachineByIndex(0,0);
	//	vm = dynamic_cast<VM*>(machine);
	//	selectedVMs.insert(selectedVMs.begin(), vm);
    cout<< "in loop:  "<< j <<"found="<<found <<endl;
    cout<< "selected vms number:  "<< selectedVMs.size() << endl;
    cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&& END OF LOOPS   &&&&&&&&&&&&&&&&&&&&&&&&&7"<<endl;

    reqVM->setVectorVM(selectedVMs);
    aReq = dynamic_cast<AbstractRequest*>(reqVM);

    return aReq;

}

void GeneralUser::requestAttended(AbstractRequest* req) {

    // Define ..
    int exceededVMs;
    int i;
    RequestVM* reqVM;

    // Initialize ..
    i = 0;

    // Begin ..
    if (req->getOperation() == REQUEST_START_VM) {

        reqVM = dynamic_cast<RequestVM*>(req);

        exceededVMs = reqVM->getVMQuantity() - getWQ_size();

        if (isEmpty_WQ()) {
            while (i != reqVM->getVMQuantity()) {
                shutdown_VM(reqVM->getVM(i));
                reqVM->eraseVM(i);
            }

        } else if (exceededVMs > 0) {

            while (exceededVMs != 0) {
                shutdown_VM(reqVM->getVM(reqVM->getVMQuantity() - 1));
                reqVM->eraseVM(reqVM->getVMQuantity() - 1);
                exceededVMs--;
            }
        }

    } else if ((req->getOperation() == REQUEST_LOCAL_STORAGE)
            || (req->getOperation() == REQUEST_REMOTE_STORAGE)) {

        // Only execute the pending jobs that the vms of the request have assigned..
        executePendingJobs();

    } else if (req->getOperation() == REQUEST_FREE_RESOURCES) {

        // User has pending jobs | requests?
        if ((getWQ_size() == 0) && (getRQ_size() == 0)
                && (checkAllVMShutdown())) {
            finalizeUser();
        }
    } else {
        showErrorMessage("Unknown request operation in BasicBehavior: %i",
                req->getOperation());
    }
}

void GeneralUser::requestErrorDeleted(AbstractRequest* req) {

    printf("request deleted: - ");
}

/* --------------------------------------------------------------------
 * ------------------------- Interact with jobs -----------------------
 * --------------------------------------------------------------------
 */

UserJob* GeneralUser::selectJob() {

    // Define ..
    jobBase *job;
    UserJob* jobC;

    // Init ..
    job = NULL;
cout <<" GeneralUser::selectJob()"<<endl;
    //get the job from the cloudManager

    if (!isEmpty_WQ()) {

        job = getJob_WQ_index(0);
        cout <<" GeneralUser::selectJob()--->job="<<job->getFullName()<<endl;
        jobC = dynamic_cast<UserJob*>(job);


    } else {
        cout <<" GeneralUser::selectJob()--->job=null"<<endl;

        jobC = NULL;

    }


    return jobC;
}

void GeneralUser::jobHasFinished(jobBase* job) {

    UserJob* jobC;
    Machine* m;
    cout <<" GeneralUser::jobHasFinished()"<<endl;

    jobC = dynamic_cast<UserJob*>(job);
    m = jobC->getMachine();

    VM* vm = check_and_cast<VM*>(m);

    if (jobC == NULL)
        throw cRuntimeError(
                "GeneralUser::jobHasFinished->job can not be casted to CloudJob\n");

    if ((vm->getNumProcessesRunning() == 1) && (getWQ_size() == 0)) {

        // if the vm is not more needed, request to shutdown it

        shutdown_VM(vm);

    }

}

/* --------------------------------------------------------------------
 * ------------------------- User Behavior ----------------------------
 * --------------------------------------------------------------------
 */

void GeneralUser::schedule() {

    //Enter_Method_Silent();
    Enter_Method("request()");
    // Define ..
   cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&  GeneralUser  schedule  &&&&&&&&&&&&&&&&&&&&&&&&&&&7"<<endl;
  // cout<< "GeneralUser::schedule()"<< endl;
 //  cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&7"<<endl;

    UserJob* job;
    jobBase* jobB;
    AbstractRequest* reqB;
    RequestVM* reqVM;
    vector<VM*> setToExecute;
    unsigned int i;
    int quantityVMFree;
    int waitingQSize;
    vector<RequestVM*> requests_vector_storage;
    vector<RequestVM*>::iterator reqIt;
    bool breakScheduling;
    VM* vm;

    // Init ..

    job = NULL;
    setToExecute.clear();
    requests_vector_storage.clear();
    quantityVMFree = 0;
    waitingQSize = 0;
    breakScheduling = false;
//cout <<"GeneralUser::schedule() ---call from abstract user class" <<endl;
    // Begin the behavior of the user.
    job = selectJob();
cout<<"job---->"<<job<<endl;
    while ((job != NULL) && (!breakScheduling)) {
        cout<<"  while ((job != NULL) && (!breakScheduling))"<<endl;
        cout <<"calling selectResourcesJob from schedule()" <<endl;

        reqB = selectResourcesJob(job);

        reqVM = dynamic_cast<RequestVM*>(reqB);

        cout<<"reqVM->getVMQuantity()--->"<<reqVM->getVMQuantity()<<endl;


        if (reqVM->getVMQuantity() != 0) {

            // Allocate the set of machines where the job is going to execute into the own job

            vm = dynamic_cast<VM*>(reqVM->getVM(0));
cout<<"vm--->"<<vm->getUid()<<":"<<vm->getPid()<<endl;
            job->setMachine(vm);
            jobB = dynamic_cast<jobBase*>(job);

            if ((vm == NULL) || (jobB == NULL)) {
               // throw cRuntimeError(
                printf("GeneralUser::schedule() -> vm or job == NULL\n");
            }

            createFSforJob(job, job->getMachine()->getIP(),
                    vm->getNodeSetName(), vm->getNodeName(), vm->getPid());
            delete (reqVM);

        } else {

            // check if the APP needs more VMs than existent free (to allocate smaller jobs)
            for (i = 0; i != machinesMap->size(); i++) {
                quantityVMFree += machinesMap->countONMachines(i);
            }

            waitingQSize = getWQ_size();

            if ((quantityVMFree > 0) && (waitingQSize >= 1)) {

                // There are not enough VMs to execute the first job.
                // It is moved from the first position to the last to execute other job that needs less resources
                jobB = dynamic_cast<jobBase*>(job);
                eraseJob_FromWQ(job->getJobId());
                pushBack_WQ(jobB);

            }

            breakScheduling = true;

        }

        job = selectJob();
        cout<< "GeneralUser::schedule() ->job->getFullName()-->"<<job->getFullName()<<endl;
        cout<<"--------------------end while -----------------"<<endl;
    }

    if ((job == NULL)) {
        cout<< "GeneralUser::schedule() ->    if ((job == NULL))"<<endl;

        for (i = 0; i < (unsigned int) machinesMap->getMapQuantity(); i++) {
            for (int j = 0; j < (int) machinesMap->getSetQuantity(i); j++) {

                Machine* machine = machinesMap->getMachineByIndex(i, j);
                VM* vm = dynamic_cast<VM*>(machine);

                if ((vm->getPendingOperation() == NOT_PENDING_OPS)
                        && (vm->getState() == MACHINE_STATE_IDLE)
                        && (vm->getNumProcessesRunning() == 0)) {
                    shutdown_VM(vm);
                }
            }
        }

    }
}
