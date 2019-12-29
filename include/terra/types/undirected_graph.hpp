#pragma once

#include <cstdint>

namespace terra
{
    typedef boost::property<boost::vertex_index_t, size_t> vertex_property;
    typedef boost::property<boost::edge_index_t, std::size_t> edge_property;

    typedef boost::adjacency_list<boost::setS,
                                  boost::vecS,
                                  boost::undirectedS,
                                  vertex_property,
                                  edge_property
                                  >undirected_graph;

    typedef boost::graph_traits<undirected_graph> undirected_graph_traits;
}