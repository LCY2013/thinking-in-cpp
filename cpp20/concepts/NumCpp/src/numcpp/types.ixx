//
// Created by fufeng on 2024/2/21.
//
export module numcpp:types;

#include <vector>
#include <cstdint>
#include <initializer_list>
#include <tuple>
#include <optional>

namespace numcpp {
   using std::size_t;

   export using Shape = std::vector<size_t>;

   export class SliceItem {
   public:
       SliceItem(): _isPlaceholder(true) {}

       explicit SliceItem(int32_t value): _value(value) {}

       [[nodiscard]] int32_t getValue() const {
           return _value;
       }

       [[nodiscard]] bool isPlaceholder() const {
           return _isPlaceholder;
       }

       [[nodiscard]] std::tuple<int32_t, bool> getValidValue(size_t maxSize, bool isStart) const {
            auto signedMaxSize = static_cast<int32_t>(maxSize);

            if (_isPlaceholder) {
                return std::make_tuple(isStart ? 0: maxSize, true);
            }

            if (_value > signedMaxSize) {
                return std::make_tuple(signedMaxSize, true);
            }

            if (_value < 0) {
                int32_t actualValue = signedMaxSize + _value;

                return std::make_tuple(actualValue, actualValue >= 0);
            }

           return std::make_tuple(_value, true);
       }

       [[nodiscard]] std::optional<int32_t> optionalValidValue(size_t maxSize, bool isStart) const {
           auto signedMaxSize = static_cast<int32_t>(maxSize);
           if (_isPlaceholder) {
               return isStart ? std::optional<int32_t>(0): std::optional<int32_t>(maxSize);
           }

           if (_value > signedMaxSize) {
               return signedMaxSize;
           }

           if (_value < 0) {
               int32_t actualValue = signedMaxSize + _value;
              return actualValue >= 0 ? std::optional<int32_t>(actualValue): std::nullopt;
           }

           return _value;
       }

   private:
       int32_t _value = 0;
       bool _isPlaceholder = false;
   };

   export const SliceItem SLICE_PLACEHOLDER;
}
