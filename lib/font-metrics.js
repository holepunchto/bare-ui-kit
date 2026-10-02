const binding = require('../binding')
const registry = require('bare-foundation-registry')
const wrap = require('./wrap')
const UIKitFont = require('./font')

module.exports = exports = class UIKitFontMetrics {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  static get defaultMetrics() {
    return wrap(UIKitFontMetrics, binding.fontMetricsDefaultMetrics())
  }

  static metricsForTextStyle(textStyle) {
    return wrap(UIKitFontMetrics, binding.fontMetricsForTextStyle(textStyle))
  }

  scaledValueForValue(value) {
    return binding.fontMetricsScaledValueForValue(this._tag, value)
  }

  scaledFontForFont(font) {
    return wrap(UIKitFont, binding.fontMetricsScaledFontForFont(this._tag, font._tag))
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitFontMetrics }
    }
  }
}

exports.FONT_TEXT_STYLE = {
  LARGE_TITLE: binding.FONT_TEXT_STYLE_LARGE_TITLE,
  TITLE1: binding.FONT_TEXT_STYLE_TITLE1,
  TITLE2: binding.FONT_TEXT_STYLE_TITLE2,
  TITLE3: binding.FONT_TEXT_STYLE_TITLE3,
  HEADLINE: binding.FONT_TEXT_STYLE_HEADLINE,
  SUBHEADLINE: binding.FONT_TEXT_STYLE_SUBHEADLINE,
  BODY: binding.FONT_TEXT_STYLE_BODY,
  CALLOUT: binding.FONT_TEXT_STYLE_CALLOUT,
  FOOTNOTE: binding.FONT_TEXT_STYLE_FOOTNOTE,
  CAPTION1: binding.FONT_TEXT_STYLE_CAPTION1,
  CAPTION2: binding.FONT_TEXT_STYLE_CAPTION2
}
