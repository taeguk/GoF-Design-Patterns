#ifndef ACCESS_KEY_H
#define ACCESS_KEY_H

namespace design
{
template <typename Accessor>
class AccessKey
{
private:
    friend Accessor;

    AccessKey() = default;
};

} // namespace design

#endif
