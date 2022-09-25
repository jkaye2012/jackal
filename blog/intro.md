# Jackal's first stripe: basic compilation infrastructure

Jackal is an experimental programming language exploring a fusion of functional and imperative programming paradigms.
This inaugural post describes the high-level goals for Jackal as a language, the development methodology I'll be using to push
updates to the language, and what exists in the language thus far (not very much just yet).

## Jackal

Jackal's primary goal is to allow programmers to express themselves in uniquely powerful ways that are fully enforced by a flexible and
consistent type system. While we may not be able to achieve performance commensurate with C, we aim to accomplish these goals without sacrificing performance
whenever possible.

Lying somewhere between the functional and imperative paradigms, Jackal aims to take the best of both worlds. We desire the directness of imperative implementations
with the maintainability and correctness of more functional styles. We want to be able to express as much of our domain logic as possible within the type system to
improve performance and correctness. We want to be able to write code free from boilerplate.

### Goals

With the high-level statement of Jackal's purpose in mind, we can explain the most important high-level goals in more detail. This is a long and aspirational list:

* Fast: Jackal shouldn't prevent programmers from writing truly performant code (including hard real-time use cases)
    * Therefore, Jackal will not be garbage collected
* First-class interoperability: we want to be able to use the vast ecosystem of existing code available to the world; therefore,
  Jackal will support native interoperability with the C ABI and calling convention
* Memory and data-race safe
    * Memory management via single ownership with generational projection
    * Possibly including structured concurrency
* Dependent types via refinement
    * Lack of type dependency is one of the primary reasons for complex run-time logic that cannot be verified by the compiler.
      By using a simple refinement-centric dependent type implementation, Jackal aims to provide most of the benefit of dependent
      types without requiring the rigor of mathematical proofs
* Explicit effects and composable error handling
    * Side-effects are part of most useful programs; however, they're also the cause of most of a programmer's problems. By explicitly
      modeling effects and errors in a simple and consistent way, Jackal allows programmers to understand how their programs can fail
      and where interactions with the external world must be managed
* Rich meta-programming: meta-programming allows libraries to effectively modify language syntax and prevents boilerplate
    * First-class reflection and introspection
    * Enabled by a stable language protocol
* Fully integrated tooling: if the language isn't pleasant to use and work with, none of these goals matters
    * Integrated debugger
    * Seamless LSP integration
    * Tree-sitter compatibility
    * Build, test, documentation, and executable management
    * Package and dependency management

## Striped development

When developing a programming language, there are many decisions to make and problems to solve. It's a large and complex project that will take significant
time and effort to complete, even if the goal isn't to eventually become an industry standard.

The striped development methodology really just means that I'll be implementing Jackal in functional phases that cut across all concerns of the compiler
rather than attempting to fully specify every aspect of the language before starting on development. While this can cause the overall time and effort required
to implement the project to increase slightly, it also makes the process much more fun and iterative by allowing me to deploy working (yet incomplete) versions
of the language.

See [my blog post on striped development](https://jordankaye.dev/posts/striped-development/) for a more in-depth discussion of this methodology.

Specifically for Jackal, whenever a stripe is completed, it will be tagged and accompanied by a blog post explaining the initial goals for the stripe and
what was ultimately accomplished.

### First stripe

The first stripe is extremely simple: very basic versions of each layer were implemented and integrated with each other. The grammar and semantics of the language are not worth
talking about as they have nothing to do with the actual Jackal language. The important thing to know is that the following concepts are now implemented, tested, and documented:

* A straightforward context-free lexer
* An LL(k) parser (supporting arbitrary k)
* A simple abstract syntax tree library
* Code generation to human-readable C code
* A basic human-centric error messaging system
* A bare-bones command line that compiles a single source file to a native executable
* Composable exception-free error handling (within the compiler itself)

While nothing implemented here is innovative or particularly exciting, it does provide a solid foundation to begin the striped development process. Notably, semantic analysis
and type-checking have not yet been implemented - this omission was intentionally left to the second stripe where it can be more reasonably designed with "real" Jackal code in
mind. In practice, with the current toy implementation, semantic analysis is "built-in" via the translation to C. While we are not in control of the error messages, they are
surprisingly useful in understanding the semantic mistake made in a source file.

## Next steps

The next stripe is to release the informal specification of the first version of Jackal's syntax and semantics along with a basic implementation using the first stripe's
base infrastructure. The second stripe will be very simple. While it's unlikely that even a single high-level goal will be accomplished, it should get the language
to a place where it's clear *how* some of those goals may eventually be implemented.
