#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct HamburgerIngredient { virtual ~HamburgerIngredient() = default; virtual std::string Name() const = 0; };

struct Bread : public HamburgerIngredient { virtual std::string Name() const override { return "Bread"; } };
struct Patty : public HamburgerIngredient { virtual std::string Name() const override { return "Patty"; } };
struct Cheese : public HamburgerIngredient { virtual std::string Name() const override { return "Cheese"; } };
struct Tomato : public HamburgerIngredient { virtual std::string Name() const override { return "Tomato"; } };
struct Lettuce : public HamburgerIngredient { virtual std::string Name() const override { return "Lettuce"; } };
struct Sauce : public HamburgerIngredient { virtual std::string Name() const override { return "Sauce"; } };

struct LightBread : public Bread { virtual std::string Name() const override { return "LightBread"; } };
struct SaltyBread : public Bread { virtual std::string Name() const override { return "SaltyBread"; } };

struct BeefPatty : public Patty { virtual std::string Name() const override { return "BeefPatty"; } };
struct PorkPatty : public Patty { virtual std::string Name() const override { return "PorkPatty"; } };

struct MozzarellaCheese : public Cheese { virtual std::string Name() const override { return "MozzarellaCheese"; } };
struct CheddarCheese : public Cheese { virtual std::string Name() const override { return "CheddarCheese"; } };

struct GarlicSauce : public Sauce { virtual std::string Name() const override { return "GarlicSauce"; } };
struct MustardSauce : public Sauce { virtual std::string Name() const override { return "MustardSauce"; } };

class Hamburger final
{
public:
    explicit Hamburger(std::vector<std::unique_ptr<HamburgerIngredient>>&& ingredientList)
        : ingredientList_(std::move(ingredientList))
    {}

    void Print() const
    {
        std::cout << "----------" << std::endl;
        for (const auto& ingredient : ingredientList_)
            std::cout << ingredient->Name() << std::endl;
        std::cout << "----------" << std::endl;
    }

private:
    std::vector<std::unique_ptr<HamburgerIngredient>> ingredientList_;
};

class HamburgerFactory
{
public:
    virtual ~HamburgerFactory() = default;

    virtual std::unique_ptr<Bread> MakeBread() const = 0;
    virtual std::unique_ptr<Patty> MakePatty() const = 0;
    virtual std::unique_ptr<Cheese> MakeCheese() const = 0;
    virtual std::unique_ptr<Tomato> MakeTomato() const = 0;
    virtual std::unique_ptr<Lettuce> MakeLettuce() const = 0;
    virtual std::unique_ptr<Sauce> MakeSauce() const = 0;
};

class BeefHamburgerFactory : public HamburgerFactory
{
public:
    virtual std::unique_ptr<Bread> MakeBread() const override { return std::make_unique<LightBread>(); }
    virtual std::unique_ptr<Patty> MakePatty() const override { return std::make_unique<BeefPatty>(); };
    virtual std::unique_ptr<Cheese> MakeCheese() const override { return std::make_unique<MozzarellaCheese>(); }
    virtual std::unique_ptr<Tomato> MakeTomato() const override { return std::make_unique<Tomato>(); }
    virtual std::unique_ptr<Lettuce> MakeLettuce() const override { return std::make_unique<Lettuce>(); }
    virtual std::unique_ptr<Sauce> MakeSauce() const override { return std::make_unique<GarlicSauce>(); }
};

class PorkHamburgerFactory : public HamburgerFactory
{
public:
    virtual std::unique_ptr<Bread> MakeBread() const override { return std::make_unique<SaltyBread>(); }
    virtual std::unique_ptr<Patty> MakePatty() const override { return std::make_unique<PorkPatty>(); };
    virtual std::unique_ptr<Cheese> MakeCheese() const override { return std::make_unique<CheddarCheese>(); }
    virtual std::unique_ptr<Tomato> MakeTomato() const override { return std::make_unique<Tomato>(); }
    virtual std::unique_ptr<Lettuce> MakeLettuce() const override { return std::make_unique<Lettuce>(); }
    virtual std::unique_ptr<Sauce> MakeSauce() const override { return std::make_unique<MustardSauce>(); }
};

class HamburgerHouse final
{
public:
    explicit HamburgerHouse(std::unique_ptr<HamburgerFactory>&& factory)
        : factory_(std::move(factory))
    {}

    void ChangeFactory(std::unique_ptr<HamburgerFactory>&& factory) { factory_ = std::move(factory); }

    Hamburger MakeCheeseBurger() const
    {
        std::vector<std::unique_ptr<HamburgerIngredient>> ingredientList;

        ingredientList.push_back(factory_->MakeBread());
        ingredientList.push_back(factory_->MakeLettuce());
        ingredientList.push_back(factory_->MakeSauce());
        ingredientList.push_back(factory_->MakePatty());
        ingredientList.push_back(factory_->MakeCheese());
        ingredientList.push_back(factory_->MakeBread());

        return Hamburger(std::move(ingredientList));
    }

    Hamburger MakeJumboBurger() const
    {
        std::vector<std::unique_ptr<HamburgerIngredient>> ingredientList;

        ingredientList.push_back(factory_->MakeBread());
        ingredientList.push_back(factory_->MakeLettuce());
        ingredientList.push_back(factory_->MakeSauce());
        ingredientList.push_back(factory_->MakePatty());
        ingredientList.push_back(factory_->MakeCheese());
        ingredientList.push_back(factory_->MakeBread());
        ingredientList.push_back(factory_->MakeLettuce());
        ingredientList.push_back(factory_->MakeSauce());
        ingredientList.push_back(factory_->MakePatty());
        ingredientList.push_back(factory_->MakeCheese());
        ingredientList.push_back(factory_->MakeBread());

        return Hamburger(std::move(ingredientList));
    }

private:
    std::unique_ptr<HamburgerFactory> factory_;
};

/*
    특정 제품들을 모아서 제품 군 형태로 관리하고, 제품 군 내에서 생성을 유도하여 일관성을 지키고 싶은 경우,
        Abstract Factory 패턴이 어울립니다.

    복잡한 객체를 생성해야 할 때, Builder 패턴과 Abstract Factory 패턴은 어느정도 유사한 점이 있습니다.
    만약, 제품 군내의 제품들 각각이 개별적으로 생성될 필요성이 있고, 좀 더 설계가 유연할 필요성이 있다면,
        Abstract Factory Pattern 이 더 어울립니다.
*/

int main()
{
    HamburgerHouse macdoria(std::make_unique<BeefHamburgerFactory>());

    std::cout << "======== Beef Based Hamburger House ========" << std::endl;
    std::cout << "* Let's make a cheese burger" << std::endl;
    macdoria.MakeCheeseBurger().Print();
    std::cout << "* Let's make a jumbo burger" << std::endl;
    macdoria.MakeJumboBurger().Print();

    macdoria.ChangeFactory(std::make_unique<PorkHamburgerFactory>());
    std::cout << std::endl;
    std::cout << "======== Pork Based Hamburger House ========" << std::endl;
    std::cout << "* Let's make a cheese burger" << std::endl;
    macdoria.MakeCheeseBurger().Print();
    std::cout << "* Let's make a jumbo burger" << std::endl;
    macdoria.MakeJumboBurger().Print();

    return 0;
}
