#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

//#include "Orders.h"
//#include "Player.h"

namespace WZ
{
    class Continent;
    class Player;

    /*Territory class that represents a territory on our map or a node on our graph
    */
    class Territory
    {
        friend class GameManager;
        friend class Player;
        friend class Continent;
        friend class Map;
        friend class MapLoader;
        friend class AggressivePlayerStrategy;
    public:
        /*Territory constructor

         name: the name of the territory to construct
         id: the id of the territory to construct
         continent: the continent this territory is part of.
         armies: optional parameter indicating the number of armies on the territory. Defaults to 0
        */
        Territory(const std::string& name, unsigned int id, Continent* continent = nullptr, unsigned int armies = 0);

        /* copy constructor of the Territory class. 
           Note that this constructor does not do a deep 
           copy of the adjancency list since it would create an infinite loop of creation
           The indepth copy is made by the Map class

           other: the other Territory which will be used to initialize this one
        */
        Territory(const Territory& other);

        /* getter function that returns the name of the Territory object

          returns: the name of the Territory object
        */
        const std::string& getName() const;
        
        /* getter function that returns the id of the Territory object

          returns: the id of the Territory object
        */
        unsigned int getID() const;

        /* getter function that returns the continent which contains the Territory object

          returns: the continent which contains the Territory object
        */
        Continent* getContinent();

        /* getter function that returns a const ptr to the continent which contains the Territory object

          returns: a const ptr to the continent which contains the Territory object
        */
        const Continent* getContinent() const;

        /* getter function that returns the number of armies currently on the Territory object

          returns: the number of armies currently on the Territory object
        */
        unsigned int getArmies() const;

        /* getter function that returns the number of available armies currently on the Territory object

          returns: the number of available armies currently on the Territory object
        */
        unsigned int getAvailableArmies() const;

        /* getter function that returns a const ptr to the player who owns this territory or 
           nullptr if the territory is neutral

          returns: a const ptr to the player who owns this territory or nullptr if the territory is neutral
        */
        const Player* getOwner() const;

        /* getter function that returns the player who owns this territory or
           nullptr if the territory is neutral

          returns: the player who owns this territory or nullptr if the territory is neutral
        */
        Player* getOwner();

        /*setter function which sets the current owner of the territory
        
         p: the new owner of this territory
        */
        void setOwner(Player* p);

        /*setter function which sets the current army count of the territory

         p: the new army count of this territory
        */
        void setArmies(unsigned int amount);

        /*function which clears the whole adjacency list of this territory
        */
        void clearAdjList();

        /*function which adds a territory to the adjacency list of this territory

          adj: the new territory to add to the adjacency list
        */
        void addAdjTerritory(Territory* adj);

        /*function which removes a territory from the adjacency list of this territory

          adj: the new territory to remove from the adjacency list
        */
        void removeAdjTerritory(Territory* adj);

        /*getter function which returns a const reference to the adjacency list of this territory

          returns: a const reference to the adjacency list of this territory
        */
        const std::vector<Territory*>& getAdjList() const;

        /* returns true if the provided territory can be accessed by this territory, false otherwise

           t: the territory to access from this one
           returns: true if the provided territory can be accessed by this territory, false otherwise
        */
        bool isAdjTo(Territory* t) const;

        /*assignment operator. copies the values of the territory provided into this territory. 
           does not do a deep copy of the adjancency list since it would create an infinite loop of creation
           The indepth copy is made by the Map class

           other: the Territory object to assign
		   returns: the modified Territory object
        */
        Territory& operator=(const Territory& other);

        /* equals operator. returns true if the two territories 
           share the same name and the same id

           other: the other Territory object being compared to this one

           returns: true if the two territories 
           share the same name and the same id false otherwise
        */
        bool operator==(const Territory& other) const;

        /*Standard != operator.
         Returns true if the == operator was false and vice versa

         returns: true if the == operator was false and vice versa
        */
        bool operator!=(const Territory& other) const;

    private:
        /*private setter function to be used by the friend class Map when performing a deep copy of the Map object. 
          Sets the new continent object which stores this territory

          continent: the new continent object which stores this territory
        */
        void setContinent(Continent* continent);

