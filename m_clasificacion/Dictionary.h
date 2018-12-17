#ifndef DICTIONARY_H
#define DICTIONARY_H
#include<unordered_map>
using namespace std;
class Dictionary
{
    public:
        map<string,int> dictionary;
        Dictionary(){this->id=1;}
        int id;
        int convert(string word);
};

int Dictionary::convert(string word)
{
    if(dictionary[word]==0)
    {
        dictionary[word]=id;

        id++;
        int i=id-1;
        return i;
    }
    else{

        return dictionary[word];    }
}

#endif // DICTIONARY_H
