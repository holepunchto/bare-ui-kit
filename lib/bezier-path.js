const binding = require('../binding')
const { expose } = require('./handle')

module.exports = exports = class UIKitBezierPath {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag === null ? binding.bezierPathInit() : tag

    this._token = binding.claim(this._tag, this)
  }

  moveTo(x, y) {
    binding.bezierPathMoveTo(this._tag, x, y)
    return this
  }

  lineTo(x, y) {
    binding.bezierPathLineTo(this._tag, x, y)
    return this
  }

  curveTo(x, y, x1, y1, x2, y2) {
    binding.bezierPathCurveTo(this._tag, x, y, x1, y1, x2, y2)
    return this
  }

  addArc(x, y, radius, startAngle, endAngle) {
    binding.bezierPathAddArc(this._tag, x, y, radius, startAngle, endAngle)
    return this
  }

  closePath() {
    binding.bezierPathClose(this._tag)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitBezierPath }
    }
  }
}

expose(exports)
