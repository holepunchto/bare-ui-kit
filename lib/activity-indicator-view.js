const binding = require('../binding')
const { adopt } = require('./handle')
const wrap = require('./wrap')
const UIKitColor = require('./color')
const UIKitView = require('./view')

module.exports = exports = class UIKitActivityIndicatorView extends UIKitView {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.activityIndicatorViewInit(x, y, width, height)
  }

  get style() {
    return binding.activityIndicatorViewStyle(this._tag)
  }

  set style(value) {
    binding.activityIndicatorViewStyle(this._tag, value)
  }

  get color() {
    return wrap(UIKitColor, binding.activityIndicatorViewColor(this._tag))
  }

  set color(value) {
    binding.activityIndicatorViewColor(this._tag, value === null ? null : adopt(value))
  }

  get hidesWhenStopped() {
    return binding.activityIndicatorViewHidesWhenStopped(this._tag)
  }

  set hidesWhenStopped(value) {
    binding.activityIndicatorViewHidesWhenStopped(this._tag, value)
  }

  // Started and stopped through one property here, where UIKit has a method
  // for each and a third to ask.
  get animating() {
    return binding.activityIndicatorViewAnimating(this._tag)
  }

  set animating(value) {
    binding.activityIndicatorViewAnimating(this._tag, value)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitActivityIndicatorView }
    }
  }
}

exports.STYLE = {
  MEDIUM: binding.ACTIVITY_INDICATOR_VIEW_STYLE_MEDIUM,
  LARGE: binding.ACTIVITY_INDICATOR_VIEW_STYLE_LARGE
}
