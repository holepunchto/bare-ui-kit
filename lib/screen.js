const binding = require('../binding')
const scratch = require('./scratch')
const wrap = require('./wrap')

module.exports = exports = class UIKitScreen {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  static get mainScreen() {
    return wrap(UIKitScreen, binding.screenMainScreen())
  }

  get bounds() {
    binding.screenBoundsInto(this._tag, scratch.buffer, 0)

    return { x: scratch[0], y: scratch[1], width: scratch[2], height: scratch[3] }
  }

  get nativeBounds() {
    binding.screenNativeBoundsInto(this._tag, scratch.buffer, 0)

    return { x: scratch[0], y: scratch[1], width: scratch[2], height: scratch[3] }
  }

  get scale() {
    return binding.screenScale(this._tag)
  }

  get nativeScale() {
    return binding.screenNativeScale(this._tag)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitScreen }
    }
  }
}
