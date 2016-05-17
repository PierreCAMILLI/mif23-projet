#include "membre.h"

membre::membre()
{
    this->obj = new Mesh();
    this->parent = NULL;
    x = 0;  y = 0;  z = 0;
    offsetX = 0; offsetY = 0; offsetZ = 0;
    scaleX = 1;  scaleY = 1;  scaleZ = 1;
    rotX = 0; rotY = 0; rotZ = 0;
    angle = 0;
}

void membre::translate(float x, float y, float z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}

float membre::getParentsX()
{
    return this->x + (this->parent != NULL ? this->parent->getParentsX() : 0);
}

float membre::getParentsY()
{
    return this->y + (this->parent != NULL ? this->parent->getParentsY() : 0);
}

float membre::getParentsZ()
{
    return this->z + (this->parent != NULL ? this->parent->getParentsZ() : 0);
}

Transform membre::getParentsTranslation()
{
    return make_translation(getParentsX()-x, getParentsY()-y, getParentsZ()-z);
}

Transform membre::getTranslation()
{
    return make_translation(getParentsX(), getParentsY(), getParentsZ());
}

float membre::getParentsOffsetX()
{
    return this->offsetX + (this->parent != NULL ? this->parent->getParentsOffsetX() : 0);
}

float membre::getParentsOffsetY()
{
    return this->offsetY + (this->parent != NULL ? this->parent->getParentsOffsetY() : 0);
}

float membre::getParentsOffsetZ()
{
    return this->offsetZ + (this->parent != NULL ? this->parent->getParentsOffsetZ() : 0);
}

void membre::setOffset(float x, float y, float z){
    this->offsetX += x;
    this->offsetY += y;
    this->offsetZ += z;
}

Transform membre::getOffset(){
    return make_translation(getParentsOffsetX(), getParentsOffsetY(), getParentsOffsetZ());
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

float membre::getParentsRotationX()
{
    return this->rotX + (this->parent != NULL ? this->parent->getParentsRotationX() : 0);
}

Transform membre::getRotationX()
{
    return make_rotationX(getParentsRotationX());
}

void membre::rotateY(float angle)
{
    this->rotY = angle;
}

float membre::getParentsRotationY()
{
    return this->rotY + (this->parent != NULL ? this->parent->getParentsRotationY() : 0);
}

Transform membre::getRotationY()
{
    return make_rotationY(getParentsRotationY());
}

void membre::rotateZ(float angle)
{
    this->rotZ = angle;
}

float membre::getParentsRotationZ()
{
    return this->rotZ + (this->parent != NULL ? this->parent->getParentsRotationZ() : 0);
}

Transform membre::getRotationZ()
{
    return make_rotationZ(getParentsRotationZ());
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
    return getOffset() * getRotation() * getTranslation() * getScale();
    //return (this->parent != NULL ? getParentsTranslation() : make_identity()) * getRotation() * getTranslation() * getScale();
}

membre::~membre()
{
    delete[] this->obj;
    delete[] this->parent;
}
