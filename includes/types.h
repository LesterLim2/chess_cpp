#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

enum class PieceType { None, Pawn, Bishop, Knight, Rook, Queen, King };
enum class ColorType { White, Black, None };
enum class moveType {Move,Capture};

inline std::vector<std::string> backRowVector = {
    "Rook",
    "Knight",
    "Bishop",
    "Queen",
    "King",
    "Bishop",
    "Knight",
    "Rook"
};
inline std::unordered_map<std::string,PieceType> stringToPieceType = {
    {"Pawn",   PieceType::Pawn},
    {"Bishop", PieceType::Bishop},
    {"Knight", PieceType::Knight},
    {"Rook",   PieceType::Rook},
    {"Queen",  PieceType::Queen},
    {"King",   PieceType::King},
    {"None",   PieceType::None}
};
inline std::unordered_map<std::string,ColorType> stringToColorType = {
    {"White", ColorType::White},
    {"Black", ColorType::Black},
    {"None",  ColorType::None}
};
inline std::unordered_map<char,PieceType> charToPieceType = {
    {'p', PieceType::Pawn},
    {'b', PieceType::Bishop},
    {'n', PieceType::Knight},
    {'r', PieceType::Rook},
    {'q', PieceType::Queen},
    {'k', PieceType::King},
    {'o', PieceType::None}
};
inline std::unordered_map<char,ColorType> charToColorType = {
    {'w', ColorType::White},
    {'b', ColorType::Black},
    {'o', ColorType::None}
};
inline int rankFromIndex(int y) { return y + 1; }
inline char fileFromIndex(int x) { return 'A' + x; }
