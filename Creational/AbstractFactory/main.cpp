#include <memory>
#include <vector>
#include <string>
#include <iostream>

struct HamburgerIngredient { virtual std::string Name() = 0; };

struct Bread : public HamburgerIngredient { virtual std::string Name() override { return "Bread"; } };
struct Patty : public HamburgerIngredient { virtual std::string Name() override { return "Patty"; } };
struct Cheese : public HamburgerIngredient { virtual std::string Name() override { return "Cheese"; } };
struct Tomato : public HamburgerIngredient { virtual std::string Name() override { return "Tomato"; } };
struct Lettuce : public HamburgerIngredient { virtual std::string Name() override { return "Lettuce"; } };
struct Sauce : public HamburgerIngredient { virtual std::string Name() override { return "Sauce"; } };

struct LightBread : public Bread { virtual std::string Name() override { return "LightBread"; } };
struct SaltyBread : public Bread { virtual std::string Name() override { return "SaltyBread"; } };

struct BeefPatty : public Patty { virtual std::string Name() override { return "BeefPatty"; } };
struct PorkPatty : public Patty { virtual std::string Name() override { return "PorkPatty"; } };

struct MozzarellaCheese : public Cheese { virtual std::string Name() override { return "MozzarellaCheese"; } };
struct CheddarCheese : public Cheese { virtual std::string Name() override { return "CheddarCheese"; } };

struct GarlicSauce : public Sauce { virtual std::string Name() override { return "GarlicSauce"; } };
struct MustardSauce : public Sauce { virtual std::string Name() override { return "MustardSauce"; } };


class Hamburger
{
public:
	void AddIngredient(std::unique_ptr<HamburgerIngredient> ingredient)
	{
		m_ingredientList.push_back(std::move(ingredient));
	}

	void Print()
	{
		std::cout << "----------" << std::endl;
		for (auto & ingredient : m_ingredientList)
			std::cout << ingredient->Name() << std::endl;
		std::cout << "----------" << std::endl;
	}

private:
	std::vector<std::unique_ptr<HamburgerIngredient>> m_ingredientList;
};

class HamburgerFactory
{
public:
	virtual std::unique_ptr<Bread> MakeBread() = 0;
	virtual std::unique_ptr<Patty> MakePatty() = 0;
	virtual std::unique_ptr<Cheese> MakeCheese() = 0;
	virtual std::unique_ptr<Tomato> MakeTomato() = 0;
	virtual std::unique_ptr<Lettuce> MakeLettuce() = 0;
	virtual std::unique_ptr<Sauce> MakeSauce() = 0;
};

class BeefHamburgerFactory : public HamburgerFactory
{
public:
	virtual std::unique_ptr<Bread> MakeBread() override { return std::make_unique<LightBread>(); }
	virtual std::unique_ptr<Patty> MakePatty() override { return std::make_unique<BeefPatty>(); };
	virtual std::unique_ptr<Cheese> MakeCheese() override { return std::make_unique<MozzarellaCheese>(); }
	virtual std::unique_ptr<Tomato> MakeTomato() override { return std::make_unique<Tomato>(); }
	virtual std::unique_ptr<Lettuce> MakeLettuce() override { return std::make_unique<Lettuce>(); }
	virtual std::unique_ptr<Sauce> MakeSauce() override { return std::make_unique<GarlicSauce>(); }
};

class PorkHamburgerFactory : public HamburgerFactory
{
public:
	virtual std::unique_ptr<Bread> MakeBread() override { return std::make_unique<SaltyBread>(); }
	virtual std::unique_ptr<Patty> MakePatty() override { return std::make_unique<PorkPatty>(); };
	virtual std::unique_ptr<Cheese> MakeCheese() override { return std::make_unique<CheddarCheese>(); }
	virtual std::unique_ptr<Tomato> MakeTomato() override { return std::make_unique<Tomato>(); }
	virtual std::unique_ptr<Lettuce> MakeLettuce() override { return std::make_unique<Lettuce>(); }
	virtual std::unique_ptr<Sauce> MakeSauce() override { return std::make_unique<MustardSauce>(); }
};

class HamburgerHouse
{
public:
	HamburgerHouse(std::unique_ptr<HamburgerFactory> factory)
		: m_factory(std::move(factory))
	{}

	void ChangeFactory(std::unique_ptr<HamburgerFactory> factory) { m_factory = std::move(factory); }

	std::unique_ptr<Hamburger> MakeCheeseBurger()
	{
		std::unique_ptr<Hamburger> hamburger = std::make_unique<Hamburger>();

		hamburger->AddIngredient(m_factory->MakeBread());
		hamburger->AddIngredient(m_factory->MakeLettuce());
		hamburger->AddIngredient(m_factory->MakeSauce());
		hamburger->AddIngredient(m_factory->MakePatty());
		hamburger->AddIngredient(m_factory->MakeCheese());
		hamburger->AddIngredient(m_factory->MakeBread());

		return std::move(hamburger);
	}

	std::unique_ptr<Hamburger> MakeJumboBurger()
	{
		std::unique_ptr<Hamburger> hamburger = std::make_unique<Hamburger>();

		hamburger->AddIngredient(m_factory->MakeBread());
		hamburger->AddIngredient(m_factory->MakeLettuce());
		hamburger->AddIngredient(m_factory->MakeSauce());
		hamburger->AddIngredient(m_factory->MakePatty());
		hamburger->AddIngredient(m_factory->MakeCheese());
		hamburger->AddIngredient(m_factory->MakeBread());
		hamburger->AddIngredient(m_factory->MakeLettuce());
		hamburger->AddIngredient(m_factory->MakeSauce());
		hamburger->AddIngredient(m_factory->MakePatty());
		hamburger->AddIngredient(m_factory->MakeCheese());
		hamburger->AddIngredient(m_factory->MakeBread());

		return std::move(hamburger);
	}

private:
	std::unique_ptr<HamburgerFactory> m_factory;
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
	macdoria.MakeCheeseBurger()->Print();
	std::cout << "* Let's make a jumbo burger" << std::endl;
	macdoria.MakeJumboBurger()->Print();

	macdoria.ChangeFactory(std::make_unique<PorkHamburgerFactory>());
	std::cout << std::endl;
	std::cout << "======== Pork Based Hamburger House ========" << std::endl;
	std::cout << "* Let's make a cheese burger" << std::endl;
	macdoria.MakeCheeseBurger()->Print();
	std::cout << "* Let's make a jumbo burger" << std::endl;
	macdoria.MakeJumboBurger()->Print();

	return 0;
}