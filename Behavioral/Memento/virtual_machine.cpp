#include <iostream>
#include <stack>
#include <vector>

// Mementor 클래스
class Snapshot
{
private:
    friend class VirtualMachine;

    // 예시를 위해 간단하게 했지만, 실제로는 외부에 노출되면 안되는 많은 상태 정보들이
    // 들어있을 것입니다.
    struct State
    {
        std::uint16_t cpuCount{ 1 };
        std::uint64_t ramSize{ 500 };
        // ...
    };

    explicit Snapshot(State const& state)
        : state_(state)
    {}

    State state_;
};

// Originator 클래스
class VirtualMachine
{
public:
    std::uint16_t GetCpuCount() const { return state_.cpuCount; }
    std::uint64_t GetRamSize() const { return state_.ramSize; }

    std::uint16_t ChangeCpuCount(std::uint16_t cpuCount)
    {
        auto old = state_.cpuCount;
        state_.cpuCount = cpuCount;
        return old;
    }

    std::uint64_t ChangeRamSize(std::uint64_t ramSize)
    {
        auto old = state_.ramSize;
        state_.ramSize = ramSize;
        return old;
    }

    void ResetToSnapshot(Snapshot const& snapshot)
    {
        state_ = snapshot.state_;
    }

    Snapshot TakeSnapshot() const
    {
        return Snapshot(state_);
    }

private:
    Snapshot::State state_;
};

void PrintVmInfo(VirtualMachine const& vm)
{
    std::cout << "----------------" << std::endl;
    std::cout << "Cpu Count : " << vm.GetCpuCount() << std::endl;
    std::cout << "Ram Size  : " << vm.GetRamSize() << std::endl << std::endl;
}

/*
    Memento Pattern은 캡슐화를 위배하지 않은 채, 어떤 객체의 내부 상태를 저장해둠으로서,
    나중에 그 객체를 다시 원래 상태로 되돌아 올 수 있도록 해줍니다.
    핵심포인트는, Originator 객체의 내부 상태를 담고 있는 Memento 객체를 외부에서
    관리한다는 점입니다.
    만약, Originator 가 모든 상태 정보들을 관리한다면, Originator 는 너무 복잡해지게 됩니다.
    Memento Pattern은 상태 정보를 담고 있는 객체들을 외부에서 관리하게 함으로서,
    Originator를 단순하게 만듭니다.
    그렇지만, 세부적인 상태 정보에 대해서는 외부에서 접근할 수 없고, 오직 Originator에서만
    접근할 수 있도록 합니다. (C++에서는 이를 위해, friend keyword 를 활용합니다.)
*/

int main()
{
    VirtualMachine vm;

    PrintVmInfo(vm);
    auto snapshot_1 = vm.TakeSnapshot();

    vm.ChangeCpuCount(16);
    vm.ChangeRamSize(1500);
    PrintVmInfo(vm);
    auto snapshot_2 = vm.TakeSnapshot();

    vm.ResetToSnapshot(snapshot_1);
    PrintVmInfo(vm);

    vm.ChangeCpuCount(4);
    vm.ChangeRamSize(6000);
    PrintVmInfo(vm);

    vm.ResetToSnapshot(snapshot_2);
    PrintVmInfo(vm);
}
