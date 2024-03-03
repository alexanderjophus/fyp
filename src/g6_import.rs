use nom::{
    bytes::complete::{take, take_while},
    IResult,
};
use petgraph::graph::{NodeIndex, UnGraph};

pub trait GraphImporter {
    fn import(&self, path: &str) -> Result<UnGraph<usize, usize>, String>;
}

pub struct G6Importer;

impl GraphImporter for G6Importer {
    fn import(&self, path: &str) -> Result<UnGraph<usize, usize>, String> {
        let file = std::fs::read_to_string(path).map_err(|e| e.to_string())?;

        decode(file.as_str())
    }
}

fn decode(
    contents: &str,
) -> Result<petgraph::prelude::Graph<usize, usize, petgraph::prelude::Undirected>, String> {
    let (remaining, n) = decode_n(contents).map_err(|e| e.to_string())?;
    let g = decode_r(n, remaining);

    Ok(g)
}

fn decode_n(input: &str) -> IResult<&str, usize> {
    let (remaining, depth) = take_while(|c: char| (c as u32) == 126)(input)?;
    let t: usize = match depth.len() {
        0 => 1,
        1 => 3,
        2 => 6,
        _ => 0,
    };
    if t == 0 {
        return Err(nom::Err::Error(nom::error::Error::new(
            input,
            nom::error::ErrorKind::TakeWhile1,
        )));
    }
    let (remaining, n) = take(t)(remaining)?;
    let v = n
        .chars()
        .fold(0, |acc, c| acc * 64 + (c as u32 - 63) as usize);
    Ok((remaining, v))
}

fn decode_r(nodes: usize, input: &str) -> UnGraph<usize, usize> {
    let mut nodes_vec = Vec::<NodeIndex>::with_capacity(nodes);
    let mut g = UnGraph::new_undirected();

    for i in 0..nodes {
        let ix = g.add_node(i);
        nodes_vec.push(ix);
    }

    let mut i = 0;
    let mut j = 1;

    for c in input.chars() {
        let mut mask = 0b100000;
        let b = c as u32 - 63;
        for _ in 0..6 {
            // if the last bit is 1, add an edge
            if b & mask != 0 {
                g.add_edge(nodes_vec[i], nodes_vec[j], 1);
            }
            // shift the bits to the right
            mask >>= 1;
            if i + 1 == j {
                i = 0;
                j += 1;
            } else {
                i += 1;
            }
        }
    }

    g
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_decode() {
        let input = "DQc";
        let result = decode(input);
        match result {
            Ok(g) => {
                assert_eq!(g.node_count(), 5);
                assert_eq!(g.edge_count(), 4);
            }
            Err(e) => {
                panic!("Error: {:?}", e);
            }
        }
    }

    #[test]
    fn test_decode_n() {
        let input = "GCQR@O";
        let result = decode_n(input);
        assert_eq!(result, Ok(("CQR@O", 8)));

        let input = "~B?xCQR@O";
        let result = decode_n(input);
        assert_eq!(result, Ok(("CQR@O", 12345)));

        let input = "~~?ZZZZZCQR@O";
        let result = decode_n(input);
        assert_eq!(result, Ok(("CQR@O", 460175067)));

        let input = "~~~?ZZZZZZZZZZZZZCQR@O";
        let result = decode_n(input);
        assert_eq!(
            result,
            Err(nom::Err::Error(nom::error::Error::new(
                "~~~?ZZZZZZZZZZZZZCQR@O",
                nom::error::ErrorKind::TakeWhile1
            )))
        );
    }

    #[test]
    fn test_decode_r() {
        let input = "Qc";
        let result = decode_r(5, input);
        assert_eq!(result.node_count(), 5);
        assert_eq!(result.edge_count(), 4);
    }
}
