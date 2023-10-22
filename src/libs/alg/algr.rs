#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

pub struct FormulaWrapper {
    pub formula: Formula,
}

pub struct FormulaElement {
    pub data: [i8; LATIN_ALPHABET_LENGTH as usize],
}

pub struct FormulaInterpetation {
    pub data: [bool; LATIN_ALPHABET_LENGTH as usize],
}

pub mod lab1;
pub mod lab2;