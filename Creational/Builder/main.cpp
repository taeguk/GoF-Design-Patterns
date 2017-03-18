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

class HamburgerBuilder
{
public:
	virtual void Initialize() = 0;

	virtual void BuildBread() = 0;
	virtual void BuildPatty() = 0;
	virtual void BuildCheese() = 0;
	virtual void BuildTomato() = 0;
	virtual void BuildLettuce() = 0;
	virtual void BuildSauce() = 0;

	virtual std::unique_ptr<Hamburger> GetHamburger() = 0;
};

class BeefHamburgerBuilder : public HamburgerBuilder
{
public:
	virtual void Initialize() override { m_hamburger = std::make_unique<Hamburger>(); }

	virtual void BuildBread() override { m_hamburger->AddIngredient(std::make_unique<LightBread>()); }
	virtual void BuildPatty() override { m_hamburger->AddIngredient(std::make_unique<BeefPatty>()); };
	virtual void BuildCheese() override { m_hamburger->AddIngredient(std::make_unique<MozzarellaCheese>()); };
	virtual void BuildTomato() override { m_hamburger->AddIngredient(std::make_unique<Tomato>()); };
	virtual void BuildLettuce() override { m_hamburger->AddIngredient(std::make_unique<Lettuce>()); };
	virtual void BuildSauce() override { m_hamburger->AddIngredient(std::make_unique<GarlicSauce>()); };

	virtual std::unique_ptr<Hamburger> GetHamburger() override 
	{ 
		auto ret = std::move(m_hamburger);
		m_hamburger = std::make_unique<Hamburger>();
		return std::move(ret); 
	};

private:
	std::unique_ptr<Hamburger> m_hamburger{ std::make_unique<Hamburger>() };
};

class PorkHamburgerBuilder : public HamburgerBuilder
{
public:
	virtual void Initialize() override { m_hamburger = std::make_unique<Hamburger>(); }

	virtual void BuildBread() override { m_hamburger->AddIngredient(std::make_unique<SaltyBread>()); }
	virtual void BuildPatty() override { m_hamburger->AddIngredient(std::make_unique<PorkPatty>()); };
	virtual void BuildCheese() override { m_hamburger->AddIngredient(std::make_unique<CheddarCheese>()); };
	virtual void BuildTomato() override { m_hamburger->AddIngredient(std::make_unique<Tomato>()); };
	virtual void BuildLettuce() override { m_hamburger->AddIngredient(std::make_unique<Lettuce>()); };
	virtual void BuildSauce() override { m_hamburger->AddIngredient(std::make_unique<MustardSauce>()); };

	virtual std::unique_ptr<Hamburger> GetHamburger() override
	{
		auto ret = std::move(m_hamburger);
		m_hamburger = std::make_unique<Hamburger>();
		return std::move(ret);
	};

private:
	std::unique_ptr<Hamburger> m_hamburger{ std::make_unique<Hamburger>() };
};

class HamburgerHouse
{
public:
	HamburgerHouse(std::unique_ptr<HamburgerBuilder> builder)
		: m_builder(std::move(builder))
	{}

	void ChangeBuilder(std::unique_ptr<HamburgerBuilder> builder) { m_builder = std::move(builder); }

	std::unique_ptr<Hamburger> MakeCheeseBurger()
	{
		m_builder->Initialize();

		m_builder->BuildBread();
		m_builder->BuildLettuce();
		m_builder->BuildSauce();
		m_builder->BuildPatty();
		m_builder->BuildCheese();
		m_builder->BuildBread();

		return m_builder->GetHamburger();
	}

	std::unique_ptr<Hamburger> MakeJumboBurger()
	{
		m_builder->Initialize();

		m_builder->BuildBread();
		m_builder->BuildLettuce();
		m_builder->BuildSauce();
		m_builder->BuildPatty();
		m_builder->BuildCheese();
		m_builder->BuildBread();
		m_builder->BuildLettuce();
		m_builder->BuildSauce();
		m_builder->BuildPatty();
		m_builder->BuildCheese();
		m_builder->BuildBread();

		return m_builder->GetHamburger();
	}

private:
	std::unique_ptr<HamburgerBuilder> m_builder;
};

/*
	복합 객체의 생성 알고리즘과 합성되는 요소 객체들의 조립 방법이 독립적이고,
		합성과정을 숨기고 싶고, 복잡한 객체를 단계적으로 생성하고 싶을 경우에,
		Builder Pattern이 어울립니다.

	복잡한 객체를 생성해야 할 때, Builder 패턴과 Abstract Factory 패턴은 어느정도 유사한 점이 있습니다.
	만약, 생성과정(요소 객체들의 합성과정)들을 숨기고 싶고, 복잡한 객체가 완성된 후 반환되어야 하는 경우라면,
		Builder Pattern 이 더 어울립니다.
*/

int main()
{
	HamburgerHouse macdoria(std::make_unique<BeefHamburgerBuilder>());

	std::cout << "======== Beef Based Hamburger House ========" << std::endl;
	std::cout << "* Let's make a cheese burger" << std::endl;
	macdoria.MakeCheeseBurger()->Print();
	std::cout << "* Let's make a jumbo burger" << std::endl;
	macdoria.MakeJumboBurger()->Print();

	macdoria.ChangeBuilder(std::make_unique<PorkHamburgerBuilder>());
	std::cout << std::endl;
	std::cout << "======== Pork Based Hamburger House ========" << std::endl;
	std::cout << "* Let's make a cheese burger" << std::endl;
	macdoria.MakeCheeseBurger()->Print();
	std::cout << "* Let's make a jumbo burger" << std::endl;
	macdoria.MakeJumboBurger()->Print();

	return 0;
}