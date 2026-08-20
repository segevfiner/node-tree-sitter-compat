const pkgPrebuilds = require('pkg-prebuilds');
const binding = pkgPrebuilds(__dirname, require('./binding-options'));
exports.convertLanguage = binding.convertLanguage;
