#pragma once

#include <cstdint>

#include <boost/graph/adjacency_list.hpp>

namespace terra
{
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, size_t, size_t> undirected_graph;
}