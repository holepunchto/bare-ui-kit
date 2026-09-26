const binding = require('../binding')
const { expose } = require('./handle')
const scratch = require('./scratch')
const wrap = require('./wrap')

module.exports = exports = class UIKitImage {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  static withContentsOfFile(path) {
    return wrap(UIKitImage, binding.imageWithContentsOfFile(path))
  }

  static named(name) {
    return wrap(UIKitImage, binding.imageNamed(name))
  }

  get size() {
    binding.imageSizeInto(this._tag, scratch.buffer, 0)

    return { width: scratch[0], height: scratch[1] }
  }

  get scale() {
    return binding.imageScale(this._tag)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitImage }
    }
  }
}

expose(exports)