        unsigned int m_id;
        std::string m_name;
        std::vector<Territory*> m_adjList;
        bool m_visited;

        Continent* m_continent;
        Player* m_owner;

        unsigned int m_armies;
        unsigned int m_availableArmies;
    };

    /* insertion operator for Territory objects. 
      Inserts a string representation of this territory object to the ostream provided

       stream: the stream to insert the Territory into
       t: the territory to insert into the stream
       returns: the stream provided once the territory has been inserted
    */
    std::ostream& operator<<(std::ostream& stream, const Territory& t);
    

    /*Continent class which acts as a sub graph of the Map class and contains the territories
    */
    class Continent
    {
        friend class Map;
    public:
        /*Continent constructor

         name: the name of the continent to construct
         bonus: the bonus provided to the player who owns all the territories inside this continent.
        */
        Continent(const std::string& name, unsigned int bonus);

        /* copy constructor of the Continent class.
           Note that this constructor does not do a deep
           copy of the territory list since it would create an infinite loop of creation
           The indepth copy is made by the Map class

           other: the other Continent which will be used to initialize this one
        */
        Continent(const Continent& other);

        /*Deconstructor of the Continent class
          deletes all the territories contained inside the Continent object   
        */
        ~Continent();

        /* getter function that returns the name of the Continent object

          returns: the name of the Continent object
        */
        const std::string& getName() const;

        /* getter function that returns the bonus of the Continent object

          returns: the bonus of the Continent object
        */
        unsigned int getBonus() const;

        /*function which clears the list of territories contained inside this Continent
        */
        void clearTerritories();
        
        /* getter function that returns the Territory object at the index provided

          index: the index of the Territory object to return
          returns: the Territory object at the index provided
        */
        Territory* getTerritory(size_t index);

        /* getter function that returns a const ptr of the Territory object at the index provided

          index: the index of the Territory object to return
          returns: a const ptr of the Territory object at the index provided
        */
        const Territory* getTerritory(size_t index) const;

        /* getter function that returns the number of Territory object stored 
          inside this continent object

          returns: the number of Territory object stored inside this continent object
        */
        size_t getCount() const;
        
        /*function which returns true if the provided territory is part of this continent

          returns: true if the provided territory is part of this continent false otherwise
        */
        bool contains(Territory* t) const;

        /*utility function which verifies that this continent is a valid connected subgraph.

          returns: true if the continent is a connected subgraph false otherwise
        */
        bool validate() const;

        /*utility function which verifies that this continent does not share 
          any territories with the other continent provided

          returns: true if continent does not share 
          any territories with the other continent provided false otherwise
        */
        bool noSharedTerritories(const Continent* other) const;

        /*Adds a Territory object to this continent object

          t: the territory to add
        */
        void addTerritory(Territory* t);
        
        /*Removes a Territory object from this continent object

          t: the territory to remove
        */
        void removeTerritory(Territory* t);

        /*assignment operator. copies the values of the Continent provided into this continent.
           does not do a deep copy of the territory list since it would create an infinite loop of creation
           The indepth copy is made by the Map class

           other: the Continent object to assign
           returns: the modified Continent object
        */
        Continent& operator=(const Continent& other);

        /* equals operator. returns true if the two continents
           share the same name and the same id

           other: the other Continent object being compared to this one

           returns: true if the two continents
           share the same name and the same id false otherwise
        */
        bool operator==(const Continent& other) const;

        /*Standard != operator.
         Returns true if the == operator was false and vice versa

         returns: true if the == operator was false and vice versa
        */
        bool operator!=(const Continent& other) const;

    private:
        /*private helper function used to reset the "visited" member variable of all the 
          territories in this continent object. 
          Used when verifiying the validity of this continent
        */
        void ResetVisitedTerritories() const;

        bool m_visited;
        unsigned int m_bonus;
        std::string m_name;
        std::vector<Territory*> m_territories;
    };

