const { test } = require('bare-tap')
const { afterAnimationFrame } = require('bare-animation-frame')
const { Scene, Screen, ViewController, Window } = require('..')
const { open } = require('./helpers')

test('fills its scene', (t) => {
  const { window } = open(t)

  t.equal(window.keyWindow, true, 'key')
  t.ok(window.windowScene === Scene.connected[0], 'scene')
  t.deepStrictEqual(window.frame, Screen.mainScreen.bounds, 'frame')
})

test('takes a frame', (t) => {
  const window = new Window({ scene: Scene.connected[0], width: 100 })

  t.deepStrictEqual(window.frame, { x: 0, y: 0, width: 100, height: 0 })
})

test('replaces its root view controller', async (t) => {
  const { window, controller } = open(t)

  t.ok(window.rootViewController === controller, 'same wrapper')

  await afterAnimationFrame()

  const other = new ViewController()

  window.rootViewController = other

  t.ok(window.rootViewController === other, 'replaced')
})
