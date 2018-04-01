#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../../AccessKey.h"

class EstateOwner;
class GroceryStore;
class Restaurant;

////////////////////////////////////////////////////////////////////////////////
class BusinessMediator
{
public:
    BusinessMediator(EstateOwner& estateOwner, GroceryStore& groceryStore, Restaurant& restaurant);

    void EstateRentPriceChanged(std::int32_t oldPrice, std::int32_t newPrice);
    void GroceryStockChanged(std::int32_t currentStock);
    void GroceryPriceChanged(std::int32_t oldPrice, std::int32_t newPrice);
    void FoodIsCooked();

private:
    EstateOwner& estateOwner_;
    GroceryStore& groceryStore_;
    Restaurant& restaurant_;
};

////////////////////////////////////////////////////////////////////////////////
class EstateOwner
{
public:
    std::int32_t SetEstateRentPrice(std::int32_t price)
    {
        auto oldPrice = estateRentPrice_;
        estateRentPrice_ = price;
        if (mediator_) mediator_->EstateRentPriceChanged(oldPrice, price);
        return oldPrice;
    }
    
    BusinessMediator* SetBusinessMediator(design::AccessKey<BusinessMediator>,
                                          BusinessMediator* mediator)
    {
        BusinessMediator* old = mediator_;
        mediator_ = mediator;
        return old;
    }

private:
    BusinessMediator* mediator_{ nullptr };
    std::int32_t estateRentPrice_{ 10000 };
};

////////////////////////////////////////////////////////////////////////////////
class GroceryStore
{
public:
    std::int32_t Supply(std::uint16_t count)
    {
        stock_ += count;
        if (mediator_) mediator_->GroceryStockChanged(stock_);
        return stock_;
    }

    std::int32_t Sell()
    {
        if (stock_ <= 0)
        {
            throw std::logic_error("Not in stock.");
        }

        --stock_;
        if (mediator_) mediator_->GroceryStockChanged(stock_);
        return price_;
    }

    std::int32_t AlterPrice(std::int32_t priceChange)
    {
        auto oldPrice = price_;
        price_ += priceChange;
        if (mediator_) mediator_->GroceryPriceChanged(oldPrice, price_);
        return price_;
    }

    BusinessMediator* SetBusinessMediator(design::AccessKey<BusinessMediator>,
                                          BusinessMediator* mediator)
    {
        BusinessMediator* old = mediator_;
        mediator_ = mediator;
        return old;
    }

private:
    BusinessMediator* mediator_{ nullptr };
    std::int32_t stock_{ 0 };
    std::int32_t price_{ 100 };
};

////////////////////////////////////////////////////////////////////////////////
class Restaurant
{
public:
    std::int32_t CookFood()
    {
        if (isOpened_)
        {
            if (mediator_) mediator_->FoodIsCooked();
            return price_;
        }
        else
        {
            return -1;
        }
    }

    std::int32_t AlterPrice(std::int32_t priceChange)
    {
        price_ += priceChange;
        return price_;
    }

    BusinessMediator* SetBusinessMediator(design::AccessKey<BusinessMediator>,
                                          BusinessMediator* mediator)
    {
        BusinessMediator* old = mediator_;
        mediator_ = mediator;
        return old;
    }

    void SetIsOpened(design::AccessKey<BusinessMediator>,
                     bool isOpened)
    {
        isOpened_ = isOpened;
    }

private:
    BusinessMediator* mediator_{ nullptr };
    bool isOpened_{ true };
    std::int32_t price_{ 500 };
};

////////////////////////////////////////////////////////////////////////////////
BusinessMediator::BusinessMediator(EstateOwner& estateOwner, GroceryStore& groceryStore, Restaurant& restaurant)
    : estateOwner_(estateOwner), groceryStore_(groceryStore), restaurant_(restaurant)
{
    estateOwner_.SetBusinessMediator(design::AccessKey<BusinessMediator>(), this);
    groceryStore_.SetBusinessMediator(design::AccessKey<BusinessMediator>(), this);
    restaurant_.SetBusinessMediator(design::AccessKey<BusinessMediator>(), this);
}

