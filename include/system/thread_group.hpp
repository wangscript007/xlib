// reference: asio/detail/thread_group.hpp
#ifndef XLIB_THREAD_GROUP_HPP
#define XLIB_THREAD_GROUP_HPP

#include <thread>

namespace xlib {

class thread_group {
 public:
  // Constructor initialises an empty thread group.
  thread_group() : first_(nullptr) {}

  // Destructor joins any remaining threads in the group.
  ~thread_group() { join(); }

  // Create a new thread in the group.
  template <typename Function>
  void create_thread(Function f) {
    first_ = new item(f, first_);
  }

  // Create new threads in the group.
  template <typename Function>
  void create_threads(Function f, std::size_t num_threads) {
    for (std::size_t i = 0; i < num_threads; ++i) {
      create_thread(f);
    }
  }

  // Wait for all threads in the group to exit.
  void join() {
    while (first_) {
      first_->thread_.join();
      item* tmp = first_;
      first_ = first_->next_;
      delete tmp;
    }
  }

 private:
  // Structure used to track a single thread in the group.
  struct item {
    template <typename Function>
    explicit item(Function f, item* next) : thread_(f), next_(next) {}

    std::thread thread_;
    item* next_;
  };

  // The first thread in the group.
  item* first_;
};

}  // namespace xlib

#endif  // XLIB_THREAD_GROUP_HPP
