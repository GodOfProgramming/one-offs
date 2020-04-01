use super::*;

#[test]
fn args_new_fails_with_too_few_args() {
  let arg_vec = vec![String::from("program name"), String::from("search string")];
  assert_eq!(Args::new(&arg_vec), Err("need more arguments"));
}

#[test]
fn args_new_passes_with_right_number_of_args() {
  let arg_vec = vec![
    String::from("program name"),
    String::from("search string"),
    String::from("filename"),
  ];
  let query = String::from("search string");
  let filename = String::from("filename");
  let expected_args = Args {
    query: &query,
    filename: &filename,
  };

  assert_eq!(Args::new(&arg_vec), Ok(expected_args));
}

#[test]
fn args_new_passes_with_more_than_enough_args() {
  let arg_vec = vec![
    String::from("program name"),
    String::from("search string"),
    String::from("filename"),
    String::from("extra arg"),
  ];
  let query = String::from("search string");
  let filename = String::from("filename");
  let expected_args = Args {
    query: &query,
    filename: &filename,
  };

  assert_eq!(Args::new(&arg_vec), Ok(expected_args));
}

#[test]
fn run_returns_error_with_wrong_filename() {
  let query = String::from("search string");
  let filename = String::from("filename");
  let args = Args {
    query: &query,
    filename: &filename,
  };

  match run(&args) {
    Ok(_) => assert!(false),
    Err(_) => assert!(true),
  };
}

#[test]
fn run_returns_ok_with_valid_filename() {
  let query = String::from("search string");
  let filename = String::from(file!());
  let args = Args {
    query: &query,
    filename: &filename,
  };

  match run(&args) {
    Ok(_) => assert!(true),
    Err(_) => assert!(false),
  };
}

// from tutorial
#[test]
fn one_result() {
  let query = "duct";
  let contents = "\
Rust
safe, fast, productive.
Pick three.";

  assert_eq!(
    vec!["safe, fast, productive."],
    search(query, contents)
  );
}
