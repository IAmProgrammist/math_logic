extern crate algr;

use std::io;

use algr::{FormulaWrapper, INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM, INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM};

fn main() {
    println!("Введите формулу: ");
    let form = loop {

        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("Ошибка во время чтения формулы");
        input.push('\0');

        let form_read = FormulaWrapper::new(INPUT_TYPE_DISJUNCTIVE_NORMAL_FORM)
            .unwrap_or_else(|| panic!("Ошибка во время инициализации формулы"))
            .read(input.clone());

        if !form_read.is_none() {
            break form_read.unwrap();
        } else {
            let form_read = FormulaWrapper::new(INPUT_TYPE_CONJUNCTIVE_NORMAL_FORM)
            .unwrap_or_else(|| panic!("Ошибка во время инициализации формулы"))
            .read(input);

            if !form_read.is_none() {
                break form_read.unwrap();
            } else {
                println!("Не удалось распознать формулу. Попробуйте ещё раз:");
            }
        }
    };
    
    let used = form.getStatements(false);
    
    println!("===================================");
    println!("Формулы, для которых формула {} - следствие:", form);
    for fx in used {
        println!("{}", fx);
    }
}