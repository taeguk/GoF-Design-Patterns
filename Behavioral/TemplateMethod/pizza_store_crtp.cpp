#include <iostream>
#include <type_traits>

template <typename Derived>
class PizzaStore
{
public:
    virtual ~PizzaStore() = default;

    void MakePizza()
    {
        auto& derived = static_cast<Derived&>(*this);
        derived.MakeDough_();
        derived.PutTopping_();
        derived.BakePizza_();
    }
};

class DeliciousPizzaStore : public PizzaStore<DeliciousPizzaStore>
{
    friend class PizzaStore<DeliciousPizzaStore>;

private:
    void MakeDough_()
    {
        std::cout << "Make delicious dough!" << std::endl;
    }

    void PutTopping_()
    {
        std::cout << "Put delicious topping!" << std::endl;
    }

    void BakePizza_()
    {
        std::cout << "Bake pizza in an expensive oven!" << std::endl;
    }
};

class PoorPizzaStore : public PizzaStore<PoorPizzaStore>
{
    friend class PizzaStore<PoorPizzaStore>;

private:
    void MakeDough_()
    {
        std::cout << "Make poor dough!" << std::endl;
    }

    void PutTopping_()
    {
        std::cout << "Put poor topping!" << std::endl;
    }

    void BakePizza_()
    {
        std::cout << "Bake pizza in a cheap oven!" << std::endl;
    }
};

/*
    C++에서 Template Method 패턴을 구현하는 또 다른 방법은 CRTP 를 활용하는 것입니다.
    이 경우, 가상함수 호출로 인한 overhead가 없다는 장점이 있습니다. 그러나,
    Derived 클래스에서 정의해야만 하는 함수들의 정보가 명확하게 드러나지 않고,
    IDE에서 에러를 표시해주지 않아서, 편의성이 떨어지고 실수할 가능성이 큽니다.
*/
int main()
{
    DeliciousPizzaStore deliciousPizzaStore;
    PoorPizzaStore poorPizzaStore;

    deliciousPizzaStore.MakePizza();
    std::cout << "----------------------------" << std::endl;
    poorPizzaStore.MakePizza();
}
