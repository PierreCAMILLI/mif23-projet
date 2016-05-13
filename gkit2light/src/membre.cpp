#include "membre.h"

membre::membre()
{
    this->obj = new Mesh();
    this->parent = NULL;
    x = 0;  y = 0;  z = 0;
    scaleX = 1;  scaleY = 1;  scaleZ = 1;
    rotX = 0; rotY = 0; rotZ = 0;
}

void membre::translate(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Transform membre::getTranslation()
{
    return make_translation(x, y, z);
}

void membre::rescale(float x, float y, float z)
{
    this->scaleX = x;
    this->scaleY = y;
    this->scaleZ = z;
}

Transform membre::getScale()
{
    return make_scale(scaleX, scaleY, scaleZ);
}

void membre::rotateX(float angle)
{
    this->rotX = angle;
}

Transform membre::getRotationX()
{
    return make_rotationX(rotX);
}

void membre::rotateY(float angle)
{
    this->rotY = angle;
}

Transform membre::getRotationY()
{
    return make_rotationY(rotY);
}

void membre::rotateZ(float angle)
{
    this->rotZ = angle;
}

Transform membre::getRotationZ()
{
    return make_rotationZ(rotZ);
}

void membre::rotateXYZ(float angleX, float angleY, float angleZ)
{
    rotateX(angleX);
    rotateY(angleY);
    rotateZ(angleZ);
}

Transform membre::getRotation()
{
    return getRotationX() * getRotationY() * getRotationZ();
}

Transform membre::getTransformation()
{
    return getTranslation() * getScale() * getRotation();
}

membre::~membre()
{
  delete[] this->obj;
  delete[] this->parent;
}
