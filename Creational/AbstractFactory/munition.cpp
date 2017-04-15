#include <memory>
#include <vector>
#include <string>
#include <iostream>

struct Weapon { virtual std::string Name() const = 0; };
struct Vehicle { virtual std::string Name() const = 0; };

struct Knife : public Weapon { virtual std::string Name() const override { return "Knife"; } };
struct Gun : public Weapon { virtual std::string Name() const override { return "Gun"; } };
struct Missile : public Weapon { virtual std::string Name() const override { return "Missile"; } };

struct Horse : public Vehicle { virtual std::string Name() const override { return "Horse"; } };
struct Tank : public Vehicle { virtual std::string Name() const override { return "Tank"; } };
struct Spaceship : public Vehicle { virtual std::string Name() const override { return "Spaceship"; } };


class MunitionFactory
{
public:
	virtual std::unique_ptr<Weapon> MakeWeapon() const = 0;
	virtual std::unique_ptr<Vehicle> MakeVehicle() const = 0;
};

class AncientMunitionFactory : public MunitionFactory
{
public:
	virtual std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Knife>(); }
	virtual std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Horse>(); }
};

class ModernMunitionFactory : public MunitionFactory
{
public:
	virtual std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Gun>(); }
	virtual std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Tank>(); }
};

class FutureMunitionFactory : public MunitionFactory
{
public:
	virtual std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Missile>(); }
	virtual std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Spaceship>(); }
};


class MunitionStore
{
public:
	MunitionStore(std::unique_ptr<MunitionFactory>&& factory)
		: m_factory(std::move(factory))
	{}
	void ChangeFactory(std::unique_ptr<MunitionFactory>&& factory) { m_factory = std::move(factory); }

	void Print() const
	{
		std::cout << "Weapon  : " << m_factory->MakeWeapon()->Name() << std::endl;
		std::cout << "Vehicle : " << m_factory->MakeVehicle()->Name() << std::endl;
		std::cout << std::endl;
	}

private:
	std::unique_ptr<MunitionFactory> m_factory;
};

/*
	Abstract Factory Pattern은 Factory Method Pattern을 이용하여 구현됩니다.
	(이 예제 코드는 Abstract Factory Pattern인 동시에 Factory Method Pattern이기도 한 셈입니다.)
*/

int main()
{
	MunitionStore store(std::make_unique<AncientMunitionFactory>());

	std::cout << "======== Ancient Munition Store ========" << std::endl;
	store.Print();

	std::cout << "======== Modern Munition Store ========" << std::endl;
	store.ChangeFactory(std::make_unique<ModernMunitionFactory>());
	store.Print();

	std::cout << "======== Future Munition Store ========" << std::endl;
	store.ChangeFactory(std::make_unique<FutureMunitionFactory>());
	store.Print();
}