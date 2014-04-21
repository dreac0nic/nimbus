#ifndef NIMBUS_MAP_H
#define NIMBUS_MAP_H

namespace Nimbus
{
    class Map
    {
    public:
        Map(void);
        ~Map(void);

    private:
        std::vector<Tile> mTileList;

    public:
        std::vector<Tile> getTileList(void) { return mTileList; }
        void update(void);
    };
}

#endif NIMBUS_MAP_H
