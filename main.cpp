#include <coro/coro.hpp>
#include <iostream>

#include "Common.hpp"
#include "Select.hpp"

template <typename T> using Chan = core::Channel<T>;

auto producer_str(Chan<std::string>::ChannelInput in) -> coro::task<void> {
  co_await in << "Hello";
  co_await in << ", ";
  co_await in << "World!";
}

auto producer_int(Chan<int>::ChannelInput in) -> coro::task<void> {
  co_await in << 1;
  co_await in << 2;
  co_await in << 3;
  co_await in << 4;
}


auto consumer(Chan<std::string>::ChannelOutput out_s,
              Chan<int>::ChannelOutput out_i) -> coro::task<void> {

  for ([[maybe_unused]] auto idx : std::views::iota(0, 7)) {
    auto result = co_await core::Select(out_i, out_s);
    std::visit(
        overloaded{
            [](int value) { std::cerr << "read int: " << value << std::endl; },
            [](std::string value) {
              std::cerr << "read string: " << value << std::endl;
            },
        },
        result);
  }
}

int main() {
  Chan<std::string> s_chan;
  Chan<int> int_chan;

  coro::sync_wait(coro::when_all(producer_str(s_chan), producer_int(int_chan),
                                 consumer(s_chan, int_chan)));

  return EXIT_SUCCESS;
}
