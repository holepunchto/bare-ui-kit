const binding = require('../binding')
const registry = require('bare-foundation-registry')
const wrap = require('./wrap')

module.exports = exports = class UIKitPasteboard {
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

  static get generalPasteboard() {
    return wrap(UIKitPasteboard, binding.pasteboardGeneralPasteboard())
  }

  get string() {
    return binding.pasteboardString(this._tag)
  }

  set string(string) {
    binding.pasteboardString(this._tag, string)
  }

  get hasStrings() {
    return binding.pasteboardHasStrings(this._tag)
  }

  get hasURLs() {
    return binding.pasteboardHasUrls(this._tag)
  }

  get hasImages() {
    return binding.pasteboardHasImages(this._tag)
  }

  get hasColors() {
    return binding.pasteboardHasColors(this._tag)
  }

  get numberOfItems() {
    return binding.pasteboardNumberOfItems(this._tag)
  }

  get changeCount() {
    return binding.pasteboardChangeCount(this._tag)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitPasteboard }
    }
  }
}
