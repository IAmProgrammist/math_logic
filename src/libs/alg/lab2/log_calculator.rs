use crate::INPUT_TYPE_INVALID;

use super::super::{FormulaElement, FormulaWrapper, FormulaInterpetation, LATIN_ALPHABET_LENGTH, LITERAL_UNDEF, LITERAL_POSITIVE, LITERAL_NEGATIVE, INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM, INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM};

use itertools::Itertools;

impl FormulaWrapper {
    pub fn getUsedVariables(&self) -> [bool; LATIN_ALPHABET_LENGTH as usize] {
        let mut result = [false; LATIN_ALPHABET_LENGTH as usize];
        for i in 0..self.formula.amount {
            let formula_element = unsafe{ std::slice::from_raw_parts(*(self.formula.val.add(i as usize)), LATIN_ALPHABET_LENGTH as usize)};

            for j in 0..LATIN_ALPHABET_LENGTH as usize {
                if formula_element[j] != LITERAL_UNDEF as i8 {
                    result[j] = true;
                }
            }
        };

        result
    }

    fn getPerfectConjunctiveForm_(&self, used_variables: [bool; LATIN_ALPHABET_LENGTH as usize], 
        current_index: usize, 
        current_element: [i8; LATIN_ALPHABET_LENGTH as usize]) -> Vec<FormulaElement> {
            let mut result_vector: Vec<FormulaElement> = Vec::new();
            let mut current_element = current_element;
            if current_index == LATIN_ALPHABET_LENGTH as usize {
                let mut interpretation = FormulaInterpetation{data: [false; LATIN_ALPHABET_LENGTH as usize]};

                for i in 0..LATIN_ALPHABET_LENGTH as usize {
                    if current_element[i] == LITERAL_NEGATIVE as i8 {
                        interpretation.data[i] = true;
                    }
                }
                
                if !self.getValueAtInterpretation(interpretation) {
                    result_vector.push(FormulaElement {data: current_element});
                }

            } else if used_variables[current_index]{
                current_element[current_index] = LITERAL_POSITIVE as i8;
                let mut ar1: Vec<FormulaElement> = self.getPerfectConjunctiveForm_(used_variables, current_index + 1, current_element);
                result_vector.append(&mut ar1);

                current_element[current_index] = LITERAL_NEGATIVE as i8;
                let mut ar2 = self.getPerfectConjunctiveForm_(used_variables, current_index + 1, current_element);
                result_vector.append(&mut ar2);
            } else {
                let mut ar1: Vec<FormulaElement> = self.getPerfectConjunctiveForm_(used_variables, current_index + 1, current_element);
                result_vector.append(&mut ar1);
            }

            result_vector
    }

    pub fn getPerfectConjunctiveForm(&self, include_unused_variables: bool) -> FormulaWrapper {
        let mut perfect_conjunctive_form = FormulaWrapper::new(INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM).unwrap();
        let used_variables = if include_unused_variables {[true; LATIN_ALPHABET_LENGTH as usize]} else {self.getUsedVariables()};

        let elements = self.getPerfectConjunctiveForm_(used_variables, 0, [LITERAL_UNDEF as i8; LATIN_ALPHABET_LENGTH as usize]);
        for element in elements {
            perfect_conjunctive_form.addElement(element);
        }

        perfect_conjunctive_form
    }

    fn getAllAdditionalElements( 
        present_elements: Vec<FormulaElement>,
        current_element: FormulaElement
    ) -> Vec<FormulaElement> {
        let mut result_elements: Vec<FormulaElement> = Vec::new();
        result_elements.push(current_element);


        result_elements
    }

    pub fn getStatements(&self, include_unused_variables: bool) -> Vec<FormulaWrapper> {
        let statements: Vec<FormulaWrapper> = Vec::new();
        let used_variables = if include_unused_variables {[true; LATIN_ALPHABET_LENGTH as usize]} else {self.getUsedVariables()};
        let perfect_conjunctive_formula = self.getPerfectConjunctiveForm(include_unused_variables);
        let mut additional_elements: Vec<FormulaElement> = Vec::new();

        {
            let mut current_element = FormulaElement {data: [LITERAL_UNDEF as i8; LATIN_ALPHABET_LENGTH as usize ]};
            for i in 0..LATIN_ALPHABET_LENGTH as usize {
                if used_variables[i] {
                    current_element.data[i] = LITERAL_NEGATIVE as i8;
                }
            }    

            let mut present_elements: Vec<FormulaElement> = Vec::new();

            // TODO: FIX THIS SHIT TOMORROW
            for element in unsafe {std::slice::from_raw_parts(*perfect_conjunctive_formula.formula.val, perfect_conjunctive_formula.formula.amount as usize)} {
                present_elements.push(value)
            }

            additional_elements = 
            FormulaWrapper::getAllAdditionalElements(, 
            current_element);
        }

        statements
    }
}

impl FormulaElement {
    pub fn equals(self, b: FormulaElement) -> bool {
        for i in 0..LATIN_ALPHABET_LENGTH as usize {
            if (self.data[i] != b.data[i]) {
                return false;
            }
        }

        true
    }
}

impl std::fmt::Display for FormulaWrapper {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        let mut buffer: String = "".to_owned();

        for i in 0..self.formula.amount {
            let element = unsafe {*self.formula.val.add(i as usize)};
            
            let mut anyWrite = false;

            if self.formula.type_ == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM as i32 {
                buffer += "(";
            }

            for j in 0..LATIN_ALPHABET_LENGTH as usize {
                let currElement = unsafe {*element.offset(j as isize)};
                if currElement == LITERAL_POSITIVE as i8 {
                    if self.formula.type_ == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!("{} + ", (j as u8 + 'A' as u8) as char);
                        anyWrite = true;
                    } else if self.formula.type_ == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!("{} & ", (j as u8 + 'A' as u8) as char);
                        anyWrite = true;
                    }
                } else if currElement == LITERAL_NEGATIVE as i8 {
                    if self.formula.type_ == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!("!{} + ", (j as u8 + 'A' as u8) as char);
                        anyWrite = true;
                    } else if self.formula.type_ == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!("!{} & ", (j as u8 + 'A' as u8) as char);
                        anyWrite = true;
                    }
                }
            }

            if anyWrite {
                buffer = buffer[..buffer.len() - 3].to_string();
                
                if i != self.formula.amount - 1 {
                    if self.formula.type_ == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!(") & ");
                    } else if self.formula.type_ == INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM as i32 {
                        buffer += &format!(" + ");
                    }
                } else if self.formula.type_ == INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM as i32 {
                    buffer += &format!(")");
                }
            }
        }
        
        write!(f, "{}", buffer)
    }
}