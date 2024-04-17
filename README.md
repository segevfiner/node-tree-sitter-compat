# node-tree-sitter-compat
Compatibility module for old v8 API based language bindings for node-tree-sitter.

Grammars should be updated by running `tree-sitter generate` so they no longer require this. Also
note that the old grammar bindings were not context aware (Won't work in Electron and/or workers).

This is a no-op if called an a Node API based language bindings so you can always just call it on
all your languages to have support for both old and new ones.

## Usage
```ts
import { convertLanguage } from 'tree-sitter-compat';
import Parser from 'tree-sitter';
import JavaScript from 'tree-sitter-javascript';

const parser = new Parser();
parser.setLanguage(convertLanguage(JavaScript));
```
