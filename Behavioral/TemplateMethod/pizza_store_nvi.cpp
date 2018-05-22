#include <iostream>

class PizzaStore
{
public:
    virtual ~PizzaStore() = default;

    void MakePizza()
    {
        MakeDough_();
        PutTopping_();
        BakePizza_();
    }

private:
    virtual void MakeDough_() = 0;
    virtual void PutTopping_() = 0;
    virtual void BakePizza_() = 0;
};

class DeliciousPizzaStore : public PizzaStore
{
private:
    void MakeDough_() override
    {
        std::cout << "Make delicious dough!" << std::endl;
    }

    void PutTopping_() override
    {
        std::cout << "Put delicious topping!" << std::endl;
    }

    void BakePizza_() override
    {
        std::cout << "Bake pizza in an expensive oven!" << std::endl;
    }
};

class PoorPizzaStore : public PizzaStore
{
private:
    void MakeDough_() override
    {
        std::cout << "Make poor dough!" << std::endl;
    }

    void PutTopping_() override
    {
        std::cout << "Put poor topping!" << std::endl;
    }

    void BakePizza_() override
    {
        std::cout << "Bake pizza in a cheap oven!" << std::endl;
    }
};

/*
    Template Method Pattern은 객체의 연산에는 알고리즘의 뼈대만을 정의하고,
    각 단계에서 수행할 구체적 처리는 서브클래스 쪽으로 미루는 패턴입니다.
    즉, "변하지 않는 것" 으로 부터 "변하는 것" 을 가상함수 기반으로 분리하는 것입니다.

    이 패턴과는 다르게, Strategy Pattern은 "변하지 않는 것" 으로 부터 "변하는 것" 을
    다른 클래스로 분리합니다. Strategy Pattern은 런타임에 구체적인 처리 방식을
    변경할 수 있는 반면에 Template Method Pattern은 런타임에 구체적 처리 방식을
    변경할 수 없습니다.

    Template Method Pattern을 C++에서 구현하는 방법에는 NVI idiom 과 CRTP idiom 이
    있습니다. 이 소스코드에서는 NVI idiom 을 이용하였습니다.
*/
int main()
{
    DeliciousPizzaStore deliciousPizzaStore;
    PoorPizzaStore poorPizzaStore;

    deliciousPizzaStore.MakePizza();
    std::cout << "----------------------------" << std::endl;
    poorPizzaStore.MakePizza();
}
