
# Assembler Project in C

## Project Overview
This project is a two-pass assembler developed in C that translates assembly language code into machine code for a simplified, hypothetical CPU. It was developed as part of the Systems Programming Lab course, where it received a grade of 100. The assembler interprets and compiles assembly commands into octal (base 8) instructions, producing executable output files.

For a detailed explanation of the project requirements and design, refer to the assembler project pdf file.

## Features
1. **Two-Pass Structure**: The assembler completes code analysis in two passes:
   - **First Pass**: Collects all labels and stores their addresses.
   - **Second Pass**: Converts instructions to machine code using the label addresses from the first pass.

2. **Directive Handling**: Supports `.data`, `.string`, `.entry`, and `.extern` directives for memory data definition, string storage, and external references.

3. **Macro Expansion**: A pre-assembler phase processes and expands macros before compiling the main code.

4. **Error Handling**: Detects syntax errors, undefined labels, and formatting issues, ensuring error-free output or detailed error messages.

5. **File Output**: Generates output files for each assembly program:
   - **.ob**: Machine code in octal format.
   - **.ext**: External symbols and memory locations.
   - **.ent**: Entry symbols for linking.

## Compilation and Execution
The project includes a `Makefile` to compile all source files with warning flags. Use the following command to compile:

```bash
make
```

This will compile the program and generate an executable named `assembler`.

### Running the Assembler

After compilation, run the assembler by providing the name of the assembly source file (without the `.as` extension):

```bash
./assembler program
```

For example, if you have an assembly file named `example.as`, running the command:

```bash
./assembler example
```

will generate the output files `example.ob`, `example.ext`, and `example.ent`, based on the contents of `example.as`.

## Project Structure

- **Source Code Files**:
  - `assembler.c`: Main driver function that orchestrates the assembler workflow.
  - `first_pass.c`: Handles the first pass of parsing, collecting labels and symbols.
  - `second_pass.c`: Manages the second pass, converting instructions to machine code.
  - `pre_proses.c`: Manages macro expansions during the pre-assembly phase.
  - `string_to_data.c`: Converts string representations into data formats for the assembler.
  - `errors.c`: Contains functions for error detection and reporting.
  - `prints.c`: Manages output formatting for the generated files.

- **Header Files**:
  - `assembler.h`, `first_pass.h`, `passes.h`: Define shared data structures and functions used across the assembler's modules.

## Testing and Debugging
The assembler was rigorously tested with various assembly programs to ensure correct output for both valid and error-prone input files. Custom test cases were created to verify each functionality, ensuring robust performance and reliable error handling.
