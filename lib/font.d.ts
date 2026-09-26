import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitFontDescriptor = require('./font-descriptor')

/** A font, as a `UIFont`. Sizes are in points. */
interface UIKitFont {
  readonly fontDescriptor: UIKitFontDescriptor

  readonly fontName: string

  readonly familyName: string

  readonly pointSize: number

  readonly ascender: number

  readonly descender: number

  readonly capHeight: number

  readonly xHeight: number

  readonly leading: number

  readonly lineHeight: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitFont {
  protected constructor()

  /** The system font. `weight` is a `WEIGHT` constant and defaults to regular. */
  static systemFont(size: number, weight?: number): UIKitFont

  static boldSystemFont(size: number): UIKitFont

  static monospacedSystemFont(size: number, weight?: number): UIKitFont

  /** The font called `name`, or `null` if it is not installed. */
  static withName(name: string, size: number): UIKitFont | null

  static withDescriptor(descriptor: UIKitFontDescriptor, size: number): UIKitFont

  static readonly WEIGHT: {
    readonly ULTRA_LIGHT: number
    readonly THIN: number
    readonly LIGHT: number
    readonly REGULAR: number
    readonly MEDIUM: number
    readonly SEMIBOLD: number
    readonly BOLD: number
    readonly HEAVY: number
    readonly BLACK: number
  }
}

export = UIKitFont
