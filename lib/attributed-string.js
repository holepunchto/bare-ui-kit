const binding = require('../binding')
const { adopt } = require('./handle')
const { wrapAttributes, unwrapAttributes } = require('./attributes')

module.exports = exports = class UIKitAttributedString {
  constructor(opts = {}) {
    const { tag = null, string = '', attributes = {} } = opts

    this._tag =
      tag === null ? binding.attributedStringInit(string, unwrapAttributes(attributes)) : tag

    this._token = binding.claim(this._tag, this)
  }

  get string() {
    return binding.attributedStringString(this._tag)
  }

  get length() {
    return binding.attributedStringLength(this._tag)
  }

  attributesAt(location) {
    return wrapAttributes(binding.attributedStringAttributesAt(this._tag, location))
  }

  append(other) {
    binding.attributedStringAppend(this._tag, adopt(other))
    return this
  }

  appendString(string, attributes = {}) {
    binding.attributedStringAppendString(this._tag, string, unwrapAttributes(attributes))
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitAttributedString }
    }
  }
}
