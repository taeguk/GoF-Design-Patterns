#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

////////////////////////////// Sub Systems //////////////////////////////
class GroceryStore final
{
public:
    static void BuyBeef() { std::cout << "Buy Beef in grocery store." << std::endl; }
    static void BuyNoodle() { std::cout << "Buy Noodle in grocery store." << std::endl; }
    static void BuySauce() { std::cout << "Buy Sauce in grocery store." << std::endl; }
    static void BuyRice() { std::cout << "Buy Rice in grocery store." << std::endl; }
    static void BuyGochujang() { std::cout << "Buy Gochujang in grocery store." << std::endl; }
    static void BuySalt() { std::cout << "Buy Salt in grocery store." << std::endl; }
    static void BuyMSG() { std::cout << "Buy MSG in grocery store." << std::endl; }
};

class CookShop final
{
public:
    static void BuyFryingPan() { std::cout << "Buy Frying Pan in cook shop." << std::endl; }
    static void BuyDish() { std::cout << "Buy Dish in cook shop." << std::endl; }
    static void BuyBowl() { std::cout << "Buy Bowl in cook shop." << std::endl; }
};

class EstateAgent final
{
public:
    static void RentShop() { std::cout << "Rent a shop from an estate agent." << std::endl; }
};

////////////////////////////// Facade //////////////////////////////
class Restaurant final
{
public:
    Restaurant()
    {
        EstateAgent::RentShop();
        std::cout << "Ready to cook!" << '\n' << std::endl;
    }

    void CookSteak()
    {
        std::cout << "Let's cook a steak!" << std::endl;
        CookShop::BuyFryingPan();
        GroceryStore::BuyBeef();
        GroceryStore::BuySauce();
        GroceryStore::BuySalt();
        GroceryStore::BuyMSG();
        CookShop::BuyDish();
        std::cout << "Here is a great steak!" << '\n' << std::endl;
    }

    void CookPasta()
    {
        std::cout << "Let's cook a pasta!" << std::endl;
        CookShop::BuyFryingPan();
        GroceryStore::BuyNoodle();
        GroceryStore::BuySauce();
        GroceryStore::BuyMSG();
        CookShop::BuyDish();
        std::cout << "Here is a delicious pasta!" << '\n' << std::endl;
    }

    void CookBibimbap()
    {
        std::cout << "Let's cook a bibimbap!" << std::endl;
        CookShop::BuyBowl();
        GroceryStore::BuyRice();
        GroceryStore::BuyGochujang();
        std::cout << "Here is a healthy bibimbap!" << '\n' << std::endl;
    }
};

int main()
{
    Restaurant restaurant;

    restaurant.CookSteak();
    restaurant.CookPasta();
    restaurant.CookBibimbap();
}
