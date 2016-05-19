#ifndef MEMBRE_H
#define MEMBRE_H
#include "vec.h"
#include "mesh.h"

class membre
{
  public:
    // Attributs
    Mesh* obj;

    membre* parent;

    Vector pos;
    Vector scale;
    Vector rot;

    // Méthodes
    membre();

    void translate(float x, float y, float z);
    Transform getParentsTranslation();
    Transform getTranslation();

    void rescale(float x, float y, float z);
    Transform getScale();

    void rotateX(float angle);
    Transform getRotationX();

    void rotateY(float angle);
    Transform getRotationY();

    void rotateZ(float angle);
    Transform getRotationZ();

    void rotateXYZ(float angleX, float angleY, float angleZ);
    Transform getRotation();

    Transform getTransformation();

    virtual ~membre();

  protected:

  private:

};

#endif // MEMBRE_H
