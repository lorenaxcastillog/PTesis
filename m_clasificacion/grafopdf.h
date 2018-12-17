#ifndef GRAFOPDF
#define GRAFOPDF
#include<fstream>
#include<unordered_map>
#include<queue>
#include<set>
#include<iostream>
#include<sstream>
#include <string.h>
#include<vector>
#include <ctype.h>
#include"porter2_stemmer.h"
#include<stdlib.h>
#include<stdio.h>
#include<map>
#include"nodo_grafo.h"
#include"Dictionary.h"
///porter espaniol
#include "stemming/spanish_stem.h"

using namespace std;

typedef string id;

string path_file="baby/";
class Grafopdf{

    public:
        float grade;
        string file;
        float size=0;
        Dictionary *dictionary;
        unordered_map<id,unordered_map<id,float>> grafo;
        unordered_map<float,id> list_words;
        unordered_map<string,bool> list_sw;
        unordered_map<id,float> result;

        map<float,unordered_map<id,bool>> keywords;

        unordered_map<id,bool> Vis;

        Grafopdf(){}
        Grafopdf(string file, float grade, Dictionary &dictionary);
        void create_graf();
        void clear_text();
        void load_stop_words();
        bool stop_word(string word);
        void minus(string &word);
        void draw_dot();
        double compare(Grafopdf *&other, int num_keywords, float radio);
        void get_keywords();
        void insert_ady_dijstra(id origen, float peso_acumulado);
        void Dijkstra(id origen,float radio);
        struct cmp{ bool operator()( const Nodo_Grafo<id,float>&a,const Nodo_Grafo<id,float> &b)
        { return a.arista>b.arista; } };
        priority_queue< Nodo_Grafo<id,float>, vector<Nodo_Grafo<id,float>>,cmp> heap_nodos;
        priority_queue< Nodo_Grafo<id,float>, vector<Nodo_Grafo<id,float>>,cmp> heap_nodos_k_menores;

        map<string,int> map_words;///variable para obtener un map con palabras y sus repeticiones en el texto para los experimentos
        void get_vector_map_texto(string tex); ///funcion para obtener un map con palabras y sus repeticiones en el texto para los experimentos

};

void remplazar (string &letra);
void Grafopdf::get_vector_map_texto( string tex)
{
    string  line;
    //ifstream file_read("text_reuters/textos/"+this->file+".txt");
    ifstream file_read( tex+".txt");
    float cont=0;
    this->load_stop_words();

    while(!file_read.eof())
    {
        getline(file_read,line);
        remplazar(line);
        minus(line);//elimino mayusculas
        stringstream list_str(line);
        string word;
        while(list_str >> word)
        {
            if(!stop_word(word))
            {
                Porter2Stemmer::stem(word); //porter ingles

                this->map_words[word]++;/////cambiooo id 1

                cont++;
             }
        }//cout<<endl;

    }

}


void remplazar (string &letra)
{
      char con[] = {'Ã','á','à','ä','é','è','ë','í','ì','ï','ó','ò','ö','ú','ù','ñ','Á','À','Ä','É','È','Ë','Í','Ì','Ï','Ó','Ò','Ö','Ú','Ù','Ü','Ñ','ç','Ç'};
      string sin[] ={ "a","a","a","a","e","e","e","i","i","i","o","o","o","u","u","n","A","A","A","E","E","E","I","I","I","O","O","O","U","U","U","N","c","C"};
      int tam=letra.length();

     for(int j=0;j<tam;j++)
     {

          for (int i = 0; i <34; i++)
          {
              if(letra[j]==con[i])
              {
              // cout<<con[i];
                letra.erase(j-1,1);
                letra.replace(j-1,1,sin[i]);
               // tam--;

              }
          }

     }


}


void Grafopdf::minus(string &word)
{
    float i=0;
    //p.erase(3,1);
    set<char> symbols={'¦','→','≈','@','"','!','$','%','&','/','(',')','=','?','¡','¿','-','_',':','.',';',',','—','','œ','€','<','>','+','*','[',']','{','}','º','„','·','#'};

    while(word[i])
    {
        if(symbols.find(word[i])!=symbols.end())
        {
            //cout<<endl<<*symbols.find(word[i]);
            word.erase(i,1);
        }
        else{

            word[i]= tolower(word[i]);
            i++;
        }
    }
}

Grafopdf::Grafopdf(string file, float grade, Dictionary &dictionary)
{
    this->file=file;
    this->grade=grade;
    this->dictionary=&dictionary;
   // string command("pdftotext "+file+".pdf "+file+".txt");
   // system(command.c_str());

    this->create_graf();
}

void Grafopdf::load_stop_words()
{
    string  buffer;
    ifstream file_sw("en_stop.txt");

    while(!file_sw.eof())
    {
        file_sw>>buffer;
        list_sw[buffer] = true;
    }
    file_sw.close();
}



bool Grafopdf::stop_word(string word)
{
    //if(word<="9" &&  word>="0") return true;
    if(this->list_sw.find(word) != this->list_sw.end())
    {
        return true;
    }
    return false;
}



void Grafopdf::clear_text()
{
    string  line;
    //ifstream file_read("text_reuters/textos/"+this->file+".txt");
    ifstream file_read(path_file+this->file+".txt");
    float cont=0;
    this->load_stop_words();

    while(!file_read.eof())
    {
        getline(file_read,line);
        remplazar(line);
        minus(line);//elimino mayusculas
        stringstream list_str(line);
        string word;
        while(list_str >> word)
        {
            if(!stop_word(word))
            {
                Porter2Stemmer::stem(word); //porter ingles
               /* std::wstring str2(word.length(), L' '); // Make room for characters
                std::copy(word.begin(), word.end(), str2.begin());
                stemming::spanish_stem<> stem ;                                     ////porter spanish
                stem(str2);
                string str3(str2.length(), ' ');
                   std::copy(str2.begin(), str2.end(), str3.begin());
                word=str3;*/
               // cout<<word<<" ";
                list_words[cont]=word;/////cambiooo id 1
                //list_words[cont]=this->dictionary->convert(word);/////cambiooo id 1
                cont++;
             }
        }//cout<<endl;

    }

}




