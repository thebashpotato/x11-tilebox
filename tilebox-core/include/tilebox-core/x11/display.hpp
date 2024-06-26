#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace tilebox::core
{

struct TILEBOX_INTERNAL DisplayDeleter
{
    auto operator()(Display *display) const noexcept -> void;
};

class X11Display;
using X11DisplaySharedResource = std::shared_ptr<X11Display>;

/// @brief Provides a RAII interface for common functionality needed from the X11 Display.
/// @details Manages the underlying Display * via a shared_ptr.
class TILEBOX_EXPORT X11Display
{
  public:
    ~X11Display() = default;
    X11Display(X11Display &&rhs) noexcept = default;
    X11Display(const X11Display &rsh) noexcept = default;

  public:
    auto operator=(X11Display &&rhs) noexcept -> X11Display & = default;
    auto operator=(const X11Display &rhs) noexcept -> X11Display & = default;

  public:
    /// @brief Static creation method for the shared display resource, this is to guarantee the resource is built
    /// correctly, and shared with all other objects that need a display connection (there are many) as a shallow
    /// pointer copy.
    ///
    /// @param `display_name` Specifies the hardware display name, which determines the display and communications
    /// domain to be used. On a POSIX-conformant system, if the display_name is NULL, it defaults to the value of the
    /// DISPLAY environment variable.
    ///
    /// @return std::nullopt if the display was unable to connect.
    [[nodiscard]] static auto create(const std::optional<std::string> &display_name = std::nullopt)
        -> std::optional<X11DisplaySharedResource>;

    /// @brief Check to see if the the underlying display is connected to the X server.
    ///
    /// @detail Should be used after creating the connection to check for failure.
    ///
    /// @return true if connected, false otherwise.
    [[nodiscard]] auto is_connected() const noexcept -> bool;

    /// @brief Gets the raw Display pointer
    [[nodiscard]] auto raw() const noexcept -> Display *;

    /// @brief Refreshes all internal display values.
    auto refresh() -> void;

    /// @brief Gets the active screen id
    [[nodiscard]] auto screen_id() const noexcept -> std::int32_t;

    /// @brief Gets the screen width
    [[nodiscard]] auto screen_width() const noexcept -> std::int32_t;

    /// @brief Gets the screen height
    [[nodiscard]] auto screen_height() const noexcept -> std::int32_t;

    /// @brief Gets the number of screens
    [[nodiscard]] auto screen_count() const noexcept -> std::int32_t;

    /// @brief Gets default root window
    [[nodiscard]] auto default_root_window() const noexcept -> Window;

    /// @brief Gets root window based on the current screen id
    [[nodiscard]] auto root_window() const noexcept -> Window;

    /// @brief Provides details about the X server implementation that is running
    [[nodiscard]] auto server_vendor() const noexcept -> std::string;

    /// @brief flushes the output buffer and then waits until all requests have been received and processed by the X
    /// server.
    ///
    /// @details Should be called before shutdown, calls `XSync`.
    ///
    /// @param `discard` Specifies whether the X server should discard all events in the event queue.
    auto sync(bool discard = false) const noexcept -> void;

  private:
    explicit X11Display(const std::optional<std::string> &display_name) noexcept;

  private:
    std::shared_ptr<Display> _dpy;
    std::int32_t _screen_id{};
    std::int32_t _screen_width{};
    std::int32_t _screen_height{};
    std::int32_t _screen_count{};
    Window _default_root_window{};
    Window _root_window{};
    std::string _server_vendor;
};

} // namespace tilebox::core
