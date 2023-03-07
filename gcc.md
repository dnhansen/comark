# Notes on `gcc`

The `gcc` command invokes the C and C++ compiler from the GNU project. This can be used to perform any of the steps in the translation process, from preprocessing to linking. Below we list each **step** of the translation process, including

- the **product** produced by the step in question,
- the usual **extension** of files produced by the step (though note that file extensions usually do not matter in Linux),
- the **flag** passed to `gcc` to halt the translation process after the given step, and
- the **default name** given to the output file.

Note that any file produced at any point during this process may be given as argument to `gcc` for further translation. In particular, we may perform the translation one step at a time by passing each flag below to `gcc` successively. To link multiple object files, pass them all as arguments to `gcc`.

Note also that the executable produced after linking does not have a file extension (compare `.exe` files on Windows). Indeed, as mentioned above file extensions barely matter on Linux. Instead, as we saw in week TODO, executables on Linux always start with the bytes TODO, so other programs can learn that the given file is an executable by examining the first few bytes. Other types of files are also identified by their leading bytes.

| Step | Product | Extension | Flag | Default name |
|------|---------|-----------|------|--------------|
| *None* | Source code with directives | `.c` | N/A | `foo.c` |
| Preprocessing | Source code | `.i` | `-E` | \* |
| Compilation | Assembly | `.s` | `-S` | `foo.s` |
| Assembly | Relocatable object code | `.o` | `-c` | `foo.o` |
| Linking | Executable binary code | *None* | *None* | `a.out` |

\*Running `gcc -E` outputs to standard output by default, but can be redirected to a named file with the `-o` flag. Thus the `.i` extension is not used by default, but it is the standard extension for source code files that do not need to be preprocessed.
