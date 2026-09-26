import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitScreen = require('./screen')

/** A scene the app shows its windows in, as a `UIWindowScene`. */
interface UIKitScene {
  readonly screen: UIKitScreen | null

  /** Whether the scene is in the foreground, as an `ACTIVATION_STATE` constant. */
  readonly activationState: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitScene {
  protected constructor()

  /** The scenes that are connected. Put a window in one to show it. */
  static readonly connected: UIKitScene[]

  static readonly ACTIVATION_STATE: {
    readonly UNATTACHED: number
    readonly FOREGROUND_ACTIVE: number
    readonly FOREGROUND_INACTIVE: number
    readonly BACKGROUND: number
  }
}

export = UIKitScene
