use std::fmt::{Debug, Display};

use petgraph::prelude::{Graph, Undirected};

pub fn naive_mis<N: Ord, E>(graph: Graph<N, E, Undirected>) -> Vec<N>
where
    N: Clone + Copy + Display + Debug,
    E: Clone,
{
    if graph.node_count() == 0 {
        return vec![];
    }

    let mut a = graph.clone();
    let mut b = graph.clone();

    // remove first node from a
    let na = a.node_indices().next().unwrap();
    a.remove_node(na);
    let res_a = naive_mis(a);

    // remove all adjacent nodes from b (and node itself)
    let nb = b.node_indices().next().unwrap();
    b.retain_nodes(|fgb, neighbor| {
        fgb.node_weight(nb).unwrap();
        !(fgb.contains_edge(nb, neighbor) || nb == neighbor)
    });
    let mut res_b = naive_mis(b);
    res_b.push(graph[nb]);

    if res_a.len() > res_b.len() {
        res_a
    } else {
        res_b
    }
}
