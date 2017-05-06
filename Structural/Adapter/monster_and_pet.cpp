#include <memory>
#include <string>
#include <iostream>
#include <type_traits>

/* 태초에 Monster들이 존재했다.. */
struct Monster 
{
	virtual std::string Name() const = 0;
	virtual int AttackAbility() const = 0;
	virtual int DefenseAbility() const = 0;
};

struct Slime : public Monster
{
	virtual std::string Name() const override { return "Slime"; }
	virtual int AttackAbility() const override { return 6; }
	virtual int DefenseAbility() const override { return 20; }
};

struct Goblin : public Monster
{
	virtual std::string Name() const override { return "Goblin"; }
	virtual int AttackAbility() const override { return 30; }
	virtual int DefenseAbility() const override { return 10; }
};

struct Dragon : public Monster
{
	virtual std::string Name() const override { return "Dragon"; }
	virtual int AttackAbility() const override { return 1000; }
	virtual int DefenseAbility() const override { return 500; }
};

/* 그리고 태초에 Pet들이 존재했다.. */
struct Pet
{
	virtual std::string Name() const = 0;
	virtual int CutenessAbility() const = 0;
	virtual int BattleBonus() const = 0;
};

struct Puppy : public Pet
{
	virtual std::string Name() const override { return "Puppy"; }
	virtual int CutenessAbility() const override { return 50; }
	virtual int BattleBonus() const override { return 5; }
};

struct Cat : public Pet
{
	virtual std::string Name() const override { return "Cat"; }
	virtual int CutenessAbility() const override { return 40; }
	virtual int BattleBonus() const override { return 10; }
};

/*** 그러나 게임이 업데이트되면서 Monster들도 Pet이 될 수 있게 되었다면?! 
		==> 이 경우 Adapter 패턴을 이용하면 기존의 Monster 클래스 계통을 수정하지 않고, 이 문제를 해결 할 수 있다!
***/

/* Monster를 정적으로(compile-time) Pet의 일부로서 만들고 싶은 경우, 
	template을 활용하면, overhead를 최소화할 수 있다. 단, 유연성은 떨어진다.
*/
template <typename _MonsterType, 
			typename = std::enable_if_t<std::is_base_of<Monster, _MonsterType>::value>>
struct PetAdapterStatic : public Pet
{
	virtual std::string Name() const override { return m_monster.Name(); }
	virtual int CutenessAbility() const override { return m_monster.DefenseAbility() - 2 * m_monster.AttackAbility(); }
	virtual int BattleBonus() const override { return m_monster.AttackAbility() + m_monster.DefenseAbility() / 2; }

private:
	_MonsterType m_monster;
};

/* Monster를 동적으로(run-time) Pet의 일부로서 만들고 싶은 경우,
	Monster 클래스 계통의 base class로서 서브타이핑을 하면 큰 유연성을 얻을 수 있다.
	pet에 해당하는 monster를 바꾸거나 pet을 다시 monster로 복귀시키는 등의 동적인 작업이 가능해진다.
	단, static한 버전(template을 이용한 버전)에 비해 overhead가 더 존재한다.
*/
struct PetAdapterDynamic : public Pet
{
	PetAdapterDynamic(std::unique_ptr<Monster>&& monster)
		: m_monster(std::move(monster))
	{}

	std::unique_ptr<Monster> ChangeMonster(std::unique_ptr<Monster>&& monster) 
	{
		std::unique_ptr<Monster> returnedMonster = std::move(m_monster);
		m_monster = std::move(monster); 
		return returnedMonster;
	}

	virtual std::string Name() const override { return m_monster->Name(); }
	virtual int CutenessAbility() const override { return m_monster->DefenseAbility() - 2 * m_monster->AttackAbility(); }
	virtual int BattleBonus() const override { return m_monster->AttackAbility() + m_monster->DefenseAbility() / 2; }

private:
	std::unique_ptr<Monster> m_monster;
};

void PrintMonster(const Monster& monster)
{
	std::cout << "---- Infomation ----" << std::endl;
	std::cout << "Name             : " << monster.Name() << std::endl;
	std::cout << "Attack Ability   : " << monster.AttackAbility() << std::endl;
	std::cout << "Defense Ability  : " << monster.DefenseAbility() << std::endl;
	std::cout << "--------------------" << std::endl;
}

void PrintPet(const Pet& pet)
{
	std::cout << "---- Infomation ----" << std::endl;
	std::cout << "Name             : " << pet.Name() << std::endl;
	std::cout << "Cuteness Ability : " << pet.CutenessAbility() << std::endl;
	std::cout << "Battle Bonus     : " << pet.BattleBonus() << std::endl;
	std::cout << "--------------------" << std::endl;
}

int main()
{
	Puppy puppy;
	Cat cat;

	std::cout << "====== Normal Pet ======" << std::endl << std::endl;

	PrintPet(puppy);
	PrintPet(cat);

	std::cout << std::endl << "====== Monster Pet ======" << std::endl << std::endl;

	PetAdapterStatic<Slime> petSlime;
	PetAdapterStatic<Goblin> petGoblin;
	PetAdapterStatic<Dragon> petDragon;

	PrintPet(petSlime);
	PrintPet(petGoblin);
	PrintPet(petDragon);

	std::cout << std::endl << "====== Monster ======" << std::endl << std::endl;

	auto slime = std::make_unique<Slime>();
	auto goblin = std::make_unique<Goblin>();
	auto dragon = std::make_unique<Dragon>();

	PrintMonster(*slime);
	PrintMonster(*goblin);
	PrintMonster(*dragon);

	std::cout << std::endl << "====== Pet From Monster ======" << std::endl << std::endl;

	PetAdapterDynamic petAdapter(std::move(slime));
	PrintPet(petAdapter);
	petAdapter.ChangeMonster(std::move(goblin));
	PrintPet(petAdapter);
	auto returnedMonster = petAdapter.ChangeMonster(std::move(dragon));
	PrintPet(petAdapter);

	std::cout << std::endl << "====== Monster Returned From Pet ======" << std::endl << std::endl;

	PrintMonster(*returnedMonster);
}