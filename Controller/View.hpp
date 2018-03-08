


// was UiClient
class View
{
  public:
    ViewClient(const char* path, float updateRate);
    ViewClient() = delete; // c++ 11 allows expressly forbidding default constructor usage
    ~ViewClient();
    void Run();

  private:
    //utils
    bool init();
    void readData();
    void updateUi();
    void display();
    void die();
    //void putImu(const IMU& vec);

    int sockfd;
    socklen_t sockLen;
    char sockPath[256];
    char buf[1024];
    char uiString[1024];
    string uiString;
    struct sockaddr_un srvAddr, cliAddr;
};

