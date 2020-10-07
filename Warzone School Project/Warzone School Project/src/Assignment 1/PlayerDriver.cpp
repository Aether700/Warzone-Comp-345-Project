#include "Player.h"
#include <iostream>
#include "Map.h"
#include "Orders.h"

int main() {

    //set a player
    WZ::Player* p1 = new WZ::Player();

    //set 3 territories
    WZ::Territory* t1 = new WZ::Territory("territory", 1);
    WZ::Territory* t2 = new WZ::Territory("territory", 1);
    WZ::Territory* t3 = new WZ::Territory("territory", 1);

    //Orders made by each player
    WZ::Order* o1 = new WZ::DeployOrder(p1, t1, 2);
    WZ::Order* o2 = new WZ::DeployOrder(p1, t2, 2);
    WZ::Order* o3 = new WZ::DeployOrder(p1, t3, 2);

    //add a territory to each one of the players
    p1->addTerritory(t1);
    t1->setOwner(p1);
    p1->addTerritory(t2);
    t2->setOwner(p1);
    p1->addTerritory(t3);
    t3->setOwner(p1);

        // to attack and to defend
    std::vector<WZ::Territory*> Defend = p1->toDefend();

    std::vector<WZ::Territory*> Attack = p1->toAttack();

    std::cout << "The territories to defend are:\n";
    for (WZ::Territory* t : Defend) {
        std::cout << *t << "\n";

    }
    std::cout << "\nThe territories to attack are:\n";
    for (WZ::Territory* t : Attack) {
        std::cout << *t << "\n";
    }

    std::cout << "\nIssue orders for player:\n";
    // add orders to player
    std::cout << *o1 << "\n";
    std::cout << *o2 << "\n";
    std::cout << *o3 << "\n";

    p1->issueOrder(o1);
    p1->issueOrder(o2);
    p1->issueOrder(o3);

    std::cout << "\nPrinting order list: ";
    std::cout << *p1->getOrderList() << "\n";

    /*clean up player and territories.
    Orders will be deleted by the OrderList of the Player. */
    delete p1;
    delete t1;
    delete t2;
    delete t3;
}