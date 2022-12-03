#pragma once
#include <memory>
#include <vector>
#include <stdint.h>

namespace guodong {

#pragma pack(1)
struct MessageHeader {
    int32_t length;
    int16_t type;
    int32_t askid;
    int32_t message_id;
};
#pragma pack()

class Message {
public:
    using Ptr = std::shared_ptr<Message>;
    Message();
    virtual ~Message();
    MessageHeader Header() { return header_; }
    int16_t Type() { return header_.type; }
    void SetType(int16_t type) { header_.type = type; }
    int32_t AskId() { return header_.askid; }
    void SetAskId(int32_t askid) { header_.askid = askid; }
    int32_t Length() { return header_.length; }
    void SetLength(int32_t len) { header_.length = len; }
    int32_t MessageId() { return header_.message_id; }
    void SetMessageId(int32_t messageid) { header_.message_id = messageid; }
    std::shared_ptr<std::vector<uint8_t>> Body() { return body_; }
    void SetBody(const std::vector<uint8_t>& buff) {
        body_->insert(body_->end(), buff.begin(), buff.end());
        header_.length = body_->size();
    }
    int32_t Decode(uint8_t* data, int32_t length);
    void Encode(std::vector<uint8_t>& buffer);

private:
    const int32_t header_size_ = 14;
    MessageHeader header_;
    std::shared_ptr<std::vector<uint8_t>> body_;
};

class MessageFactory {
public:
    virtual ~MessageFactory(){};
    Message::Ptr BuildMessage();
};

}  // namespace guodong