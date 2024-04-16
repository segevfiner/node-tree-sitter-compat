import { expect, test } from 'vitest';
import { convertLanguage } from '.';
// @ts-ignore
import JavaScript from 'tree-sitter-javascript';
import C from 'tree-sitter-c';
import Parser from 'tree-sitter';

test('convert old language', () => {
  expect(JavaScript.language).toBeUndefined();
  expect(convertLanguage(JavaScript).language).toBeTypeOf("object");
})

test('convert node api language', () => {
  expect(C.language).toBeTypeOf("object");
  expect(convertLanguage(C).language).toBeTypeOf("object");
});

test('parse with converted language', () => {
    const parser = new Parser();
    expect(() => parser.setLanguage(JavaScript)).toThrow('Invalid language object');

    parser.setLanguage(convertLanguage(JavaScript));
    const tree = parser.parse('console.log("Hello, world!");')
})
