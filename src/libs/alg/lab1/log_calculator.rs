#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

pub struct FormulaWrapper {
    formula: Formula,
}

pub struct FormulaElement {
    data: [i8; LATIN_ALPHABET_LENGTH as usize],
}

pub struct FormulaInterpetation {
    data: [bool; LATIN_ALPHABET_LENGTH as usize],
}

impl FormulaWrapper {
    pub fn new(formulaType: u32) -> Option<FormulaWrapper> {
        unsafe {
            let formula = initFormula(formulaType as i32);
            if formula.type_ == INPUT_TYPE_INVALID {
                None
            } else {
                Some(FormulaWrapper { formula:formula })
            }
        }
    }

    pub fn read(mut self, formulaVal: &str) -> Option<Self> {
        unsafe {
            if !processFormula(&mut (self.formula), formulaVal.as_ptr()) {
                Some(self)
            } else {
                None
            }
        }
    }

    pub fn addElement(&mut self, newElement: FormulaElement) {
        unsafe {
            addElement(&mut self.formula, newElement.data.as_ptr())
        }
    }

    pub fn getValueAtInterpretation(self, interpretation: FormulaInterpetation) -> bool {
        unsafe {
            findVal(self.formula, interpretation.data.as_ptr())
        }
    }

    pub fn drop(&mut self) {
        unsafe {
            freeFormula(&mut self.formula);
        }
    }
}