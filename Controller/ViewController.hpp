
// was UiServer
class ViewController{
  public:
    ViewController(const char* path);
    ViewController() = delete;
    ~ViewController();
    //void Send();
    void Test();
    void Run();
    //void SetBuffer(const char* buf);

  private:
    //utils
    //thread viewControllerThread;  //init within ctor
    bool newData;
    void die();
    bool initConnection();
    bool forkClient();
    bool init();

    VectorRing& vecRing;
    char* procArgs[8];
    char arg1[64], arg2[64], arg3[64], arg4[64], arg5[64];
    int sockfd;
    socklen_t sockLen;
    bool initialized;
    char sockPath[128];
    char msg[1024];
    struct sockaddr_un srvAddr, cliAddr;  //only one of these is really needed, since comms will be unidirectional
};
