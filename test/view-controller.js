const { test } = require('bare-tap')
const { afterAnimationFrame } = require('bare-animation-frame')
const { View, ViewController } = require('..')
const { open } = require('./helpers')

test('keeps its view', (t) => {
  const controller = new ViewController()

  t.ok(controller.view !== null, 'has a view')
  t.ok(controller.view === controller.view, 'same wrapper')

  const view = new View()

  controller.view = view

  t.ok(controller.view === view, 'replaced')
})

test('emits from its own view', async (t) => {
  const { controller } = open(t)

  let changes = 0

  controller.view.on('traitChange', () => changes++)
  controller.view.overrideUserInterfaceStyle = View.USER_INTERFACE_STYLE.DARK

  await afterAnimationFrame()

  t.ok(changes > 0)
})

test('emits after laying out its view', async (t) => {
  const { controller } = open(t)

  await afterAnimationFrame()

  let layouts = 0

  controller.on('didLayoutSubviews', () => layouts++)
  controller.view.setNeedsLayout()

  t.equal(layouts, 0, 'not before')

  await afterAnimationFrame()

  t.equal(layouts, 1, 'after')
})
