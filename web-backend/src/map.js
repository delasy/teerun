class Map {
  constructor () {
    this._tiles = [
      { x: 0, y: 0 },
      { x: 1, y: 0 },
      { x: 2, y: 0 },
      { x: 3, y: 0 },
      { x: 4, y: 0 },
      { x: 4, y: 1 }
    ]
  }

  toJSON () {
    return {
      tiles: this._tiles
    }
  }
}

module.exports = Map

