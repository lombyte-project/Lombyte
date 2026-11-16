import js from '@eslint/js';
import globals from 'globals';

export default [
  {
    ignores: ['build/**', '**/node_modules/**'],
  },
  {
    files: ['browser/**/*.mjs'],
    languageOptions: {
      ecmaVersion: 'latest',
      sourceType: 'module',
      globals: globals.browser,
    },
    rules: js.configs.recommended.rules,
  },
  {
    files: ['browser/**/*.test.mjs', 'browser/spike/*.mjs'],
    languageOptions: {
      globals: globals.node,
    },
  },
];