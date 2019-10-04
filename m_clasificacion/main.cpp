#include <QCoreApplication>
#include <iostream>
#include <string>
#include "grafopdf.h"
#include"Dictionary.h"
#include <time.h>
#include <math.h>
#include<fstream>
#include<thread>
#include <stdio.h>
#include <clocale>
#include <cmath>
#include "stemming/spanish_stem.h"
#include <cmath>

#include<stdlib.h>

using namespace std;

///string global que guardara los resultados de las comparaciones

//std::map<string,map<string,vector<double>>> resultados_finales;
std::map<string,map<string,double>> resultados_finales;

int re=0;


void probar_experimentos()
{
    clock_t t;
   ///  t = clock();
     Dictionary dictionary,p;
     int adyacencia=2;
     int keywords=15;
     double rho=2;
     int num=1600;      ///numero de documentos que probaremos

    ///Cargo los texto y convierto a grafos para poder hacer las comparaciones luego
    Grafopdf *lista_grafos[num];int a=0;
    for(int i=1;i<=num;i++)
    {
        lista_grafos[i-1]= new Grafopdf(to_string(i),adyacencia,dictionary);
        a++;     //lista_grafos[i-1]->draw_dot();
    }
    ///hago las comparaciones finales de los textos paralelamente
    t = clock();

    cout<<"loading comparaciones"<<endl;
    for(int i=0;i<(num-1);i++)
    {
      for(int j=i+1;j<num;j++)
      {
           double result=lista_grafos[i]->compare(lista_grafos[j],keywords,rho);        ///compare
           resultados_finales[lista_grafos[i]->file][lista_grafos[j]->file]= result;
           re++;      cout<<re<<" ";
      }
    }

  t = clock() - t;


  std::ostringstream oss; oss << rho;string Rho=oss.str();

    ofstream fileres("resultados_amazon/resultados_amazon_prueba1_kw"+to_string(keywords)+"_rho"+Rho+"_ayd"+to_string(adyacencia)+".txt",ios::app);
    for(auto &it1:resultados_finales)
    {
        for(auto &it2:it1.second)
        {
              fileres<<it1.first<<" "<<it2.first<<" "<<it2.second<<endl;
        }
    }
     cout<<"Time: " << ((float)t/CLOCKS_PER_SEC) <<" sec "<<endl ;
}


void get_vector_caracteristica()
{
    string categoria="baby";
    string path="resultados_journal/resultados_amazon_"+categoria;

    ifstream leer(path+"/vectores.txt");
    map<string,int> VC;
    while(!leer.eof())
    {
        string word;
        leer>>word;
        VC[word]++;
    }
    ofstream escribir(path+"/vector_Caracteristica.txt");
    std::multimap<int,string> mymultimap;
    for(auto it:VC)
    {
        if(it.second>3000)
            mymultimap.insert (std::pair<int,string>(it.second,it.first));

    }
    for (std::multimap<int,string>::iterator it=mymultimap.begin(); it!=mymultimap.end(); ++it)
      escribir<< (*it).first << " " << (*it).second <<endl;

}

string mayor(string a,int aa,string b ,int bb,string c,int cc,string d,int dd,string e,int ee,string f,int ff)
{
    map<double,string> ordenado;
    ordenado[1.0/aa]=a;
    ordenado[1.0/bb]=b;
   // ordenado[1.0/cc]=c;
    ordenado[1.0/dd]=d;
    ordenado[1.0/ee]=e;
   // ordenado[1.0/ff]=f;
    std::map<double,string>::iterator it=ordenado.begin();
    return (*it).second;
}

void get_resultados_finales()
{


    map<string,int> baby;
    map<string,int> dvd;
    map<string,int> kitchen;
    map<string,int> software;
    map<string,int> toys_games;
    map<string,int> magazine;

//cout<<"entrp";

    ifstream leer_baby("resultados_journal1/resultados_amazon_baby/vector_Caracteristica.txt");
    ifstream leer_dvd("resultados_journal1/resultados_amazon_dvd/vector_Caracteristica.txt");
    ifstream leer_software("resultados_journal1/resultados_amazon_software/vector_Caracteristica.txt");
    ifstream leer_toys_games("resultados_journal1/resultados_amazon_toys_&_games/vector_Caracteristica.txt");

    int num; string word;
    while(!leer_baby.eof()){leer_baby>>num>>word;if(!leer_baby.eof()){ baby[word]=num; }  }
    while(!leer_dvd.eof()){leer_dvd>>num>>word;if(!leer_dvd.eof()){ dvd[word]=num; }  }
    while(!leer_software.eof()){leer_software>>num>>word;if(!leer_software.eof()){ software[word]=num; }  }
    while(!leer_toys_games.eof()){leer_toys_games>>num>>word;if(!leer_toys_games.eof()){ toys_games[word]=num; }  }



    string categ[]={"baby","dvd","software","toys_&_games","magazine","kitchen"};
    for(int cat_i=0;cat_i<4;cat_i++)
    {
         map<string,vector<string> > res;

        map<string,int> porcentaje;
        for(int i=1601;i<=2000;i++)///test
        {

            Grafopdf test;
            test.get_vector_map_texto(categ[cat_i]+"/"+to_string(i));
            int b=0,d=0,k=0,s=0,t=0,m=0;
            for(auto it:test.map_words)
            {
                if(baby[it.first]==0){baby.erase(it.first);}
                else{
                   //b+=baby[it.first];
                    b++;
                }


                if(dvd[it.first]==0){dvd.erase(it.first);}
                else{
                   // d+=dvd[it.first];
                    d++;
                }

                if(software[it.first]==0){software.erase(it.first);}
                else{
                    //s+=software[it.first];
                    s++;
                }

                if(toys_games[it.first]==0){toys_games.erase(it.first);}
                else{
                    //t+=toys_games[it.first];
                    t++;
                }

            }



            string may=mayor("baby",b,"dvd",d,"kitchen",k,"software",s,"toys_&_games",t,"magazine",m);
            porcentaje[may]++;
            res[may].push_back(categ[cat_i]+"/"+to_string(i));


        }


        /*cout<<res["baby"].size()<<endl;
        cout<<res["dvd"].size()<<endl;
        cout<<res["software"].size()<<endl;
        cout<<res["toys_&_games"].size()<<endl;
        */

        cout<<endl<<"categoria     "<<categ[cat_i]<<" : "<<endl;

         for(auto iii:porcentaje){cout<<"    -"<<iii.first<<" "<<iii.second<<" => "<<(iii.second*100)/400.0<<"%"<<endl;}

    }

}



int main()
{


  //   probar_experimentos(); ///hacer las comparaciones todos contra todos
   //  get_vector_caracteristica(); /// a partir de los resultados de las comparaciones obtener vector caract. de la categoria

   get_resultados_finales();
//crear_relaciones();
  //  convertir_vectores_caracteristicas_los_resultados();
  //  seleccionar_Vectores_caracteristica();

   /*     ifstream leer("resultados_journal/resultados_amazon_baby/vector_Caracteristica.txt");
        ofstream escribir("resultados_journal/resultados_amazon_baby/hist_categoria1.tsv");
        map<string,string> mapita;
        while(!leer.eof())
        {
            string a,b;
            leer>>a>>b;
            mapita[b]=a;

        }

        for(auto it:mapita)
        {
            escribir<<it.first<<" ."<<it.second<<endl;
        }
        */

}




