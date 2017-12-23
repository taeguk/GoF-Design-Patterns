#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Data
{
public:
    explicit Data(const std::string& name)
        : name_(name)
    {}
    virtual ~Data() = default;
    
    virtual std::uint64_t CalculateSize() const = 0;

    // These two functions (AddData, RemoveData) are only for Directory.
    // I add them into a common base class of both File and Directory
    //   even though they must not be called from File.
    // It is for getting extensibility and transparancy instead of type safety.
    virtual Data* AddData(std::unique_ptr<Data>&& data)
    {
        throw std::logic_error("AddData() is not supported operation.");
    }
    virtual bool RemoveData(Data* dataPtr)
    {
        throw std::logic_error("RemoveData() is not supported operation.");
    }

    std::string Name() const { return name_; }

private:
    std::string name_;
};

class Directory : public Data
{
public:
    explicit Directory(const std::string& name)
        : Data(name)
    {}

    virtual std::uint64_t CalculateSize() const override
    {
        std::uint64_t size = 0u;
        for (const auto& data : dataList_)
            size += data->CalculateSize();
        return size;
    }

    virtual Data* AddData(std::unique_ptr<Data>&& data) override
    {
        dataList_.push_back(std::move(data));
        return dataList_.back().get();
    }

    virtual bool RemoveData(Data* dataPtr) override
    {
        auto dataIter = std::find_if(
            std::begin(dataList_),
            std::end(dataList_), 
            [dataPtr](const auto& data) -> bool
            {
                return data.get() == dataPtr;
            });

        if (dataIter == std::end(dataList_))
            return false;

        dataList_.erase(dataIter);
        return true;
    }

private:
    std::vector<std::unique_ptr<Data>> dataList_;
};

class File : public Data
{
public:
    File(const std::string& name, std::uint64_t size)
        : Data(name), size_(size)
    {}

    virtual std::uint64_t CalculateSize() const override { return size_; }

private:
    std::uint64_t size_;
};

void PrintSize(const Data& data)
{
    std::cout << "Size of '" << data.Name() << "' = " << data.CalculateSize() << std::endl;
}

void RemoveData(Data& dir, Data* data)
{
    auto dataName = data->Name();
    bool success = dir.RemoveData(data);
    data = nullptr;

    if (success)
        std::cout << "Remove '" << dataName << "' from '" << dir.Name() << "'" << std::endl;
    else
        std::cout << "Fail to remove '" << dataName << "' from '" << dir.Name() << "'" << std::endl;
}

int main()
{
    auto dirRoot = Directory("Dir:/");
    auto dirA = dirRoot.AddData(std::make_unique<Directory>("Dir:/A/"));
    auto dirAA = dirA->AddData(std::make_unique<Directory>("Dir:/A/A/"));
    auto dirAB = dirAA->AddData(std::make_unique<Directory>("Dir:/A/B/"));
    auto dirB = dirRoot.AddData(std::make_unique<Directory>("Dir:/B/"));
    auto dirC = dirRoot.AddData(std::make_unique<Directory>("Dir:/C/"));

    auto file1 = dirRoot.AddData(std::make_unique<File>("File:/1", 1));
    auto fileA1 = dirA->AddData(std::make_unique<File>("File:/A/1", 10));
    auto fileA2 = dirA->AddData(std::make_unique<File>("File:/A/2", 100));
    auto fileAB1 = dirAB->AddData(std::make_unique<File>("File:/A/B/1", 1000));
    auto fileC1 = dirC->AddData(std::make_unique<File>("File:/C/1", 10000));
    auto fileC2 = dirC->AddData(std::make_unique<File>("File:/C/2", 100000));
    auto fileC3 = dirC->AddData(std::make_unique<File>("File:/C/3", 1000000));

    PrintSize(dirRoot);
    std::cout << "------------------" << std::endl;
    PrintSize(*dirA);
    PrintSize(*fileA2);
    RemoveData(*dirA, fileA2);
    PrintSize(*dirA);
    std::cout << "------------------" << std::endl;
    RemoveData(dirRoot, dirA);
    PrintSize(dirRoot);
    std::cout << "------------------" << std::endl;
    PrintSize(*dirB);
    PrintSize(*dirC);
    RemoveData(*dirC, fileC3);
    PrintSize(*dirC);
    std::cout << "------------------" << std::endl;
    RemoveData(dirRoot, fileC1);
    RemoveData(dirRoot, dirC);
    PrintSize(dirRoot);
}
