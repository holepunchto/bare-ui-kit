import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** An image, as a `UIImage`. */
interface UIKitImage {
  /** The size in points. */
  readonly size: { width: number; height: number }

  readonly scale: number

  /** Whether the image is drawn as is or tinted, as a `RENDERING_MODE` constant. */
  readonly renderingMode: number

  /** The same image with another rendering mode. */
  withRenderingMode(renderingMode: number): UIKitImage

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitImage {
  protected constructor()

  /** Load an image file, or return `null` if it cannot be read. */
  static withContentsOfFile(path: string): UIKitImage | null

  /** An image from the app bundle, or `null` if there is none by that name. */
  static named(name: string): UIKitImage | null

  static readonly RENDERING_MODE: {
    readonly AUTOMATIC: number
    readonly ALWAYS_ORIGINAL: number
    readonly ALWAYS_TEMPLATE: number
  }
}

export = UIKitImage
