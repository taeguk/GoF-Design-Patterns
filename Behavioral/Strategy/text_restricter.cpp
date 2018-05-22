#include <iostream>
#include <memory>
#include <string>

class TextRestrictionStrategy
{
public:
    virtual ~TextRestrictionStrategy() = default;

    virtual bool Check(std::string const& text) const = 0;
};

class NoRestrictionStrategy : public TextRestrictionStrategy
{
public:
    bool Check(std::string const& text) const override
    {
        return true;
    }
};

class CurseRestrictionStrategy : public TextRestrictionStrategy
{
public:
    bool Check(std::string const& text) const override
    {
        auto npos = std::string::npos;

        if (text.find("fuck") != npos || text.find("shit") != npos)
        {
            return false;
        }
        
        return true;
    }
};

class LengthRestrictionStrategy : public TextRestrictionStrategy
{
public:
    LengthRestrictionStrategy(std::size_t minLength, std::size_t maxLength)
        : minLength_(minLength), maxLength_(maxLength)
    {}

    bool Check(std::string const& text) const override
    {
        auto length = text.length();

        if (length < minLength_ || length > maxLength_)
        {
            return false;
        }

        return true;
    }

private:
    std::size_t minLength_, maxLength_;
};

class TextRestricter
{
public:
    explicit TextRestricter(std::unique_ptr<TextRestrictionStrategy>&& strategy)
        : strategy_(std::move(strategy))
    {}

    void PrintText(std::string const& text) const
    {
        if (strategy_->Check(text))
        {
            std::cout << text << std::endl;
        }
        else
        {
            std::cout << "** Given text was restricted. **" << std::endl;
        }
    }

    bool ConcatText(std::string const& text_1,
                    std::string const& text_2,
                    std::string& concatedText) const
    {
        concatedText = text_1 + text_2;

        if (strategy_->Check(concatedText))
        {
            return true;
        }
        else
        {
            concatedText.clear();
            return false;
        }
    }

    void ChangeStrategy(std::unique_ptr<TextRestrictionStrategy>&& strategy)
    {
        strategy_ = std::move(strategy);
    }

private:
    std::unique_ptr<TextRestrictionStrategy> strategy_;
};

void ConcatAndPrintText(TextRestricter const& textRestricter,
                        std::string const& text_1,
                        std::string const& text_2)
{
    std::string concatedText;
    
    if (textRestricter.ConcatText(text_1, text_2, concatedText))
    {
        std::cout << concatedText << std::endl;
    }
    else
    {
        std::cout << "** Concated text was restricted. **" << std::endl;
    }
}

void Test(TextRestricter const& textRestricter)
{
    textRestricter.PrintText("fuck");
    textRestricter.PrintText("looooooooooooooooooooooooooong text");
    ConcatAndPrintText(textRestricter, "hi sh", "it bye");
}

/*
    Strategy Pattern은 동일 계열의 알고리즘 군을 정의하고, 그 것을 추상화합니다.
    알고리즘을 사용하는 클라이언트는 추상화된 인터페이스에만 의존하기 때문에,
    클라이언트와 독립적으로 구체적인 알고리즘들을 다양하게 변경할 수 있게 됩니다.
    또한, 런타임에 알고리즘을 변경하는 것이 가능해집니다.
    알고리즘의 세부 로직이 Concrete한 알고리즘 클래스로 응집되므로, 좀 더 구조화된
    형태로서 알고리즘 로직을 관리할 수 있게 되고, 대량의 조건문/switch 문을
    회피할 수 있게 됩니다.
*/
int main()
{
    TextRestricter textRestricter(std::make_unique<NoRestrictionStrategy>());

    std::cout << "[*] Test with 'NoRestrictionStrategy'." << std::endl;
    Test(textRestricter);

    std::cout << "\n[*] Test with 'CurseRestrictionStrategy'." << std::endl;
    textRestricter.ChangeStrategy(std::make_unique<CurseRestrictionStrategy>());
    Test(textRestricter);

    std::cout << "\n[*] Test with 'LengthRestrictionStrategy'." << std::endl;
    textRestricter.ChangeStrategy(std::make_unique<LengthRestrictionStrategy>(0, 8));
    Test(textRestricter);
}
