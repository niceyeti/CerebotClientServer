#include "common.hpp"
#include "../../encoding/encoding.h"

/* needs encoder
  Comm may itself need to become multithreaded, maintaining a read and a write
  thread for multiplexing communications.

  The Comm object also needs access to some data structure to write vector
  data to; in a synchronous manner, its easy to pass a vector parameter,
  but this throttles comms. If the comm object is to read asynchronously
  in a polling loop, then it needs access to a vector data structure object
  that implements locks/mutexes, since other objects will be accessing it...
  or just use a queue or a non-mutex ring buffer like in SW testing.

  Anyway the expected behavior for now is just to have the Comm process
  read forever. The cerebot client just calls Run(), and the comm process
  continually writes data to some hideous global data structure.
  This object will probably change a lot, but try to keep it generic.

  Additionally, this should remain a highlevel interface for now, from which
  other Comm objects (for different comm types) could inherit.
*/
class Comm
{
  public:
    Comm(char* devAddr, char* devicePath, VectorRing& ring);
    ~Comm();
    Comm() = delete;
    //void SendData(U8* data);
    void Run(); //spawns and kick off comm thread; comm thread will call internal run() method
    void Die();  //kill comm thread and die
    //void ReConnect();

  private:
    void initComms();
    bool readData();
    IMU imuVec;
    VectorRing& vecRing;

    int commfd;
    bool error;
    char devAddress[64];
    char devPath[64];
    char buf[1024];
}
