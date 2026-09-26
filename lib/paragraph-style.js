const binding = require('../binding')
const registry = require('bare-foundation-registry')

module.exports = exports = class UIKitParagraphStyle {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag === null ? binding.paragraphStyleInit() : tag

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  get alignment() {
    return binding.paragraphStyleAlignment(this._tag)
  }

  set alignment(value) {
    binding.paragraphStyleAlignment(this._tag, value)
  }

  get lineBreakMode() {
    return binding.paragraphStyleLineBreakMode(this._tag)
  }

  set lineBreakMode(value) {
    binding.paragraphStyleLineBreakMode(this._tag, value)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitParagraphStyle }
    }
  }
}
