#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Weapon
{
public:
    virtual ~Weapon() = default;
    virtual std::string Name() const = 0; 
    virtual std::unique_ptr<Weapon> Clone() const = 0;
};

class Knife : public Weapon
{
public:
    std::string Name() const override { return "Knife"; }
    std::unique_ptr<Weapon> Clone() const override { return std::make_unique<Knife>(*this); }
};
class Gun : public Weapon
{
public:
    std::string Name() const override { return "Gun"; }
    std::unique_ptr<Weapon> Clone() const override { return std::make_unique<Gun>(*this); }
};
class Missile : public Weapon
{
public:
    std::string Name() const override { return "Missile"; }
    std::unique_ptr<Weapon> Clone() const override { return std::make_unique<Missile>(*this); }
};

class Vehicle
{
public:
    virtual ~Vehicle() = default;
    virtual std::string Name() const = 0;
    virtual std::unique_ptr<Vehicle> Clone() const = 0;
};

class Horse : public Vehicle
{
public:
    std::string Name() const override { return "Horse"; }
    std::unique_ptr<Vehicle> Clone() const override { return std::make_unique<Horse>(*this); }
};
class Tank : public Vehicle
{
public:
    std::string Name() const override { return "Tank"; }
    std::unique_ptr<Vehicle> Clone() const override { return std::make_unique<Tank>(*this); }
};
class Spaceship : public Vehicle
{
public:
    std::string Name() const override { return "Spaceship"; }
    std::unique_ptr<Vehicle> Clone() const override { return std::make_unique<Spaceship>(*this); }
};

class MunitionStore final
{
public:
    MunitionStore(std::unique_ptr<Weapon>&& weapon, std::unique_ptr<Vehicle>&& vehicle)
        : weapon_(std::move(weapon)), vehicle_(std::move(vehicle))
    {}

    void RegisterWeapon(std::unique_ptr<Weapon>&& weapon) { weapon_ = std::move(weapon); }
    void RegisterVehicle(std::unique_ptr<Vehicle>&& vehicle) { vehicle_ = std::move(vehicle); }

    std::unique_ptr<Weapon> MakeWeapon() const { return weapon_->Clone(); }
    std::unique_ptr<Vehicle> MakeVehicle() const { return vehicle_->Clone(); }

private:
    std::unique_ptr<Weapon> weapon_;
    std::unique_ptr<Vehicle> vehicle_;
};

void PrintStore(MunitionStore const& store)
{
    std::cout << "---- Infomation ----" << std::endl;
    std::cout << "Weapon  : " << store.MakeWeapon()->Name() << std::endl;
    std::cout << "Vehicle : " << store.MakeVehicle()->Name() << std::endl;
    std::cout << "--------------------" << std::endl;
}

void ChangeWeapon(MunitionStore& store, std::unique_ptr<Weapon>&& weapon)
{
    std::cout << "Change Weapon to " << weapon->Name() << std::endl;
    store.RegisterWeapon(std::move(weapon));
}

void ChangeVehicle(MunitionStore& store, std::unique_ptr<Vehicle>&& vehicle)
{
    std::cout << "Change Vehicle to " << vehicle->Name() << std::endl;
    store.RegisterVehicle(std::move(vehicle));
}

/*
    Prototype Pattern은 새로운 클래스 계층을 만들 지 않는 장점이 있습니다.
    그러나, 기존에 존재하는 제품 클래스 계층에 Clone 연산을 구현해야합니다. (만약 그것이 불가능할 경우, 원형패턴은 사용할 수 없습니다.)
    
    또한, 런타임에 새로운 제품을 추가할 수 있고, 객체의 값들을 다양화함으로써 새로운 제품을 만들 수 있습니다.

    Factory Method Pattern(혹은 Abstract Factory Pattern) 같은 경우는 서브클래싱 기반임으로 클래스 계층이 비대해지는 문제가 있습니다.
    반면에, Prototype Pattern은 제품 클래스 계층을 수정하는 대신, 새로운 클래스 계층이 비대해지는 문제가 없습니다.
*/

int main()
{
    MunitionStore store(std::make_unique<Knife>(), std::make_unique<Horse>());

    std::cout << "==== Munition Store ====" << std::endl;
    PrintStore(store);

    ChangeWeapon(store, std::make_unique<Gun>());
    PrintStore(store);

    ChangeVehicle(store, std::make_unique<Tank>());
    PrintStore(store);

    ChangeWeapon(store, std::make_unique<Missile>());
    ChangeVehicle(store, std::make_unique<Spaceship>());
    PrintStore(store);
}
