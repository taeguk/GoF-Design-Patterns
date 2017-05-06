#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class Directory;

class Data
{
public:
	Data(std::string name, const std::shared_ptr<Directory>& parent)
		: m_name(std::move(name)), m_parent(parent)
	{}
	
	void Delete();
	int Size() const
	{
		if (m_deleted)
		{
			std::cout << "Cannot get the size of '" << m_name << "' because it was deleted." << std::endl;
			throw std::logic_error("Cannot get the size because data was deleted.");
		}
		return SizeImpl();
	}

	std::string Name() const { return m_name; }

private:
	virtual void DeleteImpl() = 0;
	virtual int SizeImpl() const = 0;

	std::string m_name;
	std::weak_ptr<Directory> m_parent;
	bool m_deleted{ false };
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
		m_dataList.push_back(data);
	}

	void DeleteData(Data* dataPtr)
	{
		auto dataIter = std::find_if(std::begin(m_dataList), std::end(m_dataList), 
			[dataPtr](const auto& data) {
				return data.get() == dataPtr;
		});
		m_dataList.erase(dataIter);
	}

private:
	virtual void DeleteImpl() override
	{
		/* // This is wrong.
		for (const auto& data : m_dataList)
			data->Delete();
		*/
		while (!m_dataList.empty())
			m_dataList[0]->Delete();
	}

	virtual int SizeImpl() const override
	{
		int size = 0;
		for (const auto& data : m_dataList)
			size += data->Size();
		return size;
	}

	std::vector<std::shared_ptr<Data>> m_dataList;
};

class File : public Data
{
	struct PrivateKey {};

public:
	File(PrivateKey, const std::string& name, const std::shared_ptr<Directory>& parent, int size)
		: Data(name, parent), m_size(size)
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
	virtual void DeleteImpl() override {}
	virtual int SizeImpl() const override { return m_size; }

	int m_size;
};

void Data::Delete()
{
	if (m_deleted)
	{
		std::cout << "'" << m_name << "' is already deleted." << std::endl;
	}
	else
	{
		DeleteImpl();
		m_deleted = true;
		if (auto parent = m_parent.lock())
			parent->DeleteData(this);
		std::cout << "'" << m_name << "' is successfully deleted." << std::endl;
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