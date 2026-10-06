const { test } = require('bare-tap')
const DisplayLink = require('bare-core-animation/display-link')
const RunLoop = require('bare-foundation/run-loop')
const { afterAnimationFrame } = require('bare-animation-frame')
const { open } = require('./helpers')

test('waits for a frame to be displayed', async (t) => {
  open(t)

  const link = new DisplayLink()
  t.teardown(() => link.invalidate())

  let ticks = 0

  link.on('tick', () => ticks++)
  link.addToRunLoop(RunLoop.main, RunLoop.MODE.COMMON)

  await afterAnimationFrame()

  t.ok(ticks > 0)
})

test('lays the root view out to fill the window', (t) => {
  const { window, controller } = open(t)

  t.deepStrictEqual(controller.view.frame, window.bounds, 'when shown')

  window.frame = { x: 0, y: 0, width: 200, height: 300 }

  t.deepStrictEqual(
    controller.view.frame,
    { x: 0, y: 0, width: 200, height: 300 },
    'after a resize'
  )
})

test('insets the safe area', (t) => {
  const { window, controller } = open(t)

  t.ok(controller.view.safeAreaInsets.top > 0, 'below the status bar')
  t.deepStrictEqual(controller.view.safeAreaInsets, window.safeAreaInsets, 'same as the window')
})
