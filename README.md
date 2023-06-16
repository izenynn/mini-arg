# mini-args

## Info

A simple library for argument parsing in C, inspired by `argp`.

## Features

- Short and long argument.
- Concatenated short arguments (`-abcDEF`).
- Required argument (throws an error if a required argument is not provided).
- Required argument value (syntax: `-a VALUE` / `-aVALUE` / `--arg VALUE` / `--arg=VALUE`).
- Optional argument value (value can be present or not).
- Default value, useful for optional argument value (`miniarg_option->arg`).
- If short option requires argument, rest of the string will be treated as one.
- Automatic `-h` and `--help`.
- Automatic `-V` and `--version`.
- Exposed function to print usage.
- Normal arguments.
- Count of normal arguments.
- Notify when arguments end so you can perform all necessary action.
- Supports `--`.
- Access to glabal variable `marg_err_exit_status` to customize the error exit code.
- Access to global varibale `marg_program_name` to help you with your own log functions.

## Usage

Define the following global vars:
```c
TODO...
```

TODO...
