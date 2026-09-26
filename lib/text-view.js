const binding = require('../binding')
const { adopt, expose } = require('./handle')
const wrap = require('./wrap')
const UIKitColor = require('./color')
const UIKitFont = require('./font')
const UIKitScrollView = require('./scroll-view')

module.exports = exports = class UIKitTextView extends UIKitScrollView {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.textViewInit(x, y, width, height, this)
  }

  get text() {
    return binding.textViewText(this._tag)
  }

  set text(text) {
    binding.textViewText(this._tag, text)
  }

  get font() {
    return wrap(UIKitFont, binding.textViewFont(this._tag))
  }

  set font(font) {
    binding.textViewFont(this._tag, adopt(font))
  }

  get textColor() {
    return wrap(UIKitColor, binding.textViewTextColor(this._tag))
  }

  set textColor(textColor) {
    binding.textViewTextColor(this._tag, adopt(textColor))
  }

  get textAlignment() {
    return binding.textViewTextAlignment(this._tag)
  }

  set textAlignment(textAlignment) {
    binding.textViewTextAlignment(this._tag, textAlignment)
  }

  get editable() {
    return binding.textViewEditable(this._tag)
  }

  set editable(editable) {
    binding.textViewEditable(this._tag, editable)
  }

  get keyboardType() {
    return binding.textViewKeyboardType(this._tag)
  }

  set keyboardType(keyboardType) {
    binding.textViewKeyboardType(this._tag, keyboardType)
  }

  get returnKeyType() {
    return binding.textViewReturnKeyType(this._tag)
  }

  set returnKeyType(returnKeyType) {
    binding.textViewReturnKeyType(this._tag, returnKeyType)
  }

  get autocapitalizationType() {
    return binding.textViewAutocapitalizationType(this._tag)
  }

  set autocapitalizationType(autocapitalizationType) {
    binding.textViewAutocapitalizationType(this._tag, autocapitalizationType)
  }

  get autocorrectionType() {
    return binding.textViewAutocorrectionType(this._tag)
  }

  set autocorrectionType(autocorrectionType) {
    binding.textViewAutocorrectionType(this._tag, autocorrectionType)
  }

  get scrollEnabled() {
    return binding.textViewScrollEnabled(this._tag)
  }

  set scrollEnabled(scrollEnabled) {
    binding.textViewScrollEnabled(this._tag, scrollEnabled)
  }

  get selectedRange() {
    return binding.textViewSelectedRange(this._tag)
  }

  set selectedRange(selectedRange) {
    const { location = 0, length = 0 } = selectedRange

    binding.textViewSelectedRange(this._tag, location, length)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitTextView }
    }
  }

  _ondidbeginediting() {
    this.emit('didBeginEditing')
  }

  _ondidendediting() {
    this.emit('didEndEditing')
  }

  _ondidchange() {
    this.emit('didChange')
  }

  _ondidchangeselection() {
    this.emit('didChangeSelection')
  }

  _onshouldchangetext(location, length, string) {
    this.emit('shouldChangeText', { location, length, string })
  }
}

exports._events = {
  didBeginEditing: binding.TEXT_VIEW_EVENT_DID_BEGIN_EDITING,
  didEndEditing: binding.TEXT_VIEW_EVENT_DID_END_EDITING,
  didChange: binding.TEXT_VIEW_EVENT_DID_CHANGE,
  shouldChangeText: binding.TEXT_VIEW_EVENT_SHOULD_CHANGE_TEXT,
  didChangeSelection: binding.TEXT_VIEW_EVENT_DID_CHANGE_SELECTION
}

expose(exports)
