#include "Comm.hpp"

Comm::Comm(char* devAddr, char* devicePath, VectorRing& ring)
{
  vecBuffer = ring;
  if(devAddr){
    strncpy(devAddress,devAddr,63);
  }
  devAddress[63] = '\0';
  if(devicePath){
    strncpy(devPath,bound_path,63);
  }
  devPath[63] = '\0';

  memset(&imuVec,0,sizeof(IMU));
}

~Comm::Comm()
{
  close(commfd);
  cout << "Comm exiting..." << endl;
}

bool Comm::readData()
{
  //for compressed packet transmission
  int n;

  n = read(commfd,buf,1023);
  if(n >= 0){
    buf[n] = '\0';
    cout << "read " << n << " bytes from cerebot: " << buf << endl;
  }
  else{
    cout << "ERROR read returned " << n << " in Comm.ReadCerebotData() on error: " << strerror(errno) << endl;
    return false;
  }

  DeserializeImuVector((U8*)buf,&imuVec);
  //cout << "vec accX: " << vecRing[0].accX << endl;
  //memset((void*)buf,0,64);
  vecRing.Push(imuVec);
  //tcflush(commfd, TCIFLUSH);  //flushes all data not read

  return true;
}

bool Comm::Run()
{
  if(initComms()){
    while(!error){
      error = readData();
    }
  }
  else{
    cout << "ERROR Comm object could not init comms" << endl;
  }

  return false;
}

//later
void Comm::SendData(U8* data)
{
  //int n;

  comms.SendData(data);
  //n = write(commfd,buf,strnlen(buf,64));
  //cout << "wrote " << n << " bytes: " << buf << endl;
}

int Comm::getVec(IMU* vec, char buf[256]) //require large string size
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

/*
  Alternative to the terminal modifications made in old initCerebotComms, which
  for some reason didn't work in the multithreaded model. This is much
  simpler, but opening the file this way strips newlines and other non-text
  data, it appears, when reading from the rfcomm file descriptor.
*/
bool Comm::initComms()
{
  bool success = true;

  commfd = open(bt_path,O_RDONLY);
  if(commfd < 0){
    cout << "open() returned error " << errno << ": " << strerror(errno) << endl;
    cout << "(does " << BT_DEVICE << " exist? did you run sudo rfcomm bind 0 [btaddr]? did you run main prog as `sudo ./cerebot`?)" << endl;
    success = false;
  }

  return success;
}



















//take a uart string and puts back into buf
//it has been very time consuming to get more compressed comms to work, so use this. its just easier for now.
//maps discrete device strings, instead of the mega string containing all 
/*
int getVec(IMU* v, char buf[])
{
  int c;  //dummy
  int success = 1;

  switch(buf[0]){
    case 'A':
        sscanf(buf,"%x %x %x %x", &c, (int*)&vec->accX, (int*)&vec->accY, (int*)&vec->accZ);
      break;
    case 'M':
        sscanf(buf,"%x %x %x %x", &c, (int*)&vec->magX, (int*)&vec->magY, (int*)&vec->magZ);
        //vec->T = -1; vec->B = -1;
      break;
    case 'G':
        sscanf(buf,"%x %x %x %x %x", &c, (int*)&vec->gyroRoll, (int*)&vec->gyroPitch, (int*)&vec->gyroYaw, (int*)&vec->gyroTemp);
      break;
    case 'B':
        sscanf(buf,"%x %x %x",&c, (int*)&vec->bmpPa, (int*)&vec->bmpTemp);
        //vec->T = -1; vec->B = -1;
      break;
    default:
        printf("slave str found: >%s<\n", buf);
        success = 0;
      break;
  }

  return success;
}
*/


