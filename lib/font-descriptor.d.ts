import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A description of a font, as a `UIFontDescriptor`. */
interface UIKitFontDescriptor {
  /** The traits of the font, as `SYMBOLIC_TRAITS` flags combined with `|`. */
  readonly symbolicTraits: number

  /** The same font with `traits`, or `null` if it has no such variant. */
  withSymbolicTraits(traits: number): UIKitFontDescriptor | null

  withFamily(family: string): UIKitFontDescriptor | null

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitFontDescriptor {
  protected constructor()

  static readonly SYMBOLIC_TRAITS: {
    readonly ITALIC: number
    readonly BOLD: number
    readonly EXPANDED: number
    readonly CONDENSED: number
    readonly MONO_SPACE: number
  }
}

export = UIKitFontDescriptor
