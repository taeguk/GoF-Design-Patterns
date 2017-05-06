#include <memory>
#include <string>
#include <iostream>

struct Color
{
	Color(std::uint8_t R, std::uint8_t G, std::uint8_t B)
		: R(R), G(G), B(B)
	{}
	std::uint8_t R, G, B;
};

struct Theme
{
	virtual std::string Name() const = 0;
	virtual Color WallColor() const = 0;
	virtual Color FloorColor() const = 0;
	virtual Color CeilingColor() const = 0;
};

struct WhiteTheme : public Theme
{
	virtual std::string Name() const override { return "White Theme"; }
	virtual Color WallColor() const override { return Color(255, 255, 255); }
	virtual Color FloorColor() const override { return Color(255, 255, 255); }
	virtual Color CeilingColor() const override { return Color(255, 255, 255); }
};

struct DarkTheme : public Theme
{
	virtual std::string Name() const override { return "Dark Theme"; }
	virtual Color WallColor() const override { return Color(0, 0, 0); }
	virtual Color FloorColor() const override { return Color(0, 0, 0); }
	virtual Color CeilingColor() const override { return Color(0, 0, 0); }
};

struct CustomTheme : public Theme
{
	CustomTheme(std::string name, Color wallColor, Color floorColor, Color ceilingColor)
		: m_name(name), m_wallColor(wallColor), m_floorColor(floorColor), m_ceilingColor(ceilingColor)
	{}

	virtual std::string Name() const override { return m_name; }
	virtual Color WallColor() const override { return m_wallColor; }
	virtual Color FloorColor() const override { return m_floorColor; }
	virtual Color CeilingColor() const override { return m_ceilingColor; }

private:
	std::string m_name;
	Color m_wallColor, m_floorColor, m_ceilingColor;
};

class House
{
public:
	House(const std::shared_ptr<Theme>& theme)
		: m_theme(theme)
	{}

	virtual std::string Name() const = 0;
	virtual void ShowDescription() const = 0;
	void ChangeTheme(const std::shared_ptr<Theme>& theme) 
	{
		std::cout << Name() << " changes theme to '" << theme->Name() << "'." << std::endl;
		m_theme = theme; 
	}

protected:
	const std::shared_ptr<Theme>& GetTheme() const { return m_theme; }

private:
	std::shared_ptr<Theme> m_theme;
};

class OneRoom : public House
{
public:
	OneRoom(const std::shared_ptr<Theme>& theme)
		: House(theme)
	{}

	virtual std::string Name() const override { return "One Room"; }
	virtual void ShowDescription() const override
	{
		auto theme = GetTheme();
		std::cout << Name() << " with theme('" << theme->Name() << "')." << std::endl;
	};
};

class Apartment : public House
{
public:
	Apartment(const std::shared_ptr<Theme>& theme)
		: House(theme)
	{}

	virtual std::string Name() const override { return "Apartment"; }
	virtual void ShowDescription() const override
	{
		auto theme = GetTheme();
		std::cout << Name() << " with theme('" << theme->Name() << "')." << std::endl;
	};
};

class Castle : public House
{
public:
	Castle(const std::shared_ptr<Theme>& theme)
		: House(theme)
	{}

	virtual std::string Name() const override { return "Castle"; }
	virtual void ShowDescription() const override
	{
		auto theme = GetTheme();
		std::cout << Name() << " with theme('" << theme->Name() << "')." << std::endl;
	};
};

/*
	Bridge Pattern은 두 개의 클래스 계통을 분리시킬 수 있는 장점이 있습니다.
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

	Bridge 패턴은 interface와 implementation을 분리하는 용도로도 사용가능합니다.
	이 경우, 사용자에게 상세한 구현 내용을 은닉할 수 있는 장점이 있습니다. 
	또한, interface와 implementation 사이의 종속 관계를 피할 수 있게 되고, 
	런타임에 implementation을 변경하는 것이 가능해지는 등의 장점이 있습니다.
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