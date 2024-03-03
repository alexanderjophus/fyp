mod g6_import;
mod naive_mis;

pub mod prelude {
    pub use crate::g6_import::{G6Importer, GraphImporter};
    pub use crate::naive_mis::{improved_naive_mis, naive_mis};
}
