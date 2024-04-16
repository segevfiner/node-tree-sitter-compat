# node-tree-sitter-compat
Compatibility module for old v8 API based language bindings for node-tree-sitter.

## Usage
```ts
import { convertLanguage } from 'tree-sitter-compat';
import Parser from 'tree-sitter';
import JavaScript from 'tree-sitter-javascript';

const parser = new Parser();
parser.setLanguage(convertLanguage(JavaScript));
```
