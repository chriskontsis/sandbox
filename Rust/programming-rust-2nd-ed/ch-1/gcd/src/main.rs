// fn main() {
//     println!("Hello, world!");
// }

fn gcd(mut n: u64, mut m: u64) -> u64 {
    assert!(n != 0 && m != 0);
    while m != 0 {
        if m < n {
            let t = m;
            m = n;
            n = t;
        }
        m = m % n;
    }
    n
}

#[test]
fn test_gcd() {
    assert_eq!(gcd(14, 15), 1);
    assert_eq!(gcd(2 * 3 * 5 * 11 * 17, 3 * 7 * 11 * 13 * 19), 3 * 11);
}

use std::env;
use std::str::FromStr;

fn main() {
    let mut numbers = Vec::new();

    // args() returns iterator to stdin, skip(1) b/c first val ret from args() is prog name
    for arg in env::args().skip(1) {
        // any op that may fail return a Result type, calling expect on Result gets the value if it was a success
        numbers.push(u64::from_str(&arg).expect("error parsing arg"));
    }

    if numbers.len() == 0 {
        eprintln!("Usage: gcd NUMBER ...");
        std::process::exit(1);
    }

    let mut d = numbers[0];
    // &x is a reference, to each val, tells rust we are borrowing
    // *m defefs that reference to get underlying val
    for m in &numbers[1..] {
        d = gcd(d, *m);
    }

    println!("The gcd of {:?} is {}", numbers, d);
}
