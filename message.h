#pragma once
#include <stdint.h>
#include <memory>
#include <vector>

namespace guodong {

#pragma pack(1)
struct MessageHeader {
    int32_t length_;
    int16_t type_;
    int32_t askid_;
    int32_t message_id_;
};
#pragma pack()

class Message {
 public:
    typedef std::shared_ptr<Message> Ptr;
    virtual ~Message();
    int32_t AskId() {
        return header_.askid_;
    }
    MessageHeader Header() {
        return header_;
    }
    std::vector<uint8_t> Body() {
        return body_;
    }
    int32_t Decode(uint8_t* data, int32_t length);
    void Encode(std::vector<uint8_t>& buffer);
    
 private:
    const int32_t header_size_ = 14;
    MessageHeader header_;
    std::vector<uint8_t> body_;
};

class MessageFactory {
 public:
    virtual ~MessageFactory() {};
    Message::Ptr BuildMessage();
};

} // namespace guodon class message