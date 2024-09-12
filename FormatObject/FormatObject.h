#pragma once

#include <concepts>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <span>
#include <string>
#include <sstream>
#include <variant>
#include <magic_get.h>

namespace Cango::TaskDesign {

    /// @brief Concept for objects that can be formatted using fmt.
    template <typename TObject>
    concept IsFormattableObject = requires(const TObject & obj, fmt::memory_buffer & buffer) {
        { obj.Format(buffer) } -> std::convertible_to<void>;
    };

    /// @brief Base class for formattable objects, providing a Format method that formats the object to a fmt buffer.
    struct FormattableObject {
        /// @brief Formats the object into the provided fmt buffer.
        /// @param buffer The fmt memory buffer for formatting.
        /// @note Override this method to customize the formatting of derived objects.
        virtual void Format(fmt::memory_buffer& buffer) const noexcept = 0;
    };

    // 补充备选方案FormattableVirant
    template<typename... Ts>
    struct FormattableVariant : std::variant<Ts...> {
        using std::variant<Ts...>::variant;

        void Format(fmt::memory_buffer& buffer) const noexcept {
            std::visit([&](const auto& val) {
                // 使用 if constexpr 分支处理不同的类型
                fmt::format_to(std::back_inserter(buffer), "{}", val);
                }, *this);
        }
    };

    /// @brief Formats a collection of formattable objects as a list.
    /// @param buffer The fmt memory buffer for formatting.
    /// @param objects A span of formattable objects to be formatted.
    template <IsFormattableObject TObject>
    void Format(fmt::memory_buffer& buffer, const std::span<const TObject> objects) noexcept {
        const auto size = objects.size();
        if (size == 0) {
            fmt::format_to(buffer, "[]");
            return;
        }

        fmt::format_to(buffer, "[");
        for (size_t i = 0; i < size; ++i) {
            objects[i].Format(buffer);
            if (i < size - 1) {
                fmt::format_to(buffer, ", ");
            }
        }
        fmt::format_to(buffer, "]");
    }

    /// @brief Outputs a collection of formattable objects as a formatted list to a stream.
    template <IsFormattableObject TObject>
    std::ostream& operator<<(std::ostream& stream, const std::span<const TObject> objects) noexcept {
        fmt::memory_buffer buffer;
        Format(buffer, objects);
        return stream << fmt::to_string(buffer);
    }

    //补充一个 FormattableVarriant

}