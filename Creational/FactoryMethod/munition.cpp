#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Weapon { virtual ~Weapon() = default; virtual std::string Name() const = 0; };
struct Vehicle { virtual ~Vehicle() = default; virtual std::string Name() const = 0; };

struct Knife : public Weapon { std::string Name() const override { return "Knife"; } };
struct Gun : public Weapon { std::string Name() const override { return "Gun"; } };
struct Missile : public Weapon { std::string Name() const override { return "Missile"; } };

struct Horse : public Vehicle { std::string Name() const override { return "Horse"; } };
struct Tank : public Vehicle { std::string Name() const override { return "Tank"; } };
struct Spaceship : public Vehicle { std::string Name() const override { return "Spaceship"; } };

struct WeaponCreator
    { virtual ~WeaponCreator() = default; virtual std::unique_ptr<Weapon> MakeWeapon() const = 0; };
struct VehicleCreator
    { virtual ~VehicleCreator() = default; virtual std::unique_ptr<Vehicle> MakeVehicle() const = 0; };

struct KnifeCreator : public WeaponCreator
    { std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Knife>(); } };
struct GunCreator : public WeaponCreator
    { std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Gun>(); } };
struct MissileCreator : public WeaponCreator
    { std::unique_ptr<Weapon> MakeWeapon() const override { return std::make_unique<Missile>(); } };

struct HorseCreator : public VehicleCreator
    { std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Horse>(); } };
struct TankCreator : public VehicleCreator
    { std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Tank>(); } };
struct SpaceshipCreator : public VehicleCreator
    { std::unique_ptr<Vehicle> MakeVehicle() const override { return std::make_unique<Spaceship>(); } };

class MunitionStore final
{
public:
    MunitionStore(std::unique_ptr<WeaponCreator>&& weaponCreator, std::unique_ptr<VehicleCreator>&& vehicleCreator)
        : weaponCreator_(std::move(weaponCreator)), vehicleCreator_(std::move(vehicleCreator))
    {}

    void ChangeWeaponCreator(std::unique_ptr<WeaponCreator>&& creator) { weaponCreator_ = std::move(creator); }
    void ChangeVehicleCreator(std::unique_ptr<VehicleCreator>&& creator) { vehicleCreator_ = std::move(creator); }

    void Print() const
    {
        std::cout << "---- Infomation ----" << std::endl;
        std::cout << "Weapon  : " << weaponCreator_->MakeWeapon()->Name() << std::endl;
        std::cout << "Vehicle : " << vehicleCreator_->MakeVehicle()->Name() << std::endl; 
        std::cout << "--------------------" << std::endl;
        std::cout << std::endl;
    }

private:
    std::unique_ptr<WeaponCreator> weaponCreator_;
    std::unique_ptr<VehicleCreator> vehicleCreator_;
};

/*
    Factory Method Pattern은 제품 클래스 계통을 수정하지 않고, 손쉽게 적용할 수 있는 장점이 있습니다. 
    (병렬적인 클래스 계통을 연결하는 역할을 담당합니다.)
    그러나, 제품 클래스 계통과 대응되는 팩토리 클래스 계통이 만들어지면서, 
    제품이 추가될 때마다 팩토리 클래스도 추가되어야 하며, 점점 클래스들이 많아지게 되는 단점이 있습니다.
*/

int main()
{
    MunitionStore store(std::make_unique<KnifeCreator>(), std::make_unique<HorseCreator>());

    std::cout << "==== Munition Store ====" << std::endl;
    store.Print();

    store.ChangeWeaponCreator(std::make_unique<GunCreator>());
    store.Print();

    store.ChangeVehicleCreator(std::make_unique<TankCreator>());
    store.Print();

    store.ChangeWeaponCreator(std::make_unique<MissileCreator>());
    store.ChangeVehicleCreator(std::make_unique<SpaceshipCreator>());
    store.Print();
}
