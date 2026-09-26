const binding = require('../binding')
const UIKitView = require('./view')

module.exports = exports = class UIKitScrollView extends UIKitView {
  // A `UIScrollView` is not the view subclass that delivers touches, so what
  // it reports is its own and does not include the events a view has.
  static _events = {
    didScroll: binding.SCROLL_VIEW_EVENT_DID_SCROLL
  }

  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.scrollViewInit(x, y, width, height, this)
  }

  _ondidscroll(x, y) {
    this.emit('didScroll', { x, y })
  }

  get contentSize() {
    return binding.scrollViewContentSize(this._tag)
  }

  set contentSize({ width = 0, height = 0 }) {
    binding.scrollViewContentSize(this._tag, width, height)
  }

  get contentOffset() {
    return binding.scrollViewContentOffset(this._tag)
  }

  set contentOffset({ x = 0, y = 0 }) {
    binding.scrollViewContentOffset(this._tag, x, y)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitScrollView }
    }
  }
}
