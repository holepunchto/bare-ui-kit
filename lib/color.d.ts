import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A colour, as a `UIColor`. System colours follow light and dark mode. */
interface UIKitColor {
  /** The components of the colour. A colour that has none, such as a pattern, reports zeroes. */
  readonly components: { red: number; green: number; blue: number; alpha: number }

  withAlphaComponent(alpha: number): UIKitColor

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitColor {
  protected constructor()

  /** A colour from components between 0 and 1. */
  static rgb(red: number, green: number, blue: number, alpha?: number): UIKitColor

  static hsb(hue: number, saturation: number, brightness: number, alpha?: number): UIKitColor

  static white(white: number, alpha?: number): UIKitColor

  static readonly blackColor: UIKitColor

  static readonly whiteColor: UIKitColor

  static readonly clearColor: UIKitColor

  static readonly labelColor: UIKitColor

  static readonly secondaryLabelColor: UIKitColor

  static readonly tertiaryLabelColor: UIKitColor

  static readonly quaternaryLabelColor: UIKitColor

  static readonly placeholderTextColor: UIKitColor

  static readonly separatorColor: UIKitColor

  static readonly opaqueSeparatorColor: UIKitColor

  static readonly linkColor: UIKitColor

  static readonly systemBackgroundColor: UIKitColor

  static readonly secondarySystemBackgroundColor: UIKitColor

  static readonly tertiarySystemBackgroundColor: UIKitColor

  static readonly systemGroupedBackgroundColor: UIKitColor

  static readonly systemFillColor: UIKitColor

  static readonly tintColor: UIKitColor

  static readonly systemRedColor: UIKitColor

  static readonly systemOrangeColor: UIKitColor

  static readonly systemYellowColor: UIKitColor

  static readonly systemGreenColor: UIKitColor

  static readonly systemMintColor: UIKitColor

  static readonly systemTealColor: UIKitColor

  static readonly systemCyanColor: UIKitColor

  static readonly systemBlueColor: UIKitColor

  static readonly systemIndigoColor: UIKitColor

  static readonly systemPurpleColor: UIKitColor

  static readonly systemPinkColor: UIKitColor

  static readonly systemBrownColor: UIKitColor

  static readonly systemGrayColor: UIKitColor
}

export = UIKitColor
