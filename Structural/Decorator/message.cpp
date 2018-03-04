#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <string>

class Message
{
public:
    virtual ~Message() = default;

    virtual void PrintLayer() const = 0;
    virtual std::string Serialize() const = 0;
};

class TextMessage : public Message
{
public:
    explicit TextMessage(std::string content)
        : content_(std::move(content))
    {}

    void PrintLayer() const override
    {
        std::cout << "Text Message : " << content_ << std::endl;
    }

    std::string Serialize() const override
    {
        return content_;
    }

private:
    std::string content_;
};

class ImageMessage : public Message
{
public:
    explicit ImageMessage(std::string imageFileName)
        : imageFileName_(std::move(imageFileName))
    {}

    void PrintLayer() const override
    {
        std::cout << "Image Message : " << imageFileName_ << std::endl;
    }

    std::string Serialize() const override
    {
        return "Binary Data Of " + imageFileName_;
    }

private:
    std::string imageFileName_;
};

class MessageDecorator : public Message
{
public:
    explicit MessageDecorator(std::unique_ptr<Message>&& message)
        : message_(std::move(message))
    {}

    /* 기존의 로직이 반드시 수행되는 걸 보장하기 위해서 NVI 패턴 사용 */
    void PrintLayer() const final
    {
        message_->PrintLayer();
        PrintLayerImpl_();
    }

    /* 기존의 로직이 반드시 수행되는 걸 보장하기 위해서 NVI 패턴 사용 */
    std::string Serialize() const final
    {
        std::string buffer = message_->Serialize();
        SerializeImpl_(buffer);
        return buffer;
    }

private:
    virtual void PrintLayerImpl_() const = 0;
    virtual void SerializeImpl_(std::string& buffer) const = 0;

    std::unique_ptr<Message> message_;
};

class EncryptionMessageDecorator : public MessageDecorator
{
public:
    using MessageDecorator::MessageDecorator;

private:
    void PrintLayerImpl_() const override
    {
        std::cout << "Encrypt Message" << std::endl;
    }

    void SerializeImpl_(std::string& buffer) const override
    {
        std::rotate(
            std::begin(buffer),
            std::next(std::begin(buffer), buffer.length() / 2),
            std::end(buffer));
    }
};

class CompressionMessageDecorator : public MessageDecorator
{
public:
    using MessageDecorator::MessageDecorator;

private:
    void PrintLayerImpl_() const override
    {
        std::cout << "Compress Message" << std::endl;
    }

    void SerializeImpl_(std::string& buffer) const override
    {
        buffer.erase(
            std::unique(std::begin(buffer), std::end(buffer)),
            std::end(buffer));
    }
};

int main()
{
    std::queue<std::unique_ptr<Message>> messageQueue;

    messageQueue.push(
        std::make_unique<EncryptionMessageDecorator>(
        std::make_unique<CompressionMessageDecorator>(
        std::make_unique<TextMessage>(
            "Helloooooo, I'm taeguk. Feel so gooooooood!"))));

    messageQueue.push(
        std::make_unique<EncryptionMessageDecorator>(
        std::make_unique<ImageMessage>(
            "This-is-image-file-name.PNG")));

    while (!messageQueue.empty())
    {
        auto message = std::move(messageQueue.front()); messageQueue.pop();

        std::cout << "---------------------------------------" << std::endl;
        message->PrintLayer();
        std::cout << "Serialized Data : " << message->Serialize() << '\n' << std::endl;
    }
}
