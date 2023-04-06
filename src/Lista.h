#ifndef LISTA_H
#define LISTA_H

#include "Elemento.h"

class Lista
{
    public:
        Lista();
        void agregar(Elemento* elemento);
        Elemento* getPrimer() { return _primer; }
        bool eliminar(int indice);

    private:
        Elemento* _primer;
        Elemento* _ultimo;
};
#endif
