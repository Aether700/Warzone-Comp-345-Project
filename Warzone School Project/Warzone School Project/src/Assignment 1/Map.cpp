#include "Map.h"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Continent : Map{
    private:
    string name;

    public:


}



//Represents each Node (territory)
class Territory : public Continent{

//edges represent adjacency between each node (territory)
private:
int edge;
int n_of_adjacent_territories;
//Each territory contains a number of armies
int number_of_armies;

public:

Territory(){
    number_of_armies = 0;
}

void setNumber_of_armies(int k){
number_of_armies = k;
}

int getNumber_of_armies(){
    return number_of_armies;
}




}