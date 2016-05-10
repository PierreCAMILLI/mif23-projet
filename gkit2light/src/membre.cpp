#include "membre.h"

membre::membre()
{
  this->obj = new Mesh();
  this->fils = NULL;
}

membre::~membre()
{
  delete[] this->obj;
  delete[] this->fils;
}
