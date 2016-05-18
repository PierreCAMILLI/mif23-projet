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
    Vector offset;
    Vector scale;
    Vector rot;

    // Méthodes
    membre();

    void translate(float x, float y, float z);
    float getParentsX();
    float getParentsY();
    float getParentsZ();
    Transform getParentsTranslation();
    Transform getTranslation();

    void rescale(float x, float y, float z);
    Transform getScale();

    void setOffset(float x, float y, float z);
    float getParentsOffsetX();
    float getParentsOffsetY();
    float getParentsOffsetZ();
    Transform getOffset();

    void rotateX(float angle);
    float getParentsRotationX();
    Transform getRotationX();

    void rotateY(float angle);
    float getParentsRotationY();
    Transform getRotationY();

    void rotateZ(float angle);
    float getParentsRotationZ();
    Transform getRotationZ();

    void rotateXYZ(float angleX, float angleY, float angleZ);
    Transform getRotation();

    Transform getTransformation();

    virtual ~membre();

  protected:

  private:

};

#endif // MEMBRE_H
