#ifndef NODO_GRAFO_H
#define NODO_GRAFO_H

#endif // NODO_GRAFO_H
#ifndef NODO_GRAFO
#define NODO_GRAFO
template<class V,class A>
class Nodo_Grafo
{
    public:

        V vertice;
        A arista;

        Nodo_Grafo(){}
        Nodo_Grafo(V vertice,A arista);
        bool operator>(const Nodo_Grafo<V,A>& nodo){return this->arista > nodo.arista;}
        bool operator<(const Nodo_Grafo<V,A>& nodo){return this->arista < nodo.arista;}
        bool operator>=(const Nodo_Grafo<V,A>& nodo){return this->arista >= nodo.arista;}
        bool operator<=(const Nodo_Grafo<V,A>& nodo){return this->arista <= nodo.arista;}
        bool operator==(const Nodo_Grafo<V,A>& nodo){return this->arista == nodo.arista;}
        Nodo_Grafo<V,A>& operator=(const Nodo_Grafo<V,A>& nodo);


};

template<class V,class A>
Nodo_Grafo<V,A>& Nodo_Grafo<V,A>::operator=(const  Nodo_Grafo<V,A>& nodo)
{
    this->vertice=nodo.vertice;
    this->arista=nodo.arista;

    return *this;
}



template<class V,class A>
Nodo_Grafo<V,A>::Nodo_Grafo(V vertice,A arista)
{
    this->vertice=vertice;
    this->arista=arista;


}

#endif // NODO_GRAFO

