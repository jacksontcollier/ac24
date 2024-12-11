#ifndef __GUARD_MAP_TYPES_H__
#define __GUARD_MAP_TYPES_H__

#include <utility>

using namespace std;

typedef pair<signed long long, signed long long> BoardPos;

enum GuardOrientation {
    LEFT_GUARD_FACE,
    UP_GUARD_FACE,
    RIGHT_GUARD_FACE,
    DOWN_GUARD_FACE
};

struct GuardPos {
    BoardPos pos;
    GuardOrientation orientation;
};


#endif