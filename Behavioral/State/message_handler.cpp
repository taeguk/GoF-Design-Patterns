#include <iostream>
#include <memory>
#include <string>

class MessageHandler
{
public:
    void HandleMessage(std::string const& message)
    {
        auto newState = state_->HandleMessage(message);

        if (newState)
        {
            state_ = std::move(newState);
        }
    }

private:
    class State
    {
    public:
        virtual ~State() = default;

        virtual std::unique_ptr<State> HandleMessage(std::string const& message) = 0;
    };

    class DefaultState : public State
    {
    public:
        std::unique_ptr<State> HandleMessage(std::string const& message) override;
    };

    class SessionState : public State
    {
    public:
        explicit SessionState(std::string sessionName)
            : sessionName_(std::move(sessionName))
        {}

        std::unique_ptr<State> HandleMessage(std::string const& message) override;

    private:
        std::string sessionName_;
    };

    ////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<State> state_{ std::make_unique<DefaultState>() };
};

std::unique_ptr<MessageHandler::State>
MessageHandler::DefaultState::HandleMessage(std::string const& message)
{
    std::string const kStartSessionCommand("start_session ");

    if (message.compare(0, kStartSessionCommand.size(), kStartSessionCommand) == 0)
    {
        auto it = std::next(std::begin(message), kStartSessionCommand.size());
        std::string sessionName(it, std::end(message));

        std::cout << "[Start Session] Session Name : " << sessionName << std::endl;
        return std::make_unique<SessionState>(std::move(sessionName));
    }
    else
    {
        std::cout << "\"" << message << "\" is invalid message." << std::endl;
        return nullptr;
    }
}

std::unique_ptr<MessageHandler::State>
MessageHandler::SessionState::HandleMessage(std::string const& message)
{
    std::string const kPrintCommand("print ");

    if (message == "end_session")
    {
        std::cout << "[" << sessionName_ << "][End Session]" << std::endl;

        return std::make_unique<DefaultState>();
    }
    else if (message.compare(0, kPrintCommand.size(), kPrintCommand) == 0)
    {
        auto it = std::next(std::begin(message), kPrintCommand.size());
        std::string text(it, std::end(message));

        std::cout << "[" << sessionName_ << "][Print] " << text << std::endl;
        return nullptr;
    }
    else
    {
        std::cout << "[" << sessionName_ << "] \"" <<
            message << "\" is invalid message." << std::endl;
        return nullptr;
    }
}


/*
    State Pattern은 객체의 내부 상태에 따라 스스로 행동을 변경할 수 있도록 해줍니다.
    각 상태별 행동들이 별도의 상태 클래스로 분리되므로, 좀 더 구조화된 형태로서 각 상태들을
    관리할 수 있게 되고, 대량의 조건문/switch 문을 회피할 수 있게 됩니다.
    Strategy Pattern과 유사한 점이 많은데, 차이점에 대해 잘 정리된 링크를 첨부합니다.
    (https://www.javabrahman.com/design-patterns/strategy-design-pattern-versus-state-design-pattern-analysis/)
*/
int main()
{
    MessageHandler messageHandler;

    messageHandler.HandleMessage("asdf");
    messageHandler.HandleMessage("print I'm taeguk.");
    messageHandler.HandleMessage("start_session Session_1");
    messageHandler.HandleMessage("print I'm taeguk.");
    messageHandler.HandleMessage("asdf");
    messageHandler.HandleMessage("end_session");
    messageHandler.HandleMessage("print better tomorrow");
    messageHandler.HandleMessage("start_session Session_2");
    messageHandler.HandleMessage("print better tomorrow");
}
