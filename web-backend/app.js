const GameServer = require('./src/game-server')

const gameServer = new GameServer().start()

process.on('exit', () => {
  gameServer.stop()
})