void Grafopdf::create_graf()
{

      this->clear_text();
      float tam=this->list_words.size();
      float a=this->grade;
      if(tam<=this->grade){this->grade=tam-1;}
      for(float i=0;(i<tam - this->grade) && (this->grade>0);)
      {
          for(float j=1;j<=this->grade;j++)
          {
            if(list_words[i]!=list_words[i+j]){
              this->grafo[this->list_words[i+j]][this->list_words[i]]++;
              this->grafo[this->list_words[i]][this->list_words[i+j]]++;
              this->size++;}

          }

          i++;
          if(i>=tam-a){this->grade--;}
      }
}


void Grafopdf::get_keywords()
{
    for(auto iter:this->grafo)
    {
        float sum=0;
        for(auto iter2:iter.second)
        {
            sum=sum+iter2.second;
        }

        this->keywords[(1.0)/sum][iter.first]=true;
    }
}



void  Grafopdf::insert_ady_dijstra(id origen, float peso_acumulado)
{
    for(auto it:this->grafo[origen])
    {   if(Vis[it.first]==0){
            Nodo_Grafo<id,float> tmp(it.first,((1.0)/it.second)+(peso_acumulado));// cambie 2
            this->heap_nodos.push(tmp);
            //this->Vis[it.first]==1;
        }
    }

}

void Grafopdf::Dijkstra(id origen, float radio)
{
    this->result.clear();
    this->Vis.clear();
    this->Vis[origen]=1;
    insert_ady_dijstra(origen,0);///cambie 3
    while(!this->heap_nodos.empty())
    {
        Nodo_Grafo<id,float> min=this->heap_nodos.top();
        this->heap_nodos.pop();
        if(this->Vis[min.vertice]==0){
            this->Vis[min.vertice]=1;

            if(min.arista<=radio)
            {
                this->result[min.vertice]=min.arista;
                //cout<<"entro dijkstra "<<origen <<" "<<min.vertice<<" "<<min.arista<<endl;
                this->insert_ady_dijstra(min.vertice,min.arista); //cambie 4
            }

        }
    }
}

double Grafopdf::compare(Grafopdf *&other, int num_keywords, float radio)
{
    this->get_keywords();
    other->get_keywords();
    id g1[num_keywords],g2[num_keywords];
    int cont=0,sg1=0,sg2=0;
    for(auto it:this->keywords)    {
        for(auto it2:it.second)        {
            if(cont>=num_keywords)break;
            g1[cont]=it2.first;
            sg1=cont;
            cont++;        }
        if(cont>=num_keywords)break;    }
    cont=0;
    for(auto it:other->keywords)    {
        for(auto it2:it.second)        {
            if(cont>=num_keywords)break;
            g2[cont]=it2.first;
            sg2=cont;
            cont++;        }
        if(cont>=num_keywords)break;    }
    std::vector<id> inter_kw(num_keywords); // 0  0  0  0  0  0  0  0  0  0
    std::vector<id>::iterator it;
    std::sort (g1,g1+sg1);     //  5 10 15 20 25
    std::sort (g2,g2+sg2);   // 10 20 30 40 50

    it=std::set_intersection (g1, g1+sg1, g2, g2+sg2, inter_kw.begin());
                                                 // 10 20 0  0  0  0  0  0  0  0
    inter_kw.resize(it-inter_kw.begin());                      // 10 20
    ofstream guarda("resultados_amazon/vectores.txt",ios::app);

    int distance=inter_kw.size();
    map<string,bool> palabras_elegidas;
    for(auto it:inter_kw)
    {
        int res=0;
        this->Dijkstra(it,radio);
        other->Dijkstra(it,radio);
        palabras_elegidas[it]=true; ///inserto los keywords
        for(auto it2:this->result)
        {
            if(other->result[it2.first]==0){ other->result.erase(it2.first); }
            else
            {
               // guarda<<it2.first<<" ";
                palabras_elegidas[it2.first]=true; ///inserto adyacentes a los keywords con rho
                res++;

            }
        }

        distance=distance+res;
    }
    for(auto iiit:palabras_elegidas)///modif
    {
        guarda<<iiit.first<<" ";
    }
    guarda<<endl;
    return distance;


}

void Grafopdf::draw_dot()
{
    ofstream Esc("grafos/"+this->file+".dot");
    Esc<<"graph A {"<<endl;
    Esc<<" node[shape=box,fontcolor=black,style=filled];"<<endl;

    if(this->grafo.empty())return;

    for(auto iter:this->grafo)
    {
        if(iter.second.empty())
        { Esc<< iter.first <<";"<<endl; }
        else
        {
            for(auto it:iter.second)
            {
                Esc<<'"'<< iter.first<<'"'<<" -- " << '"' <<it.first << '"'<<"[label="<<'"'<<(1.0)/it.second<<'"'<<"];"<<endl;
            }
        }
    }
   Esc<<"}"<<endl;
   Esc.close();
   string comm =  "dot grafos/"+this->file+".dot -Tjpg -o grafos/"+this->file+".jpg";
   system(comm.c_str());
   cout<<endl<<"Listo: "+this->file+".jpg"<<endl;

}


#endif // GRAFOPDF