void BusinessMediator::EstateRentPriceChanged(std::int32_t oldPrice, std::int32_t newPrice)
{
    groceryStore_.AlterPrice((newPrice - oldPrice) / 10000);
    restaurant_.AlterPrice((newPrice - oldPrice) / 1000);
}

void BusinessMediator::GroceryStockChanged(std::int32_t currentStock)
{
    if (currentStock > 0)
    {
        restaurant_.SetIsOpened(design::AccessKey<BusinessMediator>(), true);
    }
    else
    {
        restaurant_.SetIsOpened(design::AccessKey<BusinessMediator>(), false);
    }
}

void BusinessMediator::GroceryPriceChanged(std::int32_t oldPrice, std::int32_t newPrice)
{
    restaurant_.AlterPrice(newPrice - oldPrice);
}

void BusinessMediator::FoodIsCooked()
{
    groceryStore_.Sell();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void BuyFood(Restaurant& restaurant)
{
    auto price = restaurant.CookFood();
    if (price >= 0)
    {
        std::cout << "[BuyFood] The price of food : " << price << std::endl;
    }
    else
    {
        std::cout << "[BuyFood] Restaurant was closed bescause groceries are lack." << std::endl;
    }
}

void SupplyGrocery(GroceryStore& groceryStore, std::uint16_t count)
{
    auto newCount = groceryStore.Supply(count);
    auto oldCount = newCount - count;
    std::cout << "Grocery Stock Changes : " << oldCount << " -> " << newCount << std::endl;
}

void ChangeGroceryPrice(GroceryStore& groceryStore, std::int32_t priceChange)
{
    auto newPrice = groceryStore.AlterPrice(priceChange);
    auto oldPrice = newPrice - priceChange;
    std::cout << "Grocery Price Changes : " << oldPrice << " -> " << newPrice << std::endl;
}

void ChangeEstateRentPrice(EstateOwner& estateOwner, std::int32_t newPrice)
{
    auto oldPrice = estateOwner.SetEstateRentPrice(newPrice);
    std::cout << "EstateRentPrice Changes : " << oldPrice << " -> " << newPrice << std::endl;
}

/*
    Mediator Pattern은 한 집합에 속해있는 객체들의 상호작용을 캡슐화하는 패턴입니다.
    객체들간의 상호작용이 하나의 객체로 모아지기 때문에, 객체들이 서로를 직접 참조하지 않게되어
    객체들이 loosely coupling 되는 효과가 있습니다.
    객체들간의 상호작용 로직이 객체들과 분리되어 추상화되므로, 상호작용 로직만을 쉽게 수정하거나
    런타임에 변경할 수 있게 됩니다. 또한 각 객체들이 별도로 재사용이 가능해지게 됩니다.
    단, Mediator 클래스 자체는 복잡도가 증가해 유지보수가 어려워질 수 있습니다.
*/

int main()
{
    EstateOwner estateOwner;
    GroceryStore groceryStore;
    Restaurant restaurant;

    BusinessMediator mediator(estateOwner, groceryStore, restaurant);

    SupplyGrocery(groceryStore, 2);
    groceryStore.Sell();
    BuyFood(restaurant);
    BuyFood(restaurant);
    std::cout << std::endl;

    SupplyGrocery(groceryStore, 3);
    ChangeEstateRentPrice(estateOwner, 1000);
    BuyFood(restaurant);
    ChangeEstateRentPrice(estateOwner, 10000);
    BuyFood(restaurant);
    ChangeEstateRentPrice(estateOwner, 100000);
    BuyFood(restaurant);
    std::cout << std::endl;

    SupplyGrocery(groceryStore, 3);
    ChangeGroceryPrice(groceryStore, 100);
    BuyFood(restaurant);
    ChangeEstateRentPrice(estateOwner, 10000);
    BuyFood(restaurant);
    ChangeGroceryPrice(groceryStore, -100);
    BuyFood(restaurant);
    BuyFood(restaurant);
}
