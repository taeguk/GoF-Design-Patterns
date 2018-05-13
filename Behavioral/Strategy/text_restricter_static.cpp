#include <iostream>
#include <memory>
#include <string>

class NoRestrictionStrategy
{
public:
    bool Check(std::string const& text) const
    {
        return true;
    }
};

class CurseRestrictionStrategy
{
public:
    bool Check(std::string const& text) const
    {
        auto npos = std::string::npos;

        if (text.find("fuck") != npos || text.find("shit") != npos)
        {
            return false;
        }
        
        return true;
    }
};

class LengthRestrictionStrategy
{
public:
    LengthRestrictionStrategy(std::size_t minLength, std::size_t maxLength)
        : minLength_(minLength), maxLength_(maxLength)
    {}

    bool Check(std::string const& text) const
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

/*
    런타임에 전략을 변경할 필요가 없는 경우, 템플릿을 활용하여 정적으로 전략이
    binding 되도록 할 수 있습니다.
    이 경우, 비록 유연성은 떨어지지만, 가상 함수 호출에 따른 overhead가
    발생하지 않는 이점이 있습니다.
*/
template <typename TextRestrictionStrategy>
class TextRestricter
{
public:
    explicit TextRestricter(TextRestrictionStrategy&& strategy)
        : strategy_(std::move(strategy))
    {}

    void PrintText(std::string const& text) const
    {
        if (strategy_.Check(text))
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

        if (strategy_.Check(concatedText))
        {
            return true;
        }
        else
        {
            concatedText.clear();
            return false;
        }
    }

private:
    TextRestrictionStrategy strategy_;
};

template <typename TextRestrictionStrategy>
auto MakeTextRestrictor(TextRestrictionStrategy&& strategy)
    -> TextRestricter<TextRestrictionStrategy>
{
    return TextRestricter<TextRestrictionStrategy>(std::move(strategy));
}

template <typename TextRestricter>
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

template <typename TextRestricter>
void Test(TextRestricter const& textRestricter)
{
    textRestricter.PrintText("fuck");
    textRestricter.PrintText("looooooooooooooooooooooooooong text");
    ConcatAndPrintText(textRestricter, "hi sh", "it bye");
}

int main()
{
    std::cout << "[*] Test with 'NoRestrictionStrategy'." << std::endl;
    Test(MakeTextRestrictor(NoRestrictionStrategy()));

    std::cout << "\n[*] Test with 'CurseRestrictionStrategy'." << std::endl;
    Test(MakeTextRestrictor(CurseRestrictionStrategy()));

    std::cout << "\n[*] Test with 'LengthRestrictionStrategy'." << std::endl;
    Test(MakeTextRestrictor(LengthRestrictionStrategy(0, 8)));
}
