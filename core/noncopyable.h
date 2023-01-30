#pragma once

namespace guodong {
class noncopyable {
protected:
    noncopyable() = default;
    ~noncopyable() = default;
public:
    noncopyable(const noncopyable& es) = delete;
    noncopyable& operator=(const noncopyable& es) = delete;
};
}