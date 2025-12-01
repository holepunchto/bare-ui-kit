require.asset = require('require-asset')

const prebuilds = {}

exports.prebuilds = prebuilds

prebuilds['ios-arm64'] = () => {
  return require.asset('./prebuilds/ios-arm64/bare', __filename)
}

prebuilds['ios-arm64-simulator'] = () => {
  return require.asset('./prebuilds/ios-arm64-simulator/bare', __filename)
}

prebuilds['ios-x64-simulator'] = () => {
  return require.asset('./prebuilds/ios-x64-simulator/bare', __filename)
}
