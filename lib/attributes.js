const UIKitColor = require('./color')
const UIKitFont = require('./font')
const UIKitParagraphStyle = require('./paragraph-style')
const wrap = require('./wrap')

// Attributes hold wrappers in JavaScript and tags in native code.
const WRAPPERS = {
  font: UIKitFont,
  foregroundColor: UIKitColor,
  backgroundColor: UIKitColor,
  paragraphStyle: UIKitParagraphStyle
}

exports.unwrapAttributes = function unwrapAttributes(attributes) {
  const result = {}

  for (const [key, value] of Object.entries(attributes)) {
    result[key] = key in WRAPPERS && value !== null ? value._tag : value
  }

  return result
}

exports.wrapAttributes = function wrapAttributes(attributes) {
  const result = {}

  for (const [key, value] of Object.entries(attributes)) {
    const Wrapper = WRAPPERS[key]

    result[key] = Wrapper ? wrap(Wrapper, value) : value
  }

  return result
}
