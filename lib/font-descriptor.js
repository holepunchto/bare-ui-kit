const binding = require('../binding')
const registry = require('bare-foundation-registry')
const wrap = require('./wrap')

module.exports = exports = class UIKitFontDescriptor {
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

  get symbolicTraits() {
    return binding.fontDescriptorSymbolicTraits(this._tag)
  }

  withSymbolicTraits(traits) {
    return wrap(UIKitFontDescriptor, binding.fontDescriptorWithSymbolicTraits(this._tag, traits))
  }

  withFamily(family) {
    return wrap(UIKitFontDescriptor, binding.fontDescriptorWithFamily(this._tag, family))
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitFontDescriptor }
    }
  }
}

exports.SYMBOLIC_TRAITS = {
  ITALIC: binding.FONT_DESCRIPTOR_TRAIT_ITALIC,
  BOLD: binding.FONT_DESCRIPTOR_TRAIT_BOLD,
  EXPANDED: binding.FONT_DESCRIPTOR_TRAIT_EXPANDED,
  CONDENSED: binding.FONT_DESCRIPTOR_TRAIT_CONDENSED,
  MONO_SPACE: binding.FONT_DESCRIPTOR_TRAIT_MONO_SPACE
}
