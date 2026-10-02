import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** The clipboard, as a `UIPasteboard`. */
interface UIKitPasteboard {
  string: string | null

  readonly hasStrings: boolean

  readonly hasURLs: boolean

  readonly hasImages: boolean

  readonly hasColors: boolean

  readonly numberOfItems: number

  /** A number that changes every time the contents change. */
  readonly changeCount: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitPasteboard {
  protected constructor()

  static readonly generalPasteboard: UIKitPasteboard
}

export = UIKitPasteboard
