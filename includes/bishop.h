#pragma once

#include <iostream>

#include "piece.h"

class Board;
class DangerMap;

class Bishop :public Piece{
private:
public:
    Bishop(ColorType color,std::pair<int,int> position);

};