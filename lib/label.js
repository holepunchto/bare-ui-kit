const binding = require('../binding')
const { adopt, expose } = require('./handle')
const wrap = require('./wrap')
const UIKitAttributedString = require('./attributed-string')
const UIKitColor = require('./color')
const UIKitFont = require('./font')
const UIKitView = require('./view')

module.exports = exports = class UIKitLabel extends UIKitView {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.labelInit(x, y, width, height)
  }

  get text() {
    return binding.labelText(this._tag)
  }

  set text(text) {
    binding.labelText(this._tag, text)
  }

  get attributedText() {
    return wrap(UIKitAttributedString, binding.labelAttributedText(this._tag))
  }

  set attributedText(value) {
    binding.labelAttributedText(this._tag, adopt(value))
  }

  get font() {
    return wrap(UIKitFont, binding.labelFont(this._tag))
  }

  set font(font) {
    binding.labelFont(this._tag, adopt(font))
  }

  get textColor() {
    return wrap(UIKitColor, binding.labelTextColor(this._tag))
  }

  set textColor(textColor) {
    binding.labelTextColor(this._tag, adopt(textColor))
  }

  get textAlignment() {
    return binding.labelTextAlignment(this._tag)
  }

  set textAlignment(textAlignment) {
    binding.labelTextAlignment(this._tag, textAlignment)
  }

  get numberOfLines() {
    return binding.labelNumberOfLines(this._tag)
  }

  set numberOfLines(numberOfLines) {
    binding.labelNumberOfLines(this._tag, numberOfLines)
  }

  get lineBreakMode() {
    return binding.labelLineBreakMode(this._tag)
  }

  set lineBreakMode(lineBreakMode) {
    binding.labelLineBreakMode(this._tag, lineBreakMode)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitLabel }
    }
  }
}

exports.ALIGNMENT = {
  LEFT: binding.TEXT_ALIGNMENT_LEFT,
  RIGHT: binding.TEXT_ALIGNMENT_RIGHT,
  CENTER: binding.TEXT_ALIGNMENT_CENTER,
  JUSTIFIED: binding.TEXT_ALIGNMENT_JUSTIFIED,
  NATURAL: binding.TEXT_ALIGNMENT_NATURAL
}

exports.LINE_BREAK_MODE = {
  WORD_WRAPPING: binding.LINE_BREAK_MODE_WORD_WRAPPING,
  CHAR_WRAPPING: binding.LINE_BREAK_MODE_CHAR_WRAPPING,
  CLIPPING: binding.LINE_BREAK_MODE_CLIPPING,
  TRUNCATING_HEAD: binding.LINE_BREAK_MODE_TRUNCATING_HEAD,
  TRUNCATING_TAIL: binding.LINE_BREAK_MODE_TRUNCATING_TAIL,
  TRUNCATING_MIDDLE: binding.LINE_BREAK_MODE_TRUNCATING_MIDDLE
}

expose(exports)
