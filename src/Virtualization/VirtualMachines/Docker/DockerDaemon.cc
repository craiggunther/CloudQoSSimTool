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

#include "DockerDaemon.h"
#include <iostream>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

Define_Module(DockerDaemon);

DockerDaemon::DockerDaemon() {
    // TODO Auto-generated constructor stu
    containerSet.clear();
    cout << "DockerDaemon::DockerDaemon()cont"<<endl;
}
void DockerDaemon::initialize(VM* vmp){
vm =vmp;


}
DockerDaemon::~DockerDaemon() {
    // TODO Auto-generated destructor stub
    containerSet.clear();

}
DockerContainer* DockerDaemon::startDockerContainer(string image,string VMfullName)
{
    cout<< "DockerDaemon::startDockercontainer" <<endl;
    DockerContainer* dockerContainer;
    dockerContainer=new DockerContainer();
    string id,name;
    stringstream ss;
    int size;
    size=128; //MB


    if (containerSet.empty())
    {
        id='0';
        cout<< "DockerDaemon::RunDocker------->containerSet.empty()" <<endl;

    }
    else
    {
        int i;
        i= containerSet.size();
        ss << i;
        id=ss.str();
        cout<< "DockerDaemon::RunDocker------->containerSet is not empty()" <<endl;
        cout<<"id--->"<< id;

    }
    cout<< "docker id--------->"<<id<<endl;
    name="docker";
    cout<< "docker name--------->"<<name<<endl;

    dockerContainer->initialize( image,  name,  id, size, VMfullName);
    containerSet.push_back(dockerContainer);
    GetMem(size);
 return dockerContainer;
}
void DockerDaemon::GetMem(double size)
{
    cout<< vm->getFullName()<<endl;
double tmp = vm->getFreeMemory();

cout << "DockerDaemon::GetMem---->free memory before:   " << vm->getFreeMemory();
cout << "DockerDaemon::GetMem---->memory size:   " << size;

vm->setFreeMemory(tmp-size);

cout << "DockerDaemon::GetMem---->free memory after :   " << vm->getFreeMemory();
}

void DockerDaemon::FreeMem(double size)

{
    cout << "DockerDaemon::FreeMem---->free memory before:   " << vm->getFreeMemory();
    cout << "DockerDaemon::FreeMem---->memory size:   " << size;

    double tmp = vm->getFreeMemory();

    vm->setFreeMemory(tmp+size);
    cout << "DockerDaemon::FreeMem---->free memory after :   " << vm->getFreeMemory();

}
void DockerDaemon::processSelfMessage (cMessage *msg){

}

void DockerDaemon::processRequestMessage (icancloud_Message *sm){

}



void  DockerDaemon::processResponseMessage (icancloud_Message *sm){

}
void DockerDaemon::KillDocker(string id){
    bool found=false;
    for(unsigned int i=0; i< containerSet.size() and found==false;++i)
    {
        if (containerSet.at(i)->id==id)
        {
            FreeMem(containerSet.at(i)->size);
            containerSet.erase(containerSet.begin()+i);
            found=true;
        }
    }

}
void DockerDaemon::stopDockerContainer(string id)
{
    freeContainerResources(id);

   // save staus

    KillDocker(id);
}
void DockerDaemon::freeContainerResources(string id)
{
    // disconnect from network
}

void DockerDaemon::pauseDockerContainer (string id)
    {
    cout <<"DockerDaemon::PauseDockerContainer " <<endl;
    DockerContainer *dc;
    dc=getDockerById(id);
 if (dc==NULL) {
     throw cRuntimeError("DockerDaemon::can not found this container in this vm...\n");
 }else {

   double mem =  dc->getMemSize();
   FreeMem(mem);
 }

    }
void DockerDaemon::unPauseDockerContainer (string id)
    {
    cout <<"DockerDaemon::unPauseDockerContainer " <<endl;
    DockerContainer *dc = getDockerById(id);
     if (dc==NULL) {
         throw cRuntimeError("DockerDaemon::can not found this container in this vm...\n");
     }else {

         double mem =  dc->getMemSize();
         GetMem(mem);
     }
    }
void DockerDaemon::getDockerByName(string name)
    {
        bool found=false;
        for(unsigned int i=0; i< containerSet.size() and found==false;++i)
           {

               if (containerSet.at(i))
               {
                   FreeMem(containerSet.at(i)->size);
                   containerSet.erase(containerSet.begin()+i);
                   found=true;
               }
           }

    }
void DockerDaemon::getDockerByImage(string image)
    {

    }
DockerContainer* DockerDaemon::getDockerById(string id)
    {

    DockerContainer* dc ;
cout << "DockerDaemon::getDockerById containerSet size " <<containerSet.size() <<endl;
cout << "DockerDaemon::getDockerById id " << id <<endl;
    bool found=false;
          for(unsigned int i=0; i< containerSet.size() && found==false;i++)
             {
cout << "DockerDaemon::getDockerById -->" << containerSet.at(i)->getContainerId() <<endl;
                 if (containerSet.at(i)->getContainerId() ==id)
                 {
                     cout<< "DockerDaemon::getDockerById found" <<endl;
                     //FreeMem(containerSet.at(i)->size);
                     containerSet.erase(containerSet.begin()+i);
                     dc = containerSet.at(i);
                     found=true;
                 }
             }
if (found)
    return dc;
else{
    return NULL;
}
    }
void DockerDaemon::connectNetwork(string id)
    {

    }
void DockerDaemon::disconnectNetwork(string id)
    {

    }

string DockerDaemon::getContianerId(DockerContainer * dc){

     return dc->id;
}
