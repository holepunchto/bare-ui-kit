const binding = require('../binding')
const { adopt, expose } = require('./handle')
const wrap = require('./wrap')
const UIKitImage = require('./image')
const UIKitView = require('./view')

module.exports = exports = class UIKitImageView extends UIKitView {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.imageViewInit(x, y, width, height)
  }

  get image() {
    return wrap(UIKitImage, binding.imageViewImage(this._tag))
  }

  set image(image) {
    binding.imageViewImage(this._tag, adopt(image))
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitImageView }
    }
  }
}

expose(exports)
