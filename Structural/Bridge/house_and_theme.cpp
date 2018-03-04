#include <iostream>
#include <memory>
#include <string>

struct Color
{
    Color(std::uint8_t R, std::uint8_t G, std::uint8_t B)
        : R(R), G(G), B(B)
    {}

    std::uint8_t R, G, B;
};

class Theme
{
public:
    virtual ~Theme() = default;

    virtual std::string Name() const = 0;
    virtual Color WallColor() const = 0;
    virtual Color FloorColor() const = 0;
    virtual Color CeilingColor() const = 0;
};

class WhiteTheme : public Theme
{
public:
    std::string Name() const override { return "White Theme"; }
    Color WallColor() const override { return { 255, 255, 255 }; }
    Color FloorColor() const override { return { 255, 255, 255 }; }
    Color CeilingColor() const override { return { 255, 255, 255 }; }
};

class DarkTheme : public Theme
{
public:
    std::string Name() const override { return "Dark Theme"; }
    Color WallColor() const override { return { 0, 0, 0 }; }
    Color FloorColor() const override { return { 0, 0, 0 }; }
    Color CeilingColor() const override { return { 0, 0, 0 }; }
};

class CustomTheme : public Theme
{
public:
    CustomTheme(std::string name, Color wallColor, Color floorColor, Color ceilingColor)
        : name_(std::move(name)), wallColor_(wallColor), floorColor_(floorColor), ceilingColor_(ceilingColor)
    {}

    std::string Name() const override { return name_; }
    Color WallColor() const override { return wallColor_; }
    Color FloorColor() const override { return floorColor_; }
    Color CeilingColor() const override { return ceilingColor_; }

private:
    std::string name_;
    Color wallColor_, floorColor_, ceilingColor_;
};

class House
{
public:
    explicit House(std::shared_ptr<Theme> theme)
        : theme_(std::move(theme))
    {}
    virtual ~House() = default;

    virtual std::string Name() const = 0;

    void ChangeTheme(std::shared_ptr<Theme> const& theme) 
    {
        std::cout << Name() << " changes theme to '" << theme->Name() << "'." << std::endl;
        theme_ = theme; 
    }

    void ShowDescription() const
    {
        std::cout << Name() << " with theme('" << theme_->Name() << "')." << std::endl;
    }

private:
    std::shared_ptr<Theme> theme_;
};

class OneRoom : public House
{
public:
    using House::House;

    std::string Name() const override { return "One Room"; }
};

class Apartment : public House
{
public:
    using House::House;

    std::string Name() const override { return "Apartment"; }
};

class Castle : public House
{
public:
    using House::House;

    std::string Name() const override { return "Castle"; }
};

/*
    Bridge 패턴은 interface와 implementation을 분리하는 용도로 사용됩니다.
    사용자에게 상세한 구현 내용을 은닉할 수 있는 장점이 있습니다. 
    또한, interface와 implementation 사이의 종속 관계를 피할 수 있게 되고, 
    런타임에 implementation을 변경하는 것이 가능해지는 등의 장점이 있습니다.

    만약, 이 예제에서 bridge 패턴을 사용하지 않았다면,

           House
             |
   --------------------
   |         |        |
OneRoom  Apartment  Castle
   |         |        |
  ...       ...   ---------------------------
                  |            |            |
             WhiteCastle  DarkCastle  CustomCastle

    와 같이 House 클래스 계통이 엄청나게 커졌을 것입니다.
    따라서 bridge 패턴을 이용함으로서, House 클래스 계통과 Theme 클래스 계통이 분리되게 되고,
    이에 따라 House 계통과 Theme 계통을 독립적으로 확장할 수 있습니다.
    또한, 런타임에 theme를 변경하는 것이 가능해지고, 
    동일한 theme를 여러 개의 house에서 공유하는 것이 가능해집니다.
*/

int main()
{
    OneRoom oneRoom(std::make_shared<DarkTheme>());
    oneRoom.ShowDescription();
    oneRoom.ChangeTheme(std::make_shared<WhiteTheme>());
    oneRoom.ShowDescription();

    Apartment apartment(std::make_shared<WhiteTheme>());
    apartment.ShowDescription();

    auto customTheme = std::make_shared<CustomTheme>("My Custom Theme", 
        Color(12, 34, 56), Color(78, 90, 12), Color(34, 56, 78));

    Castle castle(customTheme);
    castle.ShowDescription();
    
    apartment.ChangeTheme(customTheme);
    apartment.ShowDescription();
}
