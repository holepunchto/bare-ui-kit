const binding = require('../binding')
const { expose } = require('./handle')
const scratch = require('./scratch')
const wrap = require('./wrap')

module.exports = exports = class UIKitColor {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag

    this._token = binding.claim(this._tag, this)
  }

  static rgb(red, green, blue, alpha = 1) {
    return new UIKitColor({ tag: binding.colorRGB(red, green, blue, alpha) })
  }

  static hsb(hue, saturation, brightness, alpha = 1) {
    return new UIKitColor({ tag: binding.colorHSB(hue, saturation, brightness, alpha) })
  }

  static white(white, alpha = 1) {
    return new UIKitColor({ tag: binding.colorWhite(white, alpha) })
  }

  static get blackColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_BLACK))
  }

  static get whiteColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_WHITE))
  }

  static get clearColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_CLEAR))
  }

  static get labelColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_LABEL))
  }

  static get secondaryLabelColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SECONDARY_LABEL))
  }

  static get tertiaryLabelColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_TERTIARY_LABEL))
  }

  static get quaternaryLabelColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_QUATERNARY_LABEL))
  }

  static get placeholderTextColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_PLACEHOLDER_TEXT))
  }

  static get separatorColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SEPARATOR))
  }

  static get opaqueSeparatorColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_OPAQUE_SEPARATOR))
  }

  static get linkColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_LINK))
  }

  static get systemBackgroundColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_BACKGROUND))
  }

  static get secondarySystemBackgroundColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SECONDARY_SYSTEM_BACKGROUND))
  }

  static get tertiarySystemBackgroundColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_TERTIARY_SYSTEM_BACKGROUND))
  }

  static get systemGroupedBackgroundColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_GROUPED_BACKGROUND))
  }

  static get systemFillColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_FILL))
  }

  static get tintColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_TINT))
  }

  static get systemRedColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_RED))
  }

  static get systemOrangeColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_ORANGE))
  }

  static get systemYellowColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_YELLOW))
  }

  static get systemGreenColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_GREEN))
  }

  static get systemMintColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_MINT))
  }

  static get systemTealColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_TEAL))
  }

  static get systemCyanColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_CYAN))
  }

  static get systemBlueColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_BLUE))
  }

  static get systemIndigoColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_INDIGO))
  }

  static get systemPurpleColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_PURPLE))
  }

  static get systemPinkColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_PINK))
  }

  static get systemBrownColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_BROWN))
  }

  static get systemGrayColor() {
    return wrap(UIKitColor, binding.colorSystem(binding.COLOR_SYSTEM_GRAY))
  }

  get components() {
    binding.colorComponentsInto(this._tag, scratch.buffer, 0)

    return { red: scratch[0], green: scratch[1], blue: scratch[2], alpha: scratch[3] }
  }

  withAlphaComponent(alpha) {
    return wrap(UIKitColor, binding.colorWithAlphaComponent(this._tag, alpha))
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: UIKitColor }
    }
  }
}

expose(exports)
