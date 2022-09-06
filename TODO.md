
- [ ] Lexer could operate on a lazily forced character sequence rather than
      forcing the entire input to be read eagerly. This has potential to
      improve lexer performance if a bottleneck is identifier here.
