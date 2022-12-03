#pragma once

class noncopyable {
protected:
    noncopyable() = default;
    ~noncopyable() = default;
private:
    noncopyable(const noncopyable& es) = delete;
    noncopyable& operator=(const noncopyable& es) = delete;
};