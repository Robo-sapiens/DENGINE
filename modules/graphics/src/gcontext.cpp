//
// Created by kira on 28.02.2021.
//

#include "graphics/gcontext.hpp"

namespace graphics {
auto& GContext::instance() {
    static GContext context{};
    return context;
}
}  // namespace graphics
