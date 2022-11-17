#pragma once
//#include "Common.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iosfwd>
#include <memory>

#include "log.h"

namespace guodong {

class ThreadPool {
 public:
  typedef std::shared_ptr<ThreadPool> Ptr;

  explicit ThreadPool(const std::string& threadName, size_t size)
      : work_(io_service_) {
    thread_name_ = threadName;
    std::shared_ptr<std::vector<std::string>> fields =
        std::make_shared<std::vector<std::string>>();
    if (boost::log::core::get()) {
      boost::split(*fields, thread_name_, boost::is_any_of("-"),
                   boost::token_compress_on);
    }

    for (size_t i = 0; i < size; ++i) {
      workers_.create_thread([this, fields] {
        if (fields->size() > 1) {
          boost::log::core::get()->add_thread_attribute(
              "GroupId",
              boost::log::attributes::constant<std::string>((*fields)[1]));
        }
        // dev::pthread_setThreadName(_threadName);
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
  std::string thread_name_;
  boost::thread_group workers_;
  boost::asio::io_service io_service_;
  // m_work ensures that io_service's run() function will not exit while work is
  // underway
  boost::asio::io_service::work work_;
};

}  // namespace guodong
