#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <map>
#include <deque>

#include "libtetrinet/Piece.hpp"

class GameSettings
{
public:
  GameSettings(unsigned startHeight, unsigned startLevel, unsigned lineLevel, unsigned levelIncr,
	       unsigned lineSpecial, unsigned specialCount, unsigned specialCap, std::string blockFrequency,
	       std::string specialFrequency, bool showAvgLevel, bool classic, unsigned seed);

  Piece GetPiece();
private:
  /**
    * Mixed congruential generator
    * The same PRNG used on the original game
    *
    * prev: last number generated, or, for n0, the seed
    */
  static uint32_t mcg(uint32_t prev);

  static uint32_t rng(uint32_t s, size_t n);
  static std::map<uint32_t, std::deque<uint32_t> > rngCache;
  
  unsigned startHeight, startLevel, lineLevel, levelIncr, lineSpecial, specialCount, specialCap, seed;
  std::string blockFrequency;
  std::string specialFrequency;
  bool showAvgLevel, classic;

  size_t pieceNum, specialNum;
};

#endif