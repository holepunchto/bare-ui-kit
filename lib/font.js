const binding = require('../binding')
const { expose } = require('./handle')
const wrap = require('./wrap')
const UIKitFontDescriptor = require('./font-descriptor')

module.exports = exports = class UIKitFont {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  static systemFont(size, weight = binding.FONT_WEIGHT_REGULAR) {
    return wrap(UIKitFont, binding.fontSystemFont(size, weight))
  }

  static boldSystemFont(size) {
    return wrap(UIKitFont, binding.fontBoldSystemFont(size))
  }

  static monospacedSystemFont(size, weight = binding.FONT_WEIGHT_REGULAR) {
    return wrap(UIKitFont, binding.fontMonospacedSystemFont(size, weight))
  }

  static withName(name, size) {
    return wrap(UIKitFont, binding.fontWithName(name, size))
  }

  static withDescriptor(descriptor, size) {
    return wrap(UIKitFont, binding.fontWithDescriptor(descriptor._tag, size))
  }

  get fontDescriptor() {
    return wrap(UIKitFontDescriptor, binding.fontFontDescriptor(this._tag))
  }

  get fontName() {
    return binding.fontFontName(this._tag)
  }

  get familyName() {
    return binding.fontFamilyName(this._tag)
  }

  get pointSize() {
    return binding.fontPointSize(this._tag)
  }

  get ascender() {
    return binding.fontAscender(this._tag)
  }

  get descender() {
    return binding.fontDescender(this._tag)
  }

  get capHeight() {
    return binding.fontCapHeight(this._tag)
  }

  get xHeight() {
    return binding.fontXHeight(this._tag)
  }

  get leading() {
    return binding.fontLeading(this._tag)
  }

  get lineHeight() {
    return binding.fontLineHeight(this._tag)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitFont }
    }
  }
}

exports.WEIGHT = {
  ULTRA_LIGHT: binding.FONT_WEIGHT_ULTRA_LIGHT,
  THIN: binding.FONT_WEIGHT_THIN,
  LIGHT: binding.FONT_WEIGHT_LIGHT,
  REGULAR: binding.FONT_WEIGHT_REGULAR,
  MEDIUM: binding.FONT_WEIGHT_MEDIUM,
  SEMIBOLD: binding.FONT_WEIGHT_SEMIBOLD,
  BOLD: binding.FONT_WEIGHT_BOLD,
  HEAVY: binding.FONT_WEIGHT_HEAVY,
  BLACK: binding.FONT_WEIGHT_BLACK
}

expose(exports)
