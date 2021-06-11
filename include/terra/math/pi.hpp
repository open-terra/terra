#pragma once

#include "../concepts.hpp"

namespace terra::math
{
    template<Real R>
    static constexpr R PI()
    {
        return static_cast<R>(3.141592653589793238462643383279502884);
    }
}