    /* insertion operator for Continent objects.
      Inserts a string representation of this continent object to the ostream provided

       stream: the stream to insert the Continent into
       c: the continent to insert into the stream
       returns: the stream provided once the continent has been inserted
    */
    std::ostream& operator<<(std::ostream& stream, const Continent& c);


    class Map
    {
    public:
        /*Default Map constructor
        */
        Map();

        /*Map constructor

         continents: list of the continents that will be part of this map object
        */
        Map(std::vector<Continent*> continents);

        /* copy constructor of the Map class.
           makes a deep copy of all the Continents and 
           Territory objects contained in the other Map object to copy

          other: the other Map which will be used to initialize this one
       */
        Map(const Map& other);

        /*Deconstructor of the Map class
          deletes all the continents contained inside the Map object
        */
        ~Map();

        /*validate method which, as per the assignment guidelines, makes 
          sure that the map is a connected graph where each continent is a 
          connected subgraph and that each territory is part of only one continent

          returns: true if the map is a valid map as per the assignment guidelines, false otherwise
        */
        bool validate() const;

        /* getter function that returns the Continent object at the index provided

          index: the index of the Continent object to return
          returns: the Continent object at the index provided
        */
        Continent* getContinent(size_t index);

        /* getter function that returns a const ptr of the Continent object at the index provided

          index: the index of the Continent object to return
          returns: a const ptr of the Continent object at the index provided
        */
        const Continent* getContinent(size_t index) const;

        /* getter function that returns the number of Continent object stored
          inside this map object

          returns: the number of Continent object stored inside this map object
        */
        size_t getContinentCount() const;

        /* getter function that returns the number of Territory object stored
          inside this map object

          returns: the number of Territory object stored inside this map object
        */
        size_t getTerritoryCount() const;

        /*Adds a Continent object to this map object

          c: the map to add
        */
        void addContinent(Continent* c);
        
        /*Removes a Continent object from this map object

          c: the map to remove
        */
        void removeContinent(Continent* c);

        /* returns a vector list of all the territories on the map which can access the provided territory

          t: the territory to access
          returns: a vector list of all the territories on the map which can access the provided territory
        */
        std::vector<Territory*> getAccessList(Territory* t) const;

        /*assignment operator. copies the values of the Map provided into this map.
           makes a deep copy of all the Continents and 
           Territory objects contained in the other Map object to copy

           other: the Map object to assign
           returns: the modified Map object
        */
        Map& operator=(const Map& other);

        /* begin override function which provides an iterator at the start of the unordered map of Territories of the map

          returns: an iterator at the start of the unordered map of Territories of the map
        */
        std::unordered_map<unsigned int, Territory*>::iterator begin();

        /* end override function which provides an iterator at the end of the unordered map of Territories of the map

          returns: an iterator at the end of the unordered map of Territories of the map
        */
        std::unordered_map<unsigned int, Territory*>::iterator end();

        /* begin override function which provides a const iterator at the start of the unordered map of Territories of the map

          returns: a const iterator at the start of the unordered map of Territories of the map
        */
        std::unordered_map<unsigned int, Territory*>::const_iterator begin() const;

        /* end override function which provides a const iterator at the end of the unordered map of Territories of the map

          returns: a const iterator at the end of the unordered map of Territories of the map
        */
        std::unordered_map<unsigned int, Territory*>::const_iterator end() const;

    private:
        /*private helper function which make a deep 
          copy of the Map object provided into this one
        */
        void CopyMap(const Map& other);

        /*private helper function used to reset the "visited" member variable of all the
          continents in this map object.
          Used when verifiying the validity of this map
        */
        void ResetVisitedContinents() const;

        /*helper function which sets the Territory hash map (the unordered map) 
          so that we can access the territories easily
        */
        void SetTerritoryHash();

        std::vector<Continent*> m_continents;
        std::unordered_map<unsigned int, Territory*> m_territoryHash;
    };

    /* insertion operator for Map objects.
     Inserts a string representation of this map object to the ostream provided

      stream: the stream to insert the Map into
      m: the Map to insert into the stream
      returns: the stream provided once the map has been inserted
   */
    std::ostream& operator<<(std::ostream& stream, const Map& m);
}



