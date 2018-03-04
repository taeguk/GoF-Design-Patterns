#include <iostream>
#include <map>
#include <memory>

/* 부가적 상태 (Extrinsic State) */
struct MonsterContext
{
    int hp;
    int posX, posY;
};

class Monster
{
public:
    virtual ~Monster() = default;

    virtual void Print(MonsterContext const& context) const = 0;
};

class Slime : public Monster
{
public:
    struct IntrinsicState
    {
        bool operator<(IntrinsicState const& state) const
        {
            if (viscosity < state.viscosity)
                return true;
            else if (viscosity > state.viscosity)
                return false;
            else
                return transparency < state.transparency;
        }

        int viscosity;
        int transparency;
    };

    explicit Slime(IntrinsicState const& state)
        : state_(state)
    {}

    void Print(MonsterContext const& context) const override
    {
        std::cout << "----- Slime -----" << std::endl;
        std::cout << "HP           : " << context.hp << std::endl;
        std::cout << "Position     : (" << context.posX << ", " << context.posY << ")" << std::endl;
        std::cout << "Viscosity    : " << state_.viscosity << std::endl;
        std::cout << "Transparency : " << state_.transparency << std::endl;
        std::cout << "-----------------" << std::endl;
    }

private:
    IntrinsicState state_;
};

class Goblin : public Monster
{
public:
    struct IntrinsicState
    {
        bool operator<(IntrinsicState const& state) const
        {
            if (angerDegree < state.angerDegree)
                return true;
            else if (angerDegree > state.angerDegree)
                return false;
            else
                return uglinessDegree < state.uglinessDegree;
        }

        int angerDegree;
        int uglinessDegree;
    };

    explicit Goblin(IntrinsicState const& state)
        : state_(state)
    {}

    void Print(MonsterContext const& context) const override
    {
        std::cout << "----- Goblin -----" << std::endl;
        std::cout << "HP             : " << context.hp << std::endl;
        std::cout << "Position       : (" << context.posX << ", " << context.posY << ")" << std::endl;
        std::cout << "AngerDegree    : " << state_.angerDegree << std::endl;
        std::cout << "UglinessDegree : " << state_.uglinessDegree << std::endl;
        std::cout << "------------------" << std::endl;
    }

private:
    IntrinsicState state_;
};

class Dragon : public Monster
{
public:
    struct IntrinsicState
    {
        bool operator<(IntrinsicState const& state) const
        {
            if (height < state.height)
                return true;
            else if (height > state.height)
                return false;
            else
                return firePower < state.firePower;
        }

        int height;
        int firePower;
    };

    explicit Dragon(IntrinsicState const& state)
        : state_(state)
    {}

    void Print(MonsterContext const& context) const override
    {
        std::cout << "----- Dragon -----" << std::endl;
        std::cout << "HP        : " << context.hp << std::endl;
        std::cout << "Position  : (" << context.posX << ", " << context.posY << ")" << std::endl;
        std::cout << "Height    : " << state_.height << std::endl;
        std::cout << "FirePower : " << state_.firePower << std::endl;
        std::cout << "------------------" << std::endl;
    }

private:
    IntrinsicState state_;
};

class MonsterPool
{
public:
    Slime const& GetSlime(Slime::IntrinsicState const& state) { return GetImpl_(slimePool_, state); }
    Goblin const& GetGoblin(Goblin::IntrinsicState const& state) { return GetImpl_(goblinPool_, state); }
    Dragon const& GetDragon(Dragon::IntrinsicState const& state) { return GetImpl_(dragonPool_, state); }

private:
    template <typename MonsterType>
    static MonsterType const& GetImpl_(
        std::map<typename MonsterType::IntrinsicState, MonsterType>& pool,
        typename MonsterType::IntrinsicState const& state)
    {
        auto iter = pool.find(state);

        if (iter == std::end(pool))
        {
            iter = pool.insert(std::make_pair(state, MonsterType(state))).first;
        }

        return iter->second;
    }

    std::map<Slime::IntrinsicState, Slime> slimePool_;
    std::map<Goblin::IntrinsicState, Goblin> goblinPool_;
    std::map<Dragon::IntrinsicState, Dragon> dragonPool_;
};

int main()
{
    MonsterPool monsterPool;

    monsterPool.GetSlime({ 6, 3 }).Print({ 50, 0, 0 });
    monsterPool.GetSlime({ 6, 3 }).Print({ 30, 1, 2 });
    monsterPool.GetSlime({ 1, 2 }).Print({ 40, 2, -1 });

    monsterPool.GetGoblin({ 7, 5 }).Print({ 80, 9, -3 });
    monsterPool.GetGoblin({ 5, 8 }).Print({ 70, -5, -3 });
    monsterPool.GetGoblin({ 5, 8 }).Print({ 90, 7, 0 });

    monsterPool.GetDragon({ 10, 12 }).Print({ 200, 2, 11 });
    monsterPool.GetDragon({ 10, 12 }).Print({ 400, 0, 9 });
    monsterPool.GetDragon({ 10, 12 }).Print({ 350, 5, 1 });
}
