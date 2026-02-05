#pragma once
#include <iostream>
#include <unordered_map>

enum class PieceType { None, Pawn, Bishop, Knight, Rook, Queen, King };
enum class ColorType { White, Black, None };
enum class moveType {Move,Capture};

inline int rankFromIndex(int y) { return y + 1; }
inline char fileFromIndex(int x) { return 'A' + x; }
