const { type } = require('bare-build/constants')

const { EXECUTABLE } = type

require.asset = require('require-asset')

const prebuilds = {}

exports.prebuilds = prebuilds

prebuilds['ios-arm64'] = () => {
  return {
    type: EXECUTABLE,
    path: require.asset('./prebuilds/ios-arm64/bare', __filename),
    dependencies: []
  }
}

prebuilds['ios-arm64-simulator'] = () => {
  return {
    type: EXECUTABLE,
    path: require.asset('./prebuilds/ios-arm64-simulator/bare', __filename),
    dependencies: []
  }
}

prebuilds['ios-x64-simulator'] = () => {
  return {
    type: EXECUTABLE,
    path: require.asset('./prebuilds/ios-x64-simulator/bare', __filename),
    dependencies: []
  }
}
