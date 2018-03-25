#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

enum class Task
{
    CustomerIssue,
    Programming,
    HardProgramming,
    MoneyIssue,
    M_And_A,
    ImpossibleTask
};

inline std::string GetNameOfTask(Task task)
{
    switch (task)
    {
    case Task::CustomerIssue:
        return "CustomerIssue";
    case Task::Programming:
        return "Programming";
    case Task::HardProgramming:
        return "HardProgramming";
    case Task::MoneyIssue:
        return "HardProgramming";
    case Task::M_And_A:
        return "HardProgramming";
    case Task::ImpossibleTask:
        return "HardProgramming";
    default:
        throw std::invalid_argument("");
    }
}

class Worker
{
public:
    explicit Worker(Worker* successor)
        : successor_(successor)
    {}

    virtual ~Worker() = default;

    void Work(Task task)
    {
        if (WorkImpl_(task))
        {
            return;
        }

        if (successor_)
        {
            successor_->Work(task);
        }
        else
        {
            std::cout << "Fail to handle task. (" << GetNameOfTask(task) << ")" << std::endl;
        }
    }

    Worker* ChangeSuccessor(Worker* successor)
    {
        Worker* old = successor_;
        successor_ = successor;
        return old;
    }

private:
    virtual bool WorkImpl_(Task task) = 0;

    Worker* successor_;
};

class CustomerSupporter : public Worker
{
public:
    using Worker::Worker;

private:
    bool WorkImpl_(Task task) override
    {
        switch (task)
        {
        case Task::CustomerIssue:
            std::cout << "[CustomerSupporter] Resolve customer issue." << std::endl;
            return true;

        default:
            return false;
        }
    }
};

class SoftwareEngineer : public Worker
{
public:
    using Worker::Worker;

    void Train()
    {
        isTrained_ = true;
    }

private:
    bool WorkImpl_(Task task) override
    {
        switch (task)
        {
        case Task::Programming:
            std::cout << "[SoftwareEngineer] Programming." << std::endl;
            return true;

        case Task::HardProgramming:
            if (isTrained_)
            {
                std::cout << "[SoftwareEngineer] Successfully solve hard problem!" << std::endl;
                return true;
            }
            else
            {
                std::cout << "[SoftwareEngineer] Try to solve hard problem. But failed." << std::endl;
                return false;
            }

        default:
            return false;
        }
    }

    bool isTrained_{ false };
};

class CEO : public Worker
{
public:
    using Worker::Worker;

private:
    bool WorkImpl_(Task task) override
    {
        switch (task)
        {
        case Task::MoneyIssue:
            std::cout << "[CEO] Resolve money issue." << std::endl;
            return true;

        case Task::M_And_A:
            std::cout << "[CEO] Do M&A." << std::endl;
            return true;

        default:
            return false;
        }
    }
};

class God : public Worker
{
public:
    using Worker::Worker;

private:
    bool WorkImpl_(Task) override
    {
        std::cout << "[God] God can do anything!" << std::endl;
        return true;
    }
};

int main()
{
    CEO ceo(nullptr);
    SoftwareEngineer se(&ceo);
    CustomerSupporter supporter{ &se };

    std::cout << "----------------------" << std::endl;
    supporter.Work(Task::CustomerIssue);
    supporter.Work(Task::Programming);
    supporter.Work(Task::HardProgramming);

    std::cout << "Train software engineer!" << std::endl;
    se.Train();
    supporter.Work(Task::HardProgramming);
    supporter.Work(Task::MoneyIssue);
    supporter.Work(Task::M_And_A);
    supporter.Work(Task::ImpossibleTask);

    std::cout << "Change ceo's successor to 'GOD'!" << std::endl;
    God god(nullptr);
    ceo.ChangeSuccessor(&god);
    supporter.Work(Task::ImpossibleTask);
}
