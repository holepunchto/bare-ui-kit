const binding = require('../binding')
const { adopt } = require('./handle')
const wrap = require('./wrap')
const UIKitColor = require('./color')
const UIKitFont = require('./font')
const UIKitView = require('./view')

module.exports = exports = class UIKitTextField extends UIKitView {
  _init(opts) {
    const { x = 0, y = 0, width = 0, height = 0 } = opts

    return binding.textFieldInit(x, y, width, height, this)
  }

  get text() {
    return binding.textFieldText(this._tag)
  }

  set text(text) {
    binding.textFieldText(this._tag, text)
  }

  get placeholder() {
    return binding.textFieldPlaceholder(this._tag)
  }

  set placeholder(placeholder) {
    binding.textFieldPlaceholder(this._tag, placeholder)
  }

  get font() {
    return wrap(UIKitFont, binding.textFieldFont(this._tag))
  }

  set font(font) {
    binding.textFieldFont(this._tag, adopt(font))
  }

  get textColor() {
    return wrap(UIKitColor, binding.textFieldTextColor(this._tag))
  }

  set textColor(textColor) {
    binding.textFieldTextColor(this._tag, adopt(textColor))
  }

  get textAlignment() {
    return binding.textFieldTextAlignment(this._tag)
  }

  set textAlignment(textAlignment) {
    binding.textFieldTextAlignment(this._tag, textAlignment)
  }

  get secureTextEntry() {
    return binding.textFieldSecureTextEntry(this._tag)
  }

  set secureTextEntry(secureTextEntry) {
    binding.textFieldSecureTextEntry(this._tag, secureTextEntry)
  }

  get keyboardType() {
    return binding.textFieldKeyboardType(this._tag)
  }

  set keyboardType(keyboardType) {
    binding.textFieldKeyboardType(this._tag, keyboardType)
  }

  get returnKeyType() {
    return binding.textFieldReturnKeyType(this._tag)
  }

  set returnKeyType(returnKeyType) {
    binding.textFieldReturnKeyType(this._tag, returnKeyType)
  }

  get autocapitalizationType() {
    return binding.textFieldAutocapitalizationType(this._tag)
  }

  set autocapitalizationType(autocapitalizationType) {
    binding.textFieldAutocapitalizationType(this._tag, autocapitalizationType)
  }

  get autocorrectionType() {
    return binding.textFieldAutocorrectionType(this._tag)
  }

  set autocorrectionType(autocorrectionType) {
    binding.textFieldAutocorrectionType(this._tag, autocorrectionType)
  }

  get enabled() {
    return binding.textFieldEnabled(this._tag)
  }

  set enabled(enabled) {
    binding.textFieldEnabled(this._tag, enabled)
  }

  get selectedRange() {
    return binding.textFieldSelectedRange(this._tag)
  }

  set selectedRange(selectedRange) {
    const { location = 0, length = 0 } = selectedRange

    binding.textFieldSelectedRange(this._tag, location, length)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitTextField }
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

  _onshouldchangecharacters(location, length, string) {
    this.emit('shouldChangeCharacters', { location, length, string })
  }

  _onshouldreturn() {
    this.emit('shouldReturn')
  }
}

exports._events = {
  didBeginEditing: binding.TEXT_FIELD_EVENT_DID_BEGIN_EDITING,
  didEndEditing: binding.TEXT_FIELD_EVENT_DID_END_EDITING,
  didChange: binding.TEXT_FIELD_EVENT_DID_CHANGE,
  shouldChangeCharacters: binding.TEXT_FIELD_EVENT_SHOULD_CHANGE_CHARACTERS,
  shouldReturn: binding.TEXT_FIELD_EVENT_SHOULD_RETURN,
  didChangeSelection: binding.TEXT_FIELD_EVENT_DID_CHANGE_SELECTION
}

exports.KEYBOARD_TYPE = {
  DEFAULT: binding.KEYBOARD_TYPE_DEFAULT,
  NUMBER_PAD: binding.KEYBOARD_TYPE_NUMBER_PAD,
  DECIMAL_PAD: binding.KEYBOARD_TYPE_DECIMAL_PAD,
  NUMBERS_AND_PUNCTUATION: binding.KEYBOARD_TYPE_NUMBERS_AND_PUNCTUATION,
  EMAIL_ADDRESS: binding.KEYBOARD_TYPE_EMAIL_ADDRESS,
  PHONE_PAD: binding.KEYBOARD_TYPE_PHONE_PAD,
  URL: binding.KEYBOARD_TYPE_URL
}

exports.RETURN_KEY_TYPE = {
  DEFAULT: binding.RETURN_KEY_TYPE_DEFAULT,
  DONE: binding.RETURN_KEY_TYPE_DONE,
  GO: binding.RETURN_KEY_TYPE_GO,
  NEXT: binding.RETURN_KEY_TYPE_NEXT,
  SEARCH: binding.RETURN_KEY_TYPE_SEARCH,
  SEND: binding.RETURN_KEY_TYPE_SEND
}

exports.AUTOCAPITALIZATION_TYPE = {
  NONE: binding.AUTOCAPITALIZATION_TYPE_NONE,
  WORDS: binding.AUTOCAPITALIZATION_TYPE_WORDS,
  SENTENCES: binding.AUTOCAPITALIZATION_TYPE_SENTENCES,
  ALL_CHARACTERS: binding.AUTOCAPITALIZATION_TYPE_ALL_CHARACTERS
}

exports.AUTOCORRECTION_TYPE = {
  DEFAULT: binding.AUTOCORRECTION_TYPE_DEFAULT,
  NO: binding.AUTOCORRECTION_TYPE_NO,
  YES: binding.AUTOCORRECTION_TYPE_YES
}
