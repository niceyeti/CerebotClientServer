#include <vector>

using std::vector;

/*
  Small multithreaded deq for IMU vector data.

  This is so the comm objects can simply write to the vecDeq
  as it pleases, while we pull data. The goal is to write this
  object to support multithreading, but without using mutexes.

  For now, this is just done by making sure begin points one ahead of
  the first publicly available vector.

  TODO: none of the multithreading protections are well-defined.
*/

class VectorRing
{
  public:
    VectorRing() = delete;
    VectorRing(int maxItems);
    ~VectorRing();
    bool Push(IMU& imuVec);
    int Begin();
    int End();
    bool GetAt(int i, IMU& outVec);
    void Peek(IMU& imuVec); //gets latest vector
    bool HasNewData();
    void ClearNewData();
    void CopyVec(const IMU& src,IMU& dest)

  private:
    bool newData;
    void advance();
    vector<IMU> imuRing;
    int begin;
    int _begin; //always one ahead of begin
    int end;
    const int maxSize;
}
