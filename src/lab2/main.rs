extern crate algr;

use algr::FormulaWrapper;

fn main() {
    // Rust doesnt add NUL at the end of a string wtf??!?!
    let form = FormulaWrapper::new(1).unwrap().read("(!A) & (B)\0").unwrap();
    println!("{}", form);

    let used = form.getPerfectConjunctiveForm(false);
    println!("{}", used);
}