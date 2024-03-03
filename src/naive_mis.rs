use petgraph::prelude::{Graph, Undirected};
use std::fmt::Debug;

pub fn naive_mis<N: Ord, E>(graph: &Graph<N, E, Undirected>) -> Vec<N>
where
    N: Clone + Copy,
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
    let res_a = naive_mis(&a);

    // remove all adjacent nodes from b (and node itself)
    let nb = b.node_indices().next().unwrap();
    b.retain_nodes(|fgb, neighbor| !(fgb.contains_edge(nb, neighbor) || nb == neighbor));
    let mut res_b = naive_mis(&b);
    res_b.push(graph[nb]);

    if res_a.len() > res_b.len() {
        res_a
    } else {
        res_b
    }
}

pub fn improved_naive_mis<N: Ord, E>(graph: &Graph<N, E, Undirected>) -> Vec<N>
where
    N: Clone + Copy + Debug,
    E: Clone,
{
    if graph.node_count() <= 1 {
        return graph.node_indices().map(|n| graph[n]).collect();
    }

    let nodes_with_small_degrees: Vec<petgraph::prelude::NodeIndex> = graph
        .node_indices()
        .filter(|n| graph.neighbors(*n).count() <= 1)
        .collect();

    if nodes_with_small_degrees.len() > 0 {
        let mut i = graph.clone();
        i.retain_nodes(|_, neighbor| !nodes_with_small_degrees.contains(&neighbor));

        if i.node_count() == 0 {
            return nodes_with_small_degrees
                .iter()
                .filter(|ni| {
                    graph.neighbors(**ni).count() == 0
                        || graph
                            .neighbors(**ni)
                            .into_iter()
                            .all(|nj| ni.index() > nj.index())
                })
                .map(|n| graph[*n])
                .collect();
        }
    }

    let mut a = graph.clone();
    let mut b = graph.clone();

    // remove first node from a
    let na = a.node_indices().next().unwrap();
    a.remove_node(na);
    let res_a = improved_naive_mis(&a);

    // remove all adjacent nodes from b (and node itself)
    let nb = b.node_indices().next().unwrap();
    b.retain_nodes(|fgb, neighbor| !(fgb.contains_edge(nb, neighbor) || nb == neighbor));
    let mut res_b = improved_naive_mis(&b);
    res_b.push(graph[nb]);

    if res_a.len() > res_b.len() {
        res_a
    } else {
        res_b
    }
}
