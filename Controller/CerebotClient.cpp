#include "CerebotClient.hpp"
//comms(char* devAddr, char* devicePath, VectorRing& ring)
CerebotClient::CerebotClient(const char* devAddress, const char* devPath, const char* srvPath) : comms(devAddress,devPath,vecRing), view(srvPath,vecRing), viewThread(view.Run), commThread(comms.Run)
{
  error = false;
  memset((void*)buf,0,1024);
  memset((void*)btAddr,0,64);
  memset(&oldtio,0,sizeof(struct termios));
  memset(&newtio,0,sizeof(struct termios));

  imuSize = sizeof(IMU);
  strncpy(btAddr,btAddress,64);
  vecRing.resize(10);
  for(int i = 0; i < 10; i++){
    memset((void*)&vecRing[i],0,sizeof(IMU));
  }
}

CerebotClient::~CerebotClient()
{
  vecRing.clear();
}

bool CerebotClient::init()
{
  return true;
}
/*
bool CerebotClient::init()
{
  //init
  if(view.Run()){
    if(comms.Run()){
      //more???
      return true;
    }
    else{
      cout << "View.Run() fail" << endl;      
    }
  }
  else{
    cout << "View.Run() fail" << endl;
  }

  return false;
}
*/

void CerebotClient::Run()
{
  //init
  if(init()){
    run();
  }
  cout << "ERROR exiting from cerebotClient.Run()" << endl;
}

void CerebotClient::run()
{
  cout << "Welcome to cerebot client." << endl;
  string nothing;
  
  //simple main loop: read comms, update view
  while(!error){
    cout << "Enter some data to send: " << endl;
    cin >> nothing;
  }
}

/*later
void CerebotClient::SendData(U8* data)
{
  int n;

  cout << "send() not implemented" << endl;
  //comms.SendData(data);
  //n = write(btfd,buf,strnlen(buf,64));
  //cout << "wrote " << n << " bytes: " << buf << endl;
}


int CerebotClient::getVec(IMU* vec, char buf[256]) //require large string size
{
  char c;

  if(buf[0] == '$'){
    printf("buffer: %s\n",buf);
    sscanf(buf,"%c %d %d %d %d %d %d %d %d %d %d %d %d ", &c, (int*)&vec->accX, (int*)&vec->accY, (int*)&vec->accZ,(int*)&vec->gyroRoll, (int*)&vec->gyroPitch, (int*)&vec->gyroYaw, (int*)&vec->gyroTemp,(int*)&vec->magX, (int*)&vec->magY, (int*)&vec->magZ,(int*)&vec->bmpPa, (int*)&vec->bmpTemp);
  }
  else{
    printf("str not found: >%s<\n",buf);
  }

  return 1;
}
*/

void CerebotClient::Test()
{
  //string testStr;

  if(!initCerebotComms()){
    cout << "ERROR could not init comms in CerebotClient.Test()" << endl;
    return;
  }
  cout << "cerebot client running..." << endl;

  while(1){
    ReadCerebotData();
    putImu(&vecRing[0]);

    //cout << "enter some text to send to ui:" << endl;
    //std::cin >> testStr;
    //strncpy(buf,testStr.c_str(),64);
    //SendUiData();
  }
}

int main(void)
{
  CerebotClient client("00:06:66:43:96:73",PATH_TO_CLIENT);
  client.Run();

  return 0;
}



