#pragma once

#include <coroutine>
#include <memory>
#include <variant>

#include "Common.hpp"
#include "Queue.hpp"

namespace core {

using container::Queue;

using CoroHandler = std::coroutine_handle<>;
using CoroHandlerPtr = std::weak_ptr<CoroHandler>;

struct sheduler_default {
  static void post(std::function<void()> f) {
    if (f)
      f();
  }
};

template <typename TypeValue, typename Sheduler = sheduler_default>
class Channel {
public:
  struct ChannelInput {
    using value_type = TypeValue;

    auto await_ready() noexcept -> bool {
      return m_pChannel->m_DataQueue.IsEmpty();
    }
    auto await_suspend(CoroHandler h) -> void {
      m_pChannel->m_InQueue.Put(std::move(h));
    }

    auto await_resume() {
      struct HelperInput {
        HelperInput(Channel *pChannel) : m_pChannel(pChannel) {}

        auto operator<<(TypeValue value) -> void {
          m_pChannel->m_DataQueue.Put(std::move(value));
          while (not m_pChannel->m_OutQueue.IsEmpty()) {
            std::visit(overloaded{[](CoroHandler handle) {
                                    Sheduler::post(
                                        [coro = std::move(handle)]() {
                                          coro.resume();
                                        });
                                  },
                                  [](CoroHandlerPtr ptrHandle) {
                                    auto shared_handle = ptrHandle.lock();
                                    auto coro = *shared_handle;
                                    shared_handle = nullptr;
                                    Sheduler::post([coro] { coro.resume(); });
                                  }},
                       m_pChannel->m_OutQueue.Take());
          }
        }

      private:
        Channel *m_pChannel;
      };
      return HelperInput{m_pChannel};
    }

    friend class Channel;

  private:
    ChannelInput(Channel *pChannel) : m_pChannel(pChannel) {}
    Channel *m_pChannel{nullptr};
  };

  struct ChannelOutput {
    using value_type = TypeValue;

    template <size_t Indx, typename Variant> auto extract(Variant &v) -> bool {
      if (await_ready()) {
        v = await_resume();
        return true;
      }
      return false;
    }

    auto force_suspend(std::weak_ptr<CoroHandler> pHandl) -> void {
      m_pChannel->m_OutQueue.Put(pHandl);
    }

    auto await_ready() noexcept -> bool {
      return not m_pChannel->m_DataQueue.IsEmpty();
    }
    auto await_suspend(CoroHandler h) -> void {
      m_pChannel->m_OutQueue.Put(std::move(h));
    }

    auto await_resume() -> TypeValue {
      auto data = std::move(m_pChannel->m_DataQueue.Take());
      if (not m_pChannel->m_InQueue.IsEmpty()) {
        Sheduler::post([coro = std::move(m_pChannel->m_InQueue.Take())] {
          coro.resume();
        });
      }
      return data;
    }

    friend class Channel;

  private:
    ChannelOutput(Channel *pChannel) : m_pChannel(pChannel) {}
    Channel *m_pChannel{nullptr};
  };

  operator ChannelInput() { return ChannelInput{this}; }
  operator ChannelOutput() { return ChannelOutput{this}; }

private:
  using CoroHandlerVariant = std::variant<CoroHandler, CoroHandlerPtr>;

  Queue<TypeValue> m_DataQueue;
  Queue<CoroHandler> m_InQueue;
  Queue<CoroHandlerVariant> m_OutQueue;
};

} // namespace core
