#pragma once

#include <queue>

namespace core {

namespace container {

template <typename ValueType> class Queue {
public:
  using value_type = ValueType;
  Queue() noexcept = default;
  ~Queue() noexcept = default;

  Queue(const Queue &) = delete;
  auto operator=(const Queue &) -> Queue & = delete;

  auto Put(value_type value) -> void { m_queue.emplace(std::move(value)); }

  auto Take() -> value_type {
    auto value = std::move(m_queue.front());
    m_queue.pop();
    return value;
  }

  auto IsEmpty() const { return m_queue.empty(); }

private:
  std::queue<value_type> m_queue;
};

} // namespace container

} // namespace core
