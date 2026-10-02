import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A screen, as a `UIScreen`. */
interface UIKitScreen {
  /** The bounds in points. */
  readonly bounds: { x: number; y: number; width: number; height: number }

  /** The bounds in pixels. */
  readonly nativeBounds: { x: number; y: number; width: number; height: number }

  readonly scale: number

  readonly nativeScale: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitScreen {
  protected constructor()

  static readonly mainScreen: UIKitScreen
}

export = UIKitScreen
