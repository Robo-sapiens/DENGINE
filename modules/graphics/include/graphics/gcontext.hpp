//
// Created by kira on 28.02.2021.
//

#pragma once

namespace graphics {
class GContext {
public:
    inline auto &instance();
private:
    GContext() = default;

};
}  // namespace graphics
