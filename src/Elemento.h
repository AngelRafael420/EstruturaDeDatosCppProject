#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <string>
#include "../motor_grafico/Mesh.h"
class Elemento
{
    public:
        Elemento(Mesh *modelo);
        void setModelo(Mesh* modelo) { _modelo = modelo; }
        Mesh* getModelo(){ return _modelo; }
        void setSiguiente(Elemento* siguiente) { _siguiente = siguiente; }
        Elemento* getSiguiente() { return _siguiente; }

    private:
        Mesh* _modelo;
        Elemento* _siguiente;  
};
#endif
