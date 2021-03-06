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


#ifndef DockerDaemon_H_
#define DockerDaemon_H_

#include <omnetpp.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdexcept>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include "ICCLog.h"
#include "Memoization_uthash.h"
#include "DockerContainer.h"
#include "icancloud_Message.h"
#include "VM.h"

class DockerDaemon : public cSimpleModule{
public:
    VM* vm ; //host vm
    DockerDaemon();
    void initialize(VM* vm);
    ~DockerDaemon();
    vector<DockerContainer *> containerSet;
    DockerContainer* startDockerContainer(string image,string VMfullName);

    /**
           * Process a self message.
           * @param msg Self message.
           */
   void processSelfMessage (cMessage *msg);

          /**
           * Process a request message.
           * @param sm Request message.
           */
   void processRequestMessage (icancloud_Message *sm);
           /**
                 * Process a response message.
                 * @param sm Request message.
                 */
  void processResponseMessage (icancloud_Message *sm);

  /**
                 * Process a response message.
                 * @param sm Request message.
                 */
    void GetMem(double size);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void FreeMem(double size);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void KillDocker(string id);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void freeContainerResources(string id);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void stopDockerContainer (string id);
    /**
                   * pause container by given id
                   * @param id container id
                   * output : none
                   */
    void pauseDockerContainer (string id);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void unPauseDockerContainer (string id);
    /**
                   * Process a response message.
                   * @param sm Request message.
                   */
    void getDockerByName(string name);
    /**
                   * Process a response messa
                   * @param image name
                   */
    void getDockerByImage(string image);
    /**
                   * Given a dockercontainer id and return docker container instance
                   * @param id docker container id.
                   * output : return dockercontainer instance
                   */
    DockerContainer* getDockerById(string id);
    /**
                   * Given a docker container and return this container's id
                   * @param dc instance of dockercontainer
                   * output : container ID
                   *
                   */
    string getContianerId(DockerContainer * dc);




    void connectNetwork(string id);
    void disconnectNetwork(string id);


};

#endif /* DockerDaemon_H_ */

