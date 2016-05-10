#ifndef MEMBRE_H
#define MEMBRE_H
#include "mesh.h"

class membre
{
  public:
    Mesh* obj;
    membre* fils;
    membre();
    virtual ~membre();
  protected:
  private:
};

#endif // MEMBRE_H
