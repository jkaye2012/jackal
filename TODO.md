
- [ ] Lexer could operate on a lazily forced character sequence rather than
      forcing the entire input to be read eagerly. This has potential to
      improve lexer performance if a bottleneck is identifier here.
- [ ] Documentation should include lexicon for core concepts:
    * Environment: type bindings for values within a scope
    * Context: type bindings for type variables within an instantiation
    * Argument
    * Parameter
    * Scope
    * Instantiation
