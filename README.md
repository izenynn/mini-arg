# mini-args

## Info

A simple library for argument parsing in C, inspired by `argp`.

## Features

- Short and long argument.
- Concatenated short arguments (`-abcDEF`).
- Required argument (throws an error if a required argument is not provided).
- Required argument value (syntax: `-a VALUEk` / `-aVALUE` / `--arg VALUE`).
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
- Customize failure exit code.

## Usage

Define the following global vars:
```c
TODO...
```

TODO...
