
#include <iostream>
//#include <iosfwd>
//#include <memory>
//#include <boost/algorithm/string/classification.hpp>
//#include <boost/algorithm/string/split.hpp>
#include <boost/asio.hpp>
//#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>

class ThreadPool {
public:
    explicit ThreadPool(size_t size)
        : work_(io_service_) {
        for (size_t i = 0; i < size; ++i) {
            workers_.create_thread([this] {
                io_service_.run();
            });
        }
    }
    void stop() {
        io_service_.stop();
        if (!workers_.is_this_thread_in()) {
            workers_.join_all();
        }
    }

    ~ThreadPool() { stop(); }

    // Add new work item to the pool.
    template <class F>
    void enqueue(F f) {
        io_service_.post(f);
    }

private:
    boost::thread_group workers_;
    boost::asio::io_service io_service_;
    // m_work ensures that io_service's run() function will not exit while work is
    // underway
    boost::asio::io_service::work work_;
};

int main() {
    ThreadPool pool(3);
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> vec;
    for(int i = 0; i < 3; i++) {
        vec.push_back(std::thread([&pool, start]() {
            //std::this_thread::sleep_for(std::chrono::milliseconds(50));
            auto id = std::this_thread::get_id();
            for (int i = 0; i < 10000; i++) {
                pool.enqueue([id, i, start](){
                    auto dur = (std::chrono::high_resolution_clock::now() - start);
                    std::cout << "from thread id = " << id << " ,i=" << i << "  ,线程id = " <<
                    std::this_thread::get_id() << " , time dur = " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << std::endl;
                });
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }));
    }
    
    for (auto& item : vec) {
        item.join();
    }
    
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
