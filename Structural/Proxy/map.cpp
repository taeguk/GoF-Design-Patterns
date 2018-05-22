#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

struct Resource
{
    Resource() { std::cout << "* Load Resource. (Heavy Work) *" << std::endl; }
};

class MapTile
{
public:
    MapTile(double centerX, double centerY, double width, double height)
        : centerX_(centerX), centerY_(centerY), width_(width), height_(height)
    {
        // Load resource. (Heavy Work)
    }

    void Draw() const
    {
        std::cout << "* Draw MapTile from resouce. *" << std::endl;
    }

    double CenterX() const { return centerX_; }
    double CenterY() const { return centerY_; }
    double Width() const { return width_; }
    double Height() const { return height_; }

private:
    double centerX_, centerY_;
    double width_, height_;
    Resource resource_;
};

/* Virtual Proxy */
class MapTileProxy
{
public:
    MapTileProxy(double centerX, double centerY, double width, double height)
        : centerX_(centerX), centerY_(centerY), width_(width), height_(height)
    {}

    void Draw() const
    {
        if (realMapTile_ == nullptr)
        {
            realMapTile_ = std::make_unique<MapTile>(centerX_, centerY_, width_, height_);
        }
        realMapTile_->Draw();
    }

    double CenterX() const { return centerX_; }
    double CenterY() const { return centerY_; }
    double Width() const { return width_; }
    double Height() const { return height_; }

private:
    double centerX_, centerY_;
    double width_, height_;
    mutable std::unique_ptr<MapTile> realMapTile_;
};

class Map
{
public:
    Map(double left, double top, double right, double bottom, std::size_t tileCountX, std::size_t tileCountY)
    {
        assert(tileCountX >= 1);
        assert(tileCountY >= 1);

        double width = (right - left) / tileCountX;
        double height = (bottom - top) / tileCountY;

        for (std::size_t i = 0; i < tileCountX; ++i)
        {
            for (std::size_t j = 0; j < tileCountY; ++j)
            {
                mapTileList_.emplace_back(width / 2 + width * i, height / 2 + height * j, width, height);
            }
        }
    }

    void Print(bool draw) const
    {
        for (auto const& mapTile : mapTileList_)
        {
            std::cout << "----- MapTile -----" << std::endl;
            std::cout << "Center Position       : (" << mapTile.CenterX() << ", " << mapTile.CenterY() << ")" << std::endl;
            std::cout << "Size (Width x Height) : " << mapTile.Width() << "x" << mapTile.Height() << std::endl;
            if (draw) mapTile.Draw();
            std::cout << "-------------------" << std::endl;
        }
    }

private:
    std::vector<MapTileProxy> mapTileList_;
};

/*
    "상속기반의 Proxy Pattern"과 Decorator Pattern은 구현 방법이 유사한데,
    Decorator Pattern은 하나 이상의 로직을 추가하기 위한 목적을 가지고 있고,
    Proxy Pattern은 실제 객체에 대한 접근을 제어하기 위한 목적을 가진다는 차이가 있습니다.

    따라서, Decorator Pattern에서는 기존의 로직이 반드시 수행되는 걸 보장하기 위해서
    NVI(Non Virtual Interface) 패턴을 활용할 수 있지만,
    Proxy Pattern에서는 기존의 로직에 대한 접근을 제어해야하기 때문에, NVI패턴을 사용하지 않습니다.
*/

int main()
{
    Map map(0, 0, 60, 90, 2, 2);

    map.Print(false);
    std::cout << std::endl << "@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl << std::endl;
    map.Print(true);
    std::cout << std::endl << "@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl << std::endl;
    map.Print(true);
}
