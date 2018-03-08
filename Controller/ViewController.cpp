#include "ViewController.hpp"

/*
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
*/

ViewController::ViewController(const char* path, VectorRing& imuRing) : vecRing(imuRing)
{
  for(int i = 0; i < 8; i++){
    procArgs[i] = NULL;
  }
  
  initialized = false;
  
  strcpy(arg1,"/usr/bin/gnome-terminal");
  strcpy(arg2,"--title=CEREBOT TELEMETRY");
  strcpy(arg3,"--execute");`
  
  strcpy(arg4,"./uiClient");

  procArgs[0] = arg1;
  procArgs[1] = arg2;
  procArgs[2] = arg3;
  procArgs[3] = arg4;

  sockLen = sizeof(sockaddr_un);
  memset(msg,0,1024);
  newData = false;
  strncpy(sockPath,path,127);
  sockPath[127] = '\0';
}

ViewController::~ViewController()
{
  close(sockfd);
}

void ViewController::Test()
{
  string buffer;

  //std::cin.flush();
  while(std::cin >> buffer){
    cout << "sending: " << buffer << endl;
    strncpy(msg,buffer.c_str(),64);
    Send();
    //std::cin.flush();
  }
}

/*
  View controller spins for data, updating ui whenever it detects data arrival
*/
bool ViewController::Run()
{
  bool result = false;

  if(init()){
  
    if(vecRing.HasNewData()){
      UpdateUi();
      vecRing.ClearNewData();
    }
  }

  return result;
}

//sleep, waiting for data to send to ui
void ViewController::run()
{
  if(init()){
    Test();
  }
}

void ViewController::die()
{
  //This probably doesn't apply to the server.
  exit(0);
}

//void dg_cli(FILE* fp, int sockfd, const SA* pcliAddr, socklen_t sockLen)
void ViewController::Send()
{
  cout << "in ViewController.Run() with sockfd " << sockfd << " and sockLen " << sockLen << endl;

  //sleep for new data
  int n = sendto(sockfd,msg,strlen(msg),0,(SA*)&cliAddr,sockLen);
  if(n < 0){
    cout << "ERROR ViewController.Send().sendto() returned -1. Error str: " << strerror(errno) << endl;
  }
  else{
    cout << "sent " << n << " bytes: " << msg << endl;
  }
  sleep(3);
}

/*
  Opens unix domain socket for communicating with client.
*/
bool ViewController::initConnection()
{
  bool result = false;
  
/*
  if(sockfd = socket(AF_LOCAL,SOCK_DGRAM, 0) > 0){
    unlink(sockPath);
    memset((void*)&cliAddr,0,sizeof(cliAddr));
    cliAddr.sun_family = AF_LOCAL;
    strncpy(cliAddr.sun_path,sockPath,100);
    if(bind(sockfd, (SA*)&cliAddr, sizeof(cliAddr)) >= 0){
      result = true;
    }
    else{
      cout << "ERROR bind() failed in ViewController.Init()" << endl;
    }
    //dg_echo(sockfd,(SA*)&srvAddr,sizeof(srvAddr));
  }
  else{
    cout << "ERROR socket() failed in ViewController.Init()" << endl;
  }
*/


  if((sockfd = socket(AF_LOCAL,SOCK_DGRAM, 0)) > 0){
    memset((void*)&cliAddr,0,sizeof(cliAddr));
    cliAddr.sun_family = AF_LOCAL;
    strncpy(cliAddr.sun_path,PATH_TO_CLIENT,100);
    cout << cliAddr.sun_path << "  << the file name" << endl;
    cout << "sizeof srvAddr.sunpath: " << strnlen(cliAddr.sun_path,100) << endl;
    unlink(cliAddr.sun_path);

    if(bind(sockfd, (SA*)&cliAddr, sizeof(cliAddr)) >= 0){
      memset((void*)&srvAddr,0,sizeof(srvAddr));
      srvAddr.sun_family = AF_LOCAL;
      strncpy(srvAddr.sun_path,PATH_TO_SERVER,64);
      result = true;
    }
    else{
      cout << "ERROR bind() failed in ViewController.Init()" << endl;
      cout << "error: " << strerror(errno) << endl;
    }
  }
  else{
    cout << "ERROR socket() failed in ViewController.Init()" << endl;
  }

  return result;
}

bool ViewController::forkClient()
{
  cout << "fork GO!" << endl;

  if((childPid = vfork()) < 0){
    cout << "ERROR ViewController.forkClient fork() error" << endl;
    return false;
  }

  if(childPid == 0){
    //child execs a new terminal which will start the client proc
    execve(procArgs[0],procArgs,environ);
    cout << "ERROR ViewController.forkClient exec() error" << endl;
    exit(0);
  }

  return true;
}

//comms set up
bool ViewController::init()
{
  int result = false;

  //if(environ != NULL){
    //open comms
    if(initConnection()){
      //create and detach the client terminal process
      if(forkClient()){
        cout << "init successful" << endl;
        initialized = true;
        result = true;
      }
      else{
        cout << "ERROR forkClient() failed in ViewController.Init()" << endl;
      }
    }
    else{
      cout << "ERROR socket() failed in ViewController.Init()" << endl;
    }
  //}

  return result;
}

/*
  Unix domain datagram client and server implementation.
  Direct from Stevens' Network Programming, Vol1, pg 419.
bool ViewController::Init()
{
  bool result = false;

  if(sockfd = socket(AF_LOCAL,SOCK_DGRAM, 0) > 0){
    memset((void*)&srvAddr,0,sizeof(srvAddr));
    srvAddr.sun_family = AF_LOCAL;
    strncpy(srvAddr.sun_path,tmpnam(NULL),100);
    cout << srvAddr.sun_path << "  << the file name" << endl;
    cout << "sizeof srvAddr.sunpath: " << strnlen(srvAddr.sun_path,64) << endl;

    if(bind(sockfd, (SA*)&srvAddr, sizeof(srvAddr)) >= 0){
      memset((void*)&cliAddr,0,sizeof(cliAddr));
      cliAddr.sun_family = AF_LOCAL;
      strncpy(cliAddr.sun_path,PATH_TO_CLIENT,64);
      result = true;
    }
    else{
      cout << "ERROR bind() failed in ViewController.Init()" << endl;
    }
  }
  else{
    cout << "ERROR socket() failed in ViewController.Init()" << endl;
  }

    //dg_cli(stdin,sockfd,(SA*)&cliAddr,sizeof(cliAddr));

  return result;
}
*/

/*
int main(int argc, char* argv[])
{
  ViewController server(PATH_TO_CLIENT);

  server.Test();

  return 0;
}
*/
