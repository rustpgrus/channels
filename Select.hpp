#pragma once

#include "Channel.hpp"

namespace core {

namespace utility {

template <typename Variant, typename Tuple, size_t... Indx>
auto tuple2variant(Variant &variant, Tuple tuple, std::index_sequence<Indx...>)
    -> void {
  (std::get<Indx>(tuple).template extract<Indx>(variant) || ...);
}

} // namespace utility

template <typename... Channels> auto Select(Channels... channels) {
  struct Awaiter {
    std::tuple<Channels...> m_channels;

    auto await_ready() noexcept -> bool {
      return std::apply(
          [](auto... args) { return (args.await_ready() || ...); }, m_channels);
    }

    auto await_suspend(core::CoroHandler h) -> void {
      auto pCoro = std::make_shared<core::CoroHandler>(h);
      std::apply([pCoro](auto... args) { (args.force_suspend(pCoro), ...); },
                 m_channels);
    }

    auto await_resume() -> std::variant<typename Channels::value_type...> {
      std::variant<typename Channels::value_type...> result;
      utility::tuple2variant(result, m_channels,
                             std::index_sequence_for<Channels...>{});
      return result;
    }
  };

  return Awaiter{std::make_tuple(std::move(channels)...)};
}

} // namespace core
