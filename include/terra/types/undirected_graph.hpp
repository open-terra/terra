#pragma once

#include "../utils/template_helpers.hpp"
#include "detail/graph_t.hpp"

namespace terra
{
    typedef detail::graph_t<graph_type::undirected> undirected_graph;
} // namespace terra
