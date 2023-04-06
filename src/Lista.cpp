#include "Lista.h"

Lista::Lista() : _primer(NULL), _ultimo(NULL)
{

}

int indicexcs = 0;  // Variable global que lleva la cuenta de la cantidad de elementos en la lista

void Lista::agregar(Elemento* elemento)
{
    // Si la lista no está vacía, el último elemento apunta al nuevo elemento a agregar
    // y se actualiza el puntero _ultimo
    if (_primer) {
        _ultimo->setSiguiente(elemento);
        _ultimo = elemento;
    }
    else {  // Si la lista está vacía, el nuevo elemento es tanto el primer como el último elemento
        _primer = elemento;
        _ultimo = elemento;
    }
    indicexcs++;  // Se incrementa la cantidad de elementos en la lista
}

bool Lista::eliminar(int indice)
{
    // Si el índice es mayor o igual a la cantidad de elementos en la lista o es un índice negativo
    // entonces el índice no está contenido en la lista y se retorna falso
    if (indice >= indicexcs || indice < 0) {
        return false;
    }
    else {
        Elemento* aux1, * aux2;  // Punteros auxiliares para recorrer la lista
        aux1 = _primer;

        // Si el índice no es cero, se recorre la lista hasta llegar al elemento anterior al que se quiere eliminar
        for (int i = 0; i < indice - 1; i++) {
            aux1 = aux1->getSiguiente();
        }

        aux2 = aux1->getSiguiente();  // Se guarda el puntero al elemento que se quiere eliminar
        aux1->setSiguiente(aux2->getSiguiente());  // Se actualiza el puntero del elemento anterior
        delete aux2;  // Se elimina el elemento que se quiere eliminar

        // Si se eliminó el primer elemento, se actualiza el puntero _primer
        if (indice == 0) {
            _primer = _primer->getSiguiente();
        }

        indicexcs--;  // Se decrementa la cantidad de elementos en la lista
        return true;
    }
}
