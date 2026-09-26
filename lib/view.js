const EventEmitter = require('bare-events')
const binding = require('../binding')
const { adopt, expose } = require('./handle')
const scratch = require('./scratch')
const observe = require('./events')
const wrap = require('./wrap')
const UIKitColor = require('./color')

const EVENTS = {
  [binding.VIEW_EVENT_TOUCHES_BEGAN]: 'touchesBegan',
  [binding.VIEW_EVENT_TOUCHES_MOVED]: 'touchesMoved',
  [binding.VIEW_EVENT_TOUCHES_ENDED]: 'touchesEnded',
  [binding.VIEW_EVENT_TOUCHES_CANCELLED]: 'touchesCancelled'
}

// The wrapper the superview already has, if any. Nothing is created for a
// lookup, because a wrapper minted here would be garbage immediately.
function holder(tag) {
  const superview = binding.viewSuperview(tag)

  if (superview === null) return null

  return binding.wrapper(superview) || null
}

module.exports = exports = class UIKitView extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { tag = null } = opts

    this._tag = tag === null ? this._init(opts) : tag
    this._retained = new Set()

    this._token = binding.claim(this._tag, this)

    observe(this)
  }

  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.viewInit(x, y, width, height, this)
  }

  get frame() {
    binding.viewFrameInto(this._tag, scratch.buffer, 0)

    return { x: scratch[0], y: scratch[1], width: scratch[2], height: scratch[3] }
  }

  set frame(frame) {
    const { x = 0, y = 0, width = 0, height = 0 } = frame

    binding.viewFrame(this._tag, x, y, width, height)
  }

  get bounds() {
    binding.viewBoundsInto(this._tag, scratch.buffer, 0)

    return { x: scratch[0], y: scratch[1], width: scratch[2], height: scratch[3] }
  }

  set bounds(bounds) {
    const { x = 0, y = 0, width = 0, height = 0 } = bounds

    binding.viewBounds(this._tag, x, y, width, height)
  }

  get hidden() {
    return binding.viewHidden(this._tag)
  }

  set hidden(hidden) {
    binding.viewHidden(this._tag, hidden)
  }

  get alpha() {
    return binding.viewAlpha(this._tag)
  }

  set alpha(alpha) {
    binding.viewAlpha(this._tag, alpha)
  }

  get contentMode() {
    return binding.viewContentMode(this._tag)
  }

  set contentMode(contentMode) {
    binding.viewContentMode(this._tag, contentMode)
  }

  get clipsToBounds() {
    return binding.viewClipsToBounds(this._tag)
  }

  set clipsToBounds(clipsToBounds) {
    binding.viewClipsToBounds(this._tag, clipsToBounds)
  }

  get userInteractionEnabled() {
    return binding.viewUserInteractionEnabled(this._tag)
  }

  set userInteractionEnabled(userInteractionEnabled) {
    binding.viewUserInteractionEnabled(this._tag, userInteractionEnabled)
  }

  get backgroundColor() {
    return wrap(UIKitColor, binding.viewBackgroundColor(this._tag))
  }

  set backgroundColor(backgroundColor) {
    binding.viewBackgroundColor(this._tag, adopt(backgroundColor))
  }

  get safeAreaInsets() {
    binding.viewSafeAreaInsetsInto(this._tag, scratch.buffer, 0)

    return { top: scratch[0], left: scratch[1], bottom: scratch[2], right: scratch[3] }
  }

  get superview() {
    return wrap(UIKitView, binding.viewSuperview(this._tag))
  }

  get subviews() {
    return binding.viewSubviews(this._tag).map((tag) => wrap(UIKitView, tag))
  }

  // Holding the wrapper is what keeps its listeners and any other JS state on
  // it alive; the native object is retained by its superview either way. Only
  // one of our views should be doing that, so the previous one lets go. UIKit
  // detaches the native view itself.
  _attach(view) {
    const tag = adopt(view)
    const superview = holder(tag)

    if (superview !== null) superview._retained.delete(view)

    this._retained.add(view)

    return tag
  }

  addSubview(view) {
    binding.viewAddSubview(this._tag, this._attach(view))
    return this
  }

  insertSubviewAtIndex(view, index) {
    binding.viewInsertSubviewAtIndex(this._tag, this._attach(view), index)
    return this
  }

  insertSubviewBelowSubview(view, sibling) {
    binding.viewInsertSubviewBelowSubview(this._tag, this._attach(view), adopt(sibling))
    return this
  }

  insertSubviewAboveSubview(view, sibling) {
    binding.viewInsertSubviewAboveSubview(this._tag, this._attach(view), adopt(sibling))
    return this
  }

  removeFromSuperview() {
    const superview = holder(this._tag)

    if (superview !== null) superview._retained.delete(this)

    binding.viewRemoveFromSuperview(this._tag)

    return this
  }

  setNeedsLayout() {
    binding.viewSetNeedsLayout(this._tag)
    return this
  }

  setNeedsDisplay() {
    binding.viewSetNeedsDisplay(this._tag)
    return this
  }

  sizeThatFits(width, height) {
    binding.viewSizeThatFitsInto(this._tag, width, height, scratch.buffer, 0)

    return { width: scratch[0], height: scratch[1] }
  }

  becomeFirstResponder() {
    return binding.viewBecomeFirstResponder(this._tag)
  }

  resignFirstResponder() {
    return binding.viewResignFirstResponder(this._tag)
  }

  get firstResponder() {
    return binding.viewIsFirstResponder(this._tag)
  }

  sizeToFit() {
    binding.viewSizeToFit(this._tag)
    return this
  }

  // One native callback carries every touch; UIKit reports the ones that
  // changed together and the binding unrolls them.
  _onevent(type, x, y, pointerId) {
    const name = EVENTS[type]

    if (name === undefined) return

    this.emit(name, { x, y, pointerId })
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitView }
    }
  }
}

// The same bits the map above reads, by the name a listener uses. Both come
// from the binding rather than being written out twice.
exports._events = {}

for (const bit in EVENTS) exports._events[EVENTS[bit]] = Number(bit)

exports.CONTENT_MODE = {
  SCALE_TO_FILL: binding.VIEW_CONTENT_MODE_SCALE_TO_FILL,
  SCALE_ASPECT_FIT: binding.VIEW_CONTENT_MODE_SCALE_ASPECT_FIT,
  SCALE_ASPECT_FILL: binding.VIEW_CONTENT_MODE_SCALE_ASPECT_FILL,
  REDRAW: binding.VIEW_CONTENT_MODE_REDRAW,
  CENTER: binding.VIEW_CONTENT_MODE_CENTER,
  TOP: binding.VIEW_CONTENT_MODE_TOP,
  BOTTOM: binding.VIEW_CONTENT_MODE_BOTTOM,
  LEFT: binding.VIEW_CONTENT_MODE_LEFT,
  RIGHT: binding.VIEW_CONTENT_MODE_RIGHT,
  TOP_LEFT: binding.VIEW_CONTENT_MODE_TOP_LEFT,
  TOP_RIGHT: binding.VIEW_CONTENT_MODE_TOP_RIGHT,
  BOTTOM_LEFT: binding.VIEW_CONTENT_MODE_BOTTOM_LEFT,
  BOTTOM_RIGHT: binding.VIEW_CONTENT_MODE_BOTTOM_RIGHT
}

expose(exports)
