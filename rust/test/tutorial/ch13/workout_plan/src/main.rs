use std::thread;
use std::time::Duration;

pub mod util;
use util::Catcher;

fn generate_workout(intensity: u32, random_number: u32) {
  let mut expensive_result = Catcher::new(|num| {
    println!("calculating slowly");
    thread::sleep(Duration::from_secs(2));
    num
  });

  if intensity < 25 {
    println!("Today, do {} pushups", expensive_result.value(intensity));
    println!("Next do {} situps", expensive_result.value(intensity));
  } else {
    if random_number == 3 {
      println!("Take a break today");
    } else {
      println!("Today run for {} minutes", expensive_result.value(intensity));
    }
  }
}

fn main() {
  let simulated_user_specified_value = 10;
  let simulated_random_number = 7;

  generate_workout(simulated_user_specified_value, simulated_random_number);
}
