#include "membre.h"

membre::membre()
{
    this->obj = new Mesh();
    this->parent = NULL;
    pos.x = 0;  pos.y = 0;  pos.z = 0;
    offset.x = 0; offset.y = 0; offset.z = 0;
    scale.x = 1;  scale.y = 1;  scale.z = 1;
    rot.x = 0; rot.y = 0; rot.z = 0;
}

void membre::translate(float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

float membre::getParentsX()
{
    return this->pos.x + (this->parent != NULL ? this->parent->getParentsX() : 0);
}

float membre::getParentsY()
{
    return this->pos.y + (this->parent != NULL ? this->parent->getParentsY() : 0);
}

float membre::getParentsZ()
{
    return this->pos.z + (this->parent != NULL ? this->parent->getParentsZ() : 0);
}

Transform membre::getParentsTranslation()
{
    return make_translation(getParentsX()-pos.x, getParentsY()-pos.y, getParentsZ()-pos.z);
}

Transform membre::getTranslation()
{
    //return make_translation(getParentsX(), getParentsY(), getParentsZ());
    return make_translation(pos.x, pos.y, pos.z);
}

float membre::getParentsOffsetX()
{
    return this->offset.x + (this->parent != NULL ? this->parent->getParentsOffsetX() : 0);
}

float membre::getParentsOffsetY()
{
    return this->offset.y + (this->parent != NULL ? this->parent->getParentsOffsetY() : 0);
}

float membre::getParentsOffsetZ()
{
    return this->offset.z + (this->parent != NULL ? this->parent->getParentsOffsetZ() : 0);
}

void membre::setOffset(float x, float y, float z){
    this->offset.x += x;
    this->offset.y += y;
    this->offset.z += z;
}

Transform membre::getOffset(){
    return make_translation(getParentsOffsetX(), getParentsOffsetY(), getParentsOffsetZ());
}

void membre::rescale(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
}

Transform membre::getScale()
{
    return make_scale(scale.x, scale.y, scale.z);
}

void membre::rotateX(float angle)
{
    this->rot.x = angle;
}

float membre::getParentsRotationX()
{
    return this->rot.x + (this->parent != NULL ? this->parent->getParentsRotationX() : 0);
}

Transform membre::getRotationX()
{
    //return make_rotationX(getParentsRotationX());
    return make_rotationX(rot.x);
}

void membre::rotateY(float angle)
{
    this->rot.y = angle;
}

float membre::getParentsRotationY()
{
    return this->rot.y + (this->parent != NULL ? this->parent->getParentsRotationY() : 0);
}

Transform membre::getRotationY()
{
    //return make_rotationY(getParentsRotationY());
    return make_rotationY(rot.y);
}

void membre::rotateZ(float angle)
{
    this->rot.z = angle;
}

float membre::getParentsRotationZ()
{
    return this->rot.z + (this->parent != NULL ? this->parent->getParentsRotationZ() : 0);
}

Transform membre::getRotationZ()
{
    //return make_rotationZ(getParentsRotationZ());
    return make_rotationZ(rot.z);
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
    return (this->parent != NULL ? this->parent->getTransformation() : make_identity()) * getTranslation() * getRotation();
    //return (this->parent != NULL ? getParentsTranslation() : make_identity()) * getRotation() * getTranslation() * getScale();
}

membre::~membre()
{
    delete[] this->obj;
    delete[] this->parent;
}
