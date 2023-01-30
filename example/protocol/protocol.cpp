#include <iostream>
#include <vector>
#include <cstring>
#include <type_traits>
#include <assert.h>

using Bytes = std::vector<uint8_t>;

Bytes operator+(const Bytes& left, const Bytes& right) {
    Bytes ret;
    ret.resize(left.size()+right.size());
    memcpy(ret.data(), left.data(), left.size());
    memcpy(ret.data()+left.size(), right.data(), right.size());
    return ret;
}

struct Message {
    virtual Bytes MessageSerialise() = 0;
    virtual void MessageDeserialise(Bytes& buffer) = 0;
};

template<class T>
typename std::enable_if<std::is_arithmetic<T>::value, Bytes>::type Serialise(T* t) {
    auto begin = reinterpret_cast<uint8_t*>(t);
    return Bytes(begin, begin+sizeof(T));
}

Bytes Serialise(std::string* str) {
    int len = str->size();
    Bytes ret;
    ret.reserve(sizeof(int)+len);
    auto lb = reinterpret_cast<uint8_t*>(&len);
    ret.insert(ret.end(), lb, lb+sizeof(int));
    ret.insert(ret.end(), str->begin(), str->end());
    return ret;
}


// template<class T>
// Bytes Serialise(std::vector<typename std::enable_if<std::is_pod<T>::value, T>::type>* vec) {
//     int len = vec->size();
//     Bytes ret;
//     ret.reserve(sizeof(int)+len*sizeof(T));
//     auto lb = reinterpret_cast<uint8_t*>(&len);
//     ret.insert(ret.end(), lb, lb+sizeof(int));
//     auto vb = reinterpret_cast<uint8_t*>(vec->data());
//     ret.insert(ret.end(), vb, vb+len*sizeof(T));
//     return ret;
// }

// template<class T>
// Bytes Serialise(std::vector<typename std::enable_if<!std::is_pod<T>::value, T>::type>* vec) {
//     Bytes ret;
//     int len = vec->size();
//     auto lb = reinterpret_cast<uint8_t*>(&len);
//     ret.insert(ret.end(), lb, lb+sizeof(int));
//     for(auto& item : *vec) {
//         if(std::is_base_of<Message, T>::value) {
//             ret = ret + ((Message*)&item)->MessageSerialise();
//         } else {
//             ret = ret + Serialise(item);
//         }
//     }
//     return ret;
// }

Bytes Serialise() {
    return Bytes();
}

template<class T, class... Args>
Bytes Serialise(T* t, Args... args) {
    if(std::is_base_of<Message, T>::value) {
        return ((Message*)t)->MessageSerialise() + Serialise(args...);
    }
    return Serialise(t) + Serialise(args...);
}

void Deserialise(Bytes& buffer) {
    assert(buffer.size() == 0);
    (void)buffer;
}

template<class T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type Deserialise(Bytes& buffer, T* t) {
    *t = *(reinterpret_cast<T*>(buffer.data()));
    buffer.erase(buffer.begin(), buffer.begin()+sizeof(T)); 
}

void Deserialise(Bytes& buffer, std::string* str) {
    int len = *(reinterpret_cast<int*>(buffer.data()));
    str->assign(buffer.begin() + sizeof(int), buffer.begin() + sizeof(int) + len);
    buffer.erase(buffer.begin(), buffer.begin()+sizeof(int)+len);
}

template<class T>
void Deserialise(Bytes& buffer, std::vector<typename std::enable_if<std::is_pod<T>::value, T>::type>* vec) {
    int len = *(reinterpret_cast<int*>(buffer.data()));
    vec->assign(buffer.begin() + sizeof(int), buffer.begin() + sizeof(int) + len);
    buffer.erase(buffer.begin(), buffer.begin()+sizeof(int)+len);
}

template<class T>
void Deserialise(Bytes& buffer, std::vector<typename std::enable_if<!std::is_pod<T>::value, T>::type>* vec) {
    int len = *(reinterpret_cast<int*>(buffer.data()));
    vec->reserve(len);
    for(int i = 0; i < len; i++) {
        T t;
        if(std::is_base_of<Message, T>::value) {
            t.MessageDeserialise(buffer);
        } else {
            Deserialise(buffer, &t);
        }
        vec->push_back(t);
    }
}

template<class T, class... Args>
void Deserialise(Bytes& buffer, T* t, Args... args) {
    if (std::is_base_of<Message, T>::value) {
        ((Message*)t)->MessageDeserialise(buffer);
    } else {
        Deserialise(buffer, t);
    }
    Deserialise(buffer, args...);
}

struct UserInfo : public Message {
    uint32_t user_id;
    std::string user_name;
    UserInfo() = default;
    UserInfo(uint32_t u, std::string name): user_id(u), user_name(std::move(name)){}
    Bytes MessageSerialise() override {
        return Serialise(&user_id, &user_name);
    }
    void MessageDeserialise(Bytes& buffer) override {
        Deserialise(buffer, &user_id, &user_name);
    }
};

struct ReqLogin : public Message {
    uint32_t message_id;
    uint32_t room_id;
    int32_t tea_id;
    std::string room_name;
    bool ret;
    UserInfo user;
    Bytes MessageSerialise() override {
        return Serialise(&message_id, &room_id, &tea_id, &room_name, &ret, &user);
    }
    void MessageDeserialise(Bytes& buffer) override {
        Deserialise(buffer, &message_id, &room_id, &tea_id, &room_name, &ret, &user);
    }
};

std::ostream& operator<< (std::ostream& os, const ReqLogin& rq) {
    os << "messageid="<<rq.message_id<<" ,roomid="<<rq.room_id<<" ,teaid="<<rq.tea_id
    <<" ,room_name="<<rq.room_name<<" ,ret="<<rq.ret << " ,user id" << rq.user.user_id;
    return os;
}



int main() {
    std::cout << std::is_pod<UserInfo>::value << std::endl;
    std::cout << std::is_same<std::vector<UserInfo>, 
    std::vector<typename std::enable_if<!std::is_pod<UserInfo>::value, UserInfo>::type>>::value << std::endl;
    // std::cout << std::is_arithmetic<int32_t>::value << std::endl;
    ReqLogin msg;
    msg.message_id = 1;
    msg.room_id = 2;
    msg.tea_id = 3;
    msg.room_name = "nihao";
    msg.ret = true;
    msg.user.user_id = 10000;
    msg.user.user_name = "xiaobo";
    auto ret = msg.MessageSerialise();
    std::cout << "ffffffffffffffffffffffffffffffffffffffffffffffffff" << std::endl;
    ReqLogin rq;
    rq.MessageDeserialise(ret);
    std::cout << rq << std::endl;
    return 0;
}