const binding = require('../binding')
const scratch = require('./scratch')
const UIKitView = require('./view')

module.exports = exports = class UIKitScrollView extends UIKitView {
  // A scroll view does not report touches, so it has its own events instead of
  // those of a view.
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

  get contentInsetAdjustmentBehavior() {
    return binding.scrollViewContentInsetAdjustmentBehavior(this._tag)
  }

  set contentInsetAdjustmentBehavior(behavior) {
    binding.scrollViewContentInsetAdjustmentBehavior(this._tag, behavior)
  }

  get adjustedContentInset() {
    binding.scrollViewAdjustedContentInsetInto(this._tag, scratch.buffer, 0)

    return { top: scratch[0], left: scratch[1], bottom: scratch[2], right: scratch[3] }
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitScrollView }
    }
  }
}

exports.CONTENT_INSET_ADJUSTMENT_BEHAVIOR = {
  AUTOMATIC: binding.SCROLL_VIEW_CONTENT_INSET_ADJUSTMENT_BEHAVIOR_AUTOMATIC,
  SCROLLABLE_AXES: binding.SCROLL_VIEW_CONTENT_INSET_ADJUSTMENT_BEHAVIOR_SCROLLABLE_AXES,
  NEVER: binding.SCROLL_VIEW_CONTENT_INSET_ADJUSTMENT_BEHAVIOR_NEVER,
  ALWAYS: binding.SCROLL_VIEW_CONTENT_INSET_ADJUSTMENT_BEHAVIOR_ALWAYS
}
