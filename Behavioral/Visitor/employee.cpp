#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

class SoftwareEngineer;
class SalesPerson;
class CustomerSupporter;

class EmployeeVisitor
{
public:
    virtual ~EmployeeVisitor() = default;

    virtual void VisitSoftwareEngineer(SoftwareEngineer& softwareEngineer) = 0;
    virtual void VisitSalesPerson(SalesPerson& salesPerson) = 0;
    virtual void VisitCustomerSupporter(CustomerSupporter& customerSupporter) = 0;
};

class Employee
{
public:
    virtual ~Employee() = default;

    virtual void Accept(EmployeeVisitor& visitor) = 0;
};

class SoftwareEngineer : public Employee
{
public:
    void Accept(EmployeeVisitor& visitor) override { visitor.VisitSoftwareEngineer(*this); }

    /* hard coded value for simplicity of example */
    std::uint64_t GetSoftwareQuality() const { return 40; }
    std::uint64_t GetBusinessComprehension() const { return 20; }
};

class SalesPerson : public Employee
{
public:
    void Accept(EmployeeVisitor& visitor) override { visitor.VisitSalesPerson(*this); }

    /* hard coded value for simplicity of example */
    std::uint64_t GetSalesVolume() const { return 8000; }
};

class CustomerSupporter : public Employee
{
public:
    void Accept(EmployeeVisitor& visitor) override { visitor.VisitCustomerSupporter(*this); }

    /* hard coded value for simplicity of example */
    std::uint64_t GetCustomerSatisfaction() const { return 60; }
};

class IncentiveCalculationVisitor : public EmployeeVisitor
{
public:
    void VisitSoftwareEngineer(SoftwareEngineer& softwareEngineer) override
    {
        totalIncentive_ += softwareEngineer.GetSoftwareQuality() * 2 +
                           softwareEngineer.GetBusinessComprehension();
    }

    void VisitSalesPerson(SalesPerson& salesPerson) override
    {
        totalIncentive_ += static_cast<std::uint64_t>(salesPerson.GetSalesVolume() * 0.01);
    }

    void VisitCustomerSupporter(CustomerSupporter& customerSupporter) override
    {
        totalIncentive_ += customerSupporter.GetCustomerSatisfaction();
    }

    std::uint64_t GetTotalIncentive() const { return totalIncentive_; }

private:
    std::uint64_t totalIncentive_{ 0u };
};

class PrintInformationVisitor : public EmployeeVisitor
{
public:
    void VisitSoftwareEngineer(SoftwareEngineer& softwareEngineer) override
    {
        std::cout << "--- Information of Software Engineer ---" << std::endl;
        std::cout << "Software Quality       : " << softwareEngineer.GetSoftwareQuality() << std::endl;
        std::cout << "Business Comprehension : " << softwareEngineer.GetBusinessComprehension() << std::endl;
    }

    void VisitSalesPerson(SalesPerson& salesPerson) override
    {
        std::cout << "--- Information of Sales Person ---" << std::endl;
        std::cout << "Sales Volume : " << salesPerson.GetSalesVolume() << std::endl;
    }

    void VisitCustomerSupporter(CustomerSupporter& customerSupporter) override
    {
        std::cout << "--- Information of Customer Supporter ---" << std::endl;
        std::cout << "Customer Satisfaction : " << customerSupporter.GetCustomerSatisfaction() << std::endl;
    }
};

void Calculate(const std::vector<Employee*>& employeeList,
               const std::vector<EmployeeVisitor*>& visitorList)
{
    for (auto employee : employeeList)
    {
        for (auto visitor : visitorList)
        {
            employee->Accept(*visitor);
        }
    }
}

/*
    Visitor Pattern은 알고리즘을 객체 구조에서 분리시키는 패턴으로서, 연산을 적용할
    원소의 클래스를 변경하지 않고도 새로운 연산을 정의할 수 있도록 해줍니다.
    객체 구조 (클래스 계통) 에 대한 특정 연산들이 모두 하나의 Visitor 클래스에
    모아지는 특징이 있습니다. (경우에 따라 장점일 수도 단점일 수도 있습니다.)

    전통적 객체지향 디자인에서는, 클래스 계통에 연산을 추가해야할 경우 모든 클래스에
    가상함수를 하나 추가해야합니다. 그리고, 클래스를 추가해야할 경우, 새로운 클래스를
    하나 만들면 됩니다.
    Visitor Pattern 에서는 반대의 현상이 나타납니다. 클래스 계통에 연산을 추가해야할
    경우 새로운 Visitor 클래스를 하나 만들면 되고, 클래스를 추가할 경우, 모든
    Visitor 클래스들에 대해 가상함수를 하나 추가해야합니다.
    즉, Visitor Pattern은 객체 구조를 정의한 클래스는 거의 변하지 않지만, 새로운
    연산이 추가될 가능성이 클 경우 유리합니다.

    Visitor Pattern은 Accept() 연산에서 이중 디스패치 (Double Dispatch) 기법을
    사용합니다. Visitor 객체의 타입과 Element 객체의 타입 모두에 따라 실제 수행될
    연산이 결정됩니다.
*/
int main()
{
    //
    SoftwareEngineer se_1, se_2;
    SalesPerson sp_1, sp_2, sp_3;
    CustomerSupporter cs_1;

    PrintInformationVisitor printInformationVisitor;
    IncentiveCalculationVisitor incentiveCalculationVisitor;

    //
    std::vector<Employee*> employeeList;

    employeeList.push_back(&se_1); employeeList.push_back(&se_2);
    employeeList.push_back(&sp_1); employeeList.push_back(&sp_2); employeeList.push_back(&sp_3);
    employeeList.push_back(&cs_1);

    std::vector<EmployeeVisitor*> visitorList;
    visitorList.push_back(&printInformationVisitor);
    visitorList.push_back(&incentiveCalculationVisitor);

    //
    Calculate(employeeList, visitorList);

    //
    std::cout << "\n*** Total Incentive : " << incentiveCalculationVisitor.GetTotalIncentive() << std::endl;
}
