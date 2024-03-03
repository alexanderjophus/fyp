use fyp::prelude::{improved_naive_mis, naive_mis, G6Importer, GraphImporter};

fn main() {
    let importer = G6Importer;
    let graph = importer.import("examples/DQc.g6").unwrap();
    let mis = naive_mis(&graph);
    assert_eq!(mis.len(), 3);
    let imis = improved_naive_mis(&graph);
    assert_eq!(imis.len(), 3);

    let graph = importer.import("examples/Ch.g6").unwrap();
    let mis = naive_mis(&graph);
    assert_eq!(mis.len(), 2);
    let imis = improved_naive_mis(&graph);
    assert_eq!(imis.len(), 2);

    let graph = importer.import("examples/GhCKN{.g6").unwrap();
    let mis = naive_mis(&graph);
    assert_eq!(mis.len(), 3);
    let imis = improved_naive_mis(&graph);
    assert_eq!(imis.len(), 3);

    let graph = importer.import("examples/G}hPW{.g6").unwrap();
    let mis = naive_mis(&graph);
    assert_eq!(mis.len(), 2);
    let imis = improved_naive_mis(&graph);
    assert_eq!(imis.len(), 2);

    // let graph = importer.import("examples/big.g6").unwrap();
    // let mis = naive_mis(&graph);
    // assert_eq!(mis.len(), 16);
    // let imis = improved_naive_mis(&graph);
    // assert_eq!(imis.len(), 16);
}
