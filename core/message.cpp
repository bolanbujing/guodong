#include "log.h"
#include "message.h"

using namespace guodong;

Message::~Message() {}

int32_t Message::Decode(uint8_t* data, int32_t length) {
    if (length > 1024 * 1024 * 100) {
        LOG(ERROR) << LOG_KV("message length is", length);
        return -1;
    }
    if (length < header_size_) {
        return 0;
    } else {
        auto len = *((int32_t*)data);
        if (length < len) {
            return 0;
        }
        header_.length_ = len;
        header_.type_ = *((int16_t*)(data + 4));
        header_.askid_ = *((int32_t*)(data + 6));
        header_.message_id_ = *((int32_t*)(data + 10));
        body_.assign(data + header_size_, data + header_size_ + len);
        return len;
    }
}

void Message::Encode(std::vector<uint8_t>& buffer) {
    buffer.insert(buffer.end(), (uint8_t*)&header_.length_, (uint8_t*)&header_.length_ + 4);
    buffer.insert(buffer.end(), (uint8_t*)&header_.type_, (uint8_t*)&header_.type_ + 2);
    buffer.insert(buffer.end(), (uint8_t*)&header_.askid_, (uint8_t*)&header_.askid_ + 4);
    buffer.insert(buffer.end(), (uint8_t*)&header_.message_id_, (uint8_t*)&header_.message_id_ + 4);
    buffer.insert(buffer.end(), body_.begin(), body_.end());
}

Message::Ptr MessageFactory::BuildMessage() { return std::make_shared<Message>(); }