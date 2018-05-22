#include <iostream>
#include <string>

/* Receiver (실제 요청을 처리할 책임을 지는 객체) */
class WebPageManager
{
public:
    void ChangeWebPage(std::string const& webPage)
    {
        std::cout << "Change web page to " << webPage << "." << std::endl;
    }
};

class Command
{
public:
    virtual ~Command() = default;

    virtual void Execute() = 0;
};

class ChangeWebPageCommand : public Command
{
public:
    ChangeWebPageCommand(WebPageManager& pageManager, std::string webPage)
        : pageManager_(pageManager), webPage_(std::move(webPage))
    {}

    void Execute() override
    {
        pageManager_.ChangeWebPage(webPage_);
    }

private:
    WebPageManager& pageManager_;
    std::string webPage_;
};

/* 템플릿을 활용해서, 임의의 Receiver에게 명령을 전달할 수 있도록 한다. */
template <typename Receiver>
class GenericCommand : public Command
{
public:
    explicit GenericCommand(Receiver&& receiver)
        : receiver_(std::move(receiver))
    {}

    void Execute() override
    {
        receiver_();
    }

private:
    Receiver receiver_;
};

template <typename Receiver>
auto MakeGenericCommand(Receiver&& receiver)
    -> GenericCommand<Receiver>
{
    return GenericCommand<Receiver>(std::move(receiver));
}

class Button
{
public:
    explicit Button(Command* command)
        : command_(command)
    {}

    void Click()
    {
        if (command_)
        {
            command_->Execute();
        }
    }

    Command* ChangeCommand(Command* command)
    {
        Command* old = command_;
        command_ = command;
        return old;
    }

private:
    Command* command_;
};

/*
    Command Pattern은 요청 자체를 캡슐화하기 위해 사용합니다.
    Invoker (명령을 요청하는 측) 은 Receiver (실제 명령을 수행하는 측) 에 대한 어떠한
    구체적인 정보를 알고 있지 않아도 됩니다. 즉, Invoker와 Receiver의 결합을 없애는
    효과가 있습니다.
    Receiver에 대한 구체적인 정보와 의존성은 Concrete Command 클래스에 들어가게 되고,
    추상화된 베이스 클래스인 Command 클래스는 Receiver에 대한 의존성을 가지지 않습니다.
    따라서, Invoker 측은 Receiver에 대한 직접적인 의존성 없이 명령을 요청할 수 있게 됩니다.
*/

int main()
{
    WebPageManager webPageManager;
    ChangeWebPageCommand changeWebPageCommand(webPageManager, "http://taeguk.me");

    Button button(&changeWebPageCommand);
    button.Click();

    auto genericCommand = MakeGenericCommand(
        []() -> void
        {
            std::cout << "This is generic command." << std::endl;
        });

    button.ChangeCommand(&genericCommand);
    button.Click();
}
