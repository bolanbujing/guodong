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
        if (length < header_size_ + len) {
            return 0;
        }
        LOG(INFO) << LOG_KV("message content length = ", length);
        header_.length = len;
        header_.type = *((int16_t*)(data + 4));
        header_.askid = *((int32_t*)(data + 6));
        header_.message_id = *((int32_t*)(data + 10));
        body_.assign(data + header_size_, data + header_size_ + len);
        return len;
    }
}

void Message::Encode(std::vector<uint8_t>& buffer) {
    buffer.insert(buffer.end(), (uint8_t*)&header_.length, (uint8_t*)&header_.length + 4);
    buffer.insert(buffer.end(), (uint8_t*)&header_.type, (uint8_t*)&header_.type + 2);
    buffer.insert(buffer.end(), (uint8_t*)&header_.askid, (uint8_t*)&header_.askid + 4);
    buffer.insert(buffer.end(), (uint8_t*)&header_.message_id, (uint8_t*)&header_.message_id + 4);
    buffer.insert(buffer.end(), body_.begin(), body_.end());
}

Message::Ptr MessageFactory::BuildMessage() { return std::make_shared<Message>(); }