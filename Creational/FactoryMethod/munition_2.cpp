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


enum class WeaponType { Knife, Gun, Missile };
enum class VehicleType { Horse, Tank, Spaceship };

struct WeaponCreator 
{ 
	virtual std::unique_ptr<Weapon> MakeWeapon(WeaponType type) const
	{
		switch (type)
		{
			case WeaponType::Knife:
				return std::make_unique<Knife>();
			case WeaponType::Gun:
				return std::make_unique<Gun>();
			case WeaponType::Missile:
				return std::make_unique<Missile>();
			default:
				return nullptr;
		}
	}
};

struct FakeWeaponCreator : public WeaponCreator
{
	virtual std::unique_ptr<Weapon> MakeWeapon(WeaponType type) const override
	{
		switch (type)
		{
		case WeaponType::Knife:
			return std::make_unique<Missile>();
		default:
			return WeaponCreator::MakeWeapon(type);
		}
	}
};

struct VehicleCreator
{
	virtual std::unique_ptr<Vehicle> MakeVehicle(VehicleType type) const
	{
		switch (type)
		{
		case VehicleType::Horse:
			return std::make_unique<Horse>();
		case VehicleType::Tank:
			return std::make_unique<Tank>();
		case VehicleType::Spaceship:
			return std::make_unique<Spaceship>();
		default:
			return nullptr;
		}
	}
};

struct FakeVehicleCreator : public VehicleCreator
{
	virtual std::unique_ptr<Vehicle> MakeVehicle(VehicleType type) const override
	{
		switch (type)
		{
		case VehicleType::Horse:
			return std::make_unique<Spaceship>();
		case VehicleType::Tank:
			return std::make_unique<Horse>();
		case VehicleType::Spaceship:
			return std::make_unique<Tank>();
		default:
			return nullptr;
		}
	}
};


class MunitionStore
{
public:
	MunitionStore(std::unique_ptr<WeaponCreator>&& weaponCreator, std::unique_ptr<VehicleCreator>&& vehicleCreator)
		: m_weaponCreator(std::move(weaponCreator)), m_vehicleCreator(std::move(vehicleCreator))
	{}

	void ChangeWeaponCreator(std::unique_ptr<WeaponCreator>&& creator) { m_weaponCreator = std::move(creator); }
	void ChangeVehicleCreator(std::unique_ptr<VehicleCreator>&& creator) { m_vehicleCreator = std::move(creator); }

	void Print(WeaponType weaponType, VehicleType vehicleType) const
	{
		std::cout << "---- Infomation ----" << std::endl;
		std::cout << "Weapon  : " << m_weaponCreator->MakeWeapon(weaponType)->Name() << std::endl;
		std::cout << "Vehicle : " << m_vehicleCreator->MakeVehicle(vehicleType)->Name() << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << std::endl;
	}

private:
	std::unique_ptr<WeaponCreator> m_weaponCreator;
	std::unique_ptr<VehicleCreator> m_vehicleCreator;
};

/*
	Factory Method Pattern은 매개변수화되어 사용될 수도 있습니다.
*/

int main()
{
	MunitionStore store(std::make_unique<WeaponCreator>(), std::make_unique<VehicleCreator>());

	std::cout << "==== Munition Store ====" << std::endl;
	store.Print(WeaponType::Knife, VehicleType::Horse);

	store.ChangeWeaponCreator(std::make_unique<FakeWeaponCreator>());
	store.Print(WeaponType::Knife, VehicleType::Horse);

	store.ChangeVehicleCreator(std::make_unique<FakeVehicleCreator>());
	store.Print(WeaponType::Knife, VehicleType::Horse);
}