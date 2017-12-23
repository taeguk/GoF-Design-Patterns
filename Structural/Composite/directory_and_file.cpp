#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Directory;

class Data
{
public:
    Data(std::string name, const std::shared_ptr<Directory>& parent)
        : name_(std::move(name)), parent_(parent)
    {}
    virtual ~Data() = default;
    
    void Delete();
    int Size() const
    {
        if (deleted_)
        {
            std::cout << "Cannot get the size of '" << name_ << "' because it was deleted." << std::endl;
            throw std::logic_error("Cannot get the size because data was deleted.");
        }
        return SizeImpl_();
    }

    std::string Name() const { return name_; }

private:
    virtual void DeleteImpl_() = 0;
    virtual int SizeImpl_() const = 0;

    std::string name_;
    std::weak_ptr<Directory> parent_;
    bool deleted_{ false };
};

class Directory : public Data
{
    struct PrivateKey {};

public:
    Directory(PrivateKey, const std::string& name, const std::shared_ptr<Directory>& parent)
        : Data(name, parent)
    {}

    static std::shared_ptr<Directory> Create(const std::string& name, const std::shared_ptr<Directory>& parent)
    {
        auto dir = std::make_shared<Directory>(PrivateKey(), name, parent);
        if (parent)
            parent->AddData(dir);
        return dir;
    }

    void AddData(const std::shared_ptr<Data>& data)
    {
        dataList_.push_back(data);
    }

    void DeleteData(Data* dataPtr)
    {
        auto dataIter = std::find_if(std::begin(dataList_), std::end(dataList_), 
            [dataPtr](const auto& data) {
                return data.get() == dataPtr;
        });
        dataList_.erase(dataIter);
    }

private:
    virtual void DeleteImpl_() override
    {
        /* // This is wrong.
        for (const auto& data : dataList_)
            data->Delete();
        */
        while (!dataList_.empty())
            dataList_[0]->Delete();
    }

    virtual int SizeImpl_() const override
    {
        int size = 0;
        for (const auto& data : dataList_)
            size += data->Size();
        return size;
    }

    std::vector<std::shared_ptr<Data>> dataList_;
};

class File : public Data
{
    struct PrivateKey {};

public:
    File(PrivateKey, const std::string& name, const std::shared_ptr<Directory>& parent, int size)
        : Data(name, parent), size_(size)
    {}

    static std::shared_ptr<File> Create(const std::string& name, const std::shared_ptr<Directory>& parent, int size)
    {
        if (!parent)
            throw std::logic_error("A file must have parent.");
        auto file = std::make_shared<File>(PrivateKey(), name, parent, size);
        parent->AddData(file);
        return file;
    }

private:
    virtual void DeleteImpl_() override {}
    virtual int SizeImpl_() const override { return size_; }

    int size_;
};

void Data::Delete()
{
    if (deleted_)
    {
        std::cout << "'" << name_ << "' is already deleted." << std::endl;
    }
    else
    {
        DeleteImpl_();
        deleted_ = true;
        if (auto parent = parent_.lock())
            parent->DeleteData(this);
        std::cout << "'" << name_ << "' is successfully deleted." << std::endl;
    }
}

void PrintSize(const std::shared_ptr<Data>& data)
{
    std::cout << "Size of '" << data->Name() << "' = " << data->Size() << std::endl;
}

int main()
{
    auto dirRoot = Directory::Create("Dir:/", nullptr);
    auto dirA = Directory::Create("Dir:/A/", dirRoot);
    auto dirAA = Directory::Create("Dir:/A/A/", dirA);
    auto dirAB = Directory::Create("Dir:/A/B/", dirA);
    auto dirB = Directory::Create("Dir:/B/", dirRoot);
    auto dirC = Directory::Create("Dir:/C/", dirRoot);

    auto file1 = File::Create("File:/1", dirRoot, 1);
    auto fileA1 = File::Create("File:/A/1", dirA, 10);
    auto fileA2 = File::Create("File:/A/2", dirA, 100);
    auto fileAB1 = File::Create("File:/A/B/1", dirAB, 1000);
    auto fileC1 = File::Create("File:/C/1", dirC, 10000);
    auto fileC2 = File::Create("File:/C/2", dirC, 100000);
    auto fileC3 = File::Create("File:/C/3", dirC, 1000000);

    PrintSize(dirRoot);
    std::cout << "------------------" << std::endl;
    PrintSize(dirA);
    PrintSize(fileA2);
    fileA2->Delete();
    PrintSize(dirA);
    std::cout << "------------------" << std::endl;
    dirA->Delete();
    std::cout << "------------------" << std::endl;
    fileAB1->Delete();
    std::cout << "------------------" << std::endl;
    PrintSize(dirRoot);
    dirRoot->Delete();
    std::cout << "------------------" << std::endl;
    dirRoot->Delete();
    std::cout << "------------------" << std::endl;
}
