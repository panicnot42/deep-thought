#include "libtetrinet/Piece.hpp"

#include <assert.h>
#include <string>
#include <ostream>

#include "libtetrinet/PieceShape.hpp"

using namespace std;
using namespace std::experimental;
using namespace boost;

Piece::Piece(PieceShape shape, PieceRotation rotation)
  : Piece(defs[shape][uint8_t(rotation)])
{
}

bool Piece::operator== (const Piece& rhs) const
{
  return rotation == rhs.rotation && shape == rhs.shape && definition == rhs.definition && width == rhs.width && height == rhs.height;
}

bool Piece::operator() (uCoord x, uCoord y) const
{
  if (x >= GetWidth())
    throw out_of_range("x");
  if (y >= GetHeight())
    throw out_of_range("y");
  return definition[(y * pieceWidth) + x];
}

Piece::operator std::string() const
{
  std::string def(definition.to_string());
  std::reverse(def.begin(), def.end());
  return lexical_cast<std::string>(int(rotation)) + lexical_cast<std::string>(int(shape)) + def
    + lexical_cast<std::string>(width) + lexical_cast<std::string>(height);
}

uAxis Piece::GetWidth() const
{
  return width;
}

uAxis Piece::GetHeight() const
{
  return height;
}

PieceRotation Piece::GetRotation() const
{
  return rotation;
}

PieceShape Piece::GetShape() const
{
  return shape;
}

std::experimental::optional<uCoord> Piece::GetHeightAt(uCoord x) const
{
  return pieceBottoms[x];
}

constexpr Piece::Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, uCoord width, uCoord height)
  : rotation(rotation), shape(shape), definition(definition), width(width), height(height), pieceBottoms()
{
  for (uCoord x = 0; x < pieceWidth; ++x)
  {
    pieceBottoms[x] = nullopt;
    if (x < width)
      for (uCoord y = 0; y < height; ++y)
	if ((*this)(x, y))
	  pieceBottoms[x] = y;
  }
}

//TODO ALL THIS
// here be dragons
constexpr bool isNumeric(char c)
{
  return c >= '0' && c <= '9';
}

template <typename T>
constexpr T fromChar(char num) // digit to number
{
  //assert(isNumeric(num));
  return T(num - '0');
}

template <typename T>
constexpr unsigned _stoi(const char* string, size_t length, T val = 0)
{
  return length == 0 ?                 (val * 2) + fromChar<T>(string[0])
    : _stoi<T>(string - 1, length - 1, (val * 2) + fromChar<T>(string[0]));
}
// end dragons

constexpr Piece operator "" _pd(const char* definition, size_t size)
{
  //assert(size == 20);
  return Piece(static_cast<PieceShape>(definition[0]),
	       fromChar<PieceRotation>(definition[1]),
	       Piece::PieceDefinition(_stoi<uint16_t>(&definition[4 + 16], 16)),
	       fromChar<uCoord>(definition[2]),
	       fromChar<uCoord>(definition[3]));
}

ostream& operator<< (ostream& os, const Piece& piece)
{
  return os << std::string(piece);
}

PieceDefinitionMap Piece::defs = PieceDefinitionMap
{
  {
    PieceShape::I,
    {
      "I044"
        "0000"
        "1111"
        "0000"
        "0000"_pd,
      "I144"
        "0010"
        "0010"
        "0010"
        "0010"_pd,
      "I244"
        "0000"
        "0000"
        "1111"
        "0000"_pd,
      "I344"
        "0100"
        "0100"
        "0100"
        "0100"_pd,
    }
  },
  {
    PieceShape::O,
    {
      "O022"
        "1100"
        "1100"
        "0000"
        "0000"_pd,
      "O122"
        "1100"
        "1100"
        "0000"
        "0000"_pd,
      "O222"
        "1100"
        "1100"
        "0000"
        "0000"_pd,
      "O322"
        "1100"
        "1100"
        "0000"
        "0000"_pd
    }
  },
  {
    PieceShape::J,
    {
      "J033"
        "1000"
        "1110"
        "0000"
        "0000"_pd,
      "J133"
        "0110"
        "0100"
        "0100"
        "0000"_pd,
      "J233"
        "0000"
        "1110"
        "0010"
        "0000"_pd,
      "J333"
        "0100"
        "0100"
        "1100"
        "0000"_pd
    }
  },
  {
    PieceShape::L,
    {
      "L033"
        "0010"
        "1110"
        "0000"
        "0000"_pd,
      "L133"
        "0100"
        "0100"
        "0110"
        "0000"_pd,
      "L233"
        "0000"
        "1110"
        "1000"
        "0000"_pd,
      "L333"
        "1100"
        "0100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::S,
    {
      "S033"
        "0110"
        "1100"
        "0000"
        "0000"_pd,
      "S133"
        "0100"
        "0110"
        "0010"
        "0000"_pd,
      "S233"
        "0000"
        "0110"
        "1100"
        "0000"_pd,
      "S333"
        "1000"
        "1100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::Z,
    {
      "Z033"
        "0110"
        "1100"
        "0000"
        "0000"_pd,
      "Z133"
        "0100"
        "0110"
        "0010"
        "0000"_pd,
      "Z233"
        "0000"
        "0110"
        "1100"
        "0000"_pd,
      "Z333"
        "1000"
        "1100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::T,
    {
      "T033"
        "0100"
        "1110"
        "0000"
        "0000"_pd,
      "T133"
        "0100"
        "0110"
        "0100"
        "0000"_pd,
      "T233"
        "0000"
        "1110"
        "0100"
        "0000"_pd,
      "T333"
        "0100"
        "1100"
        "0100"
        "0000"_pd
    }
  }
};
