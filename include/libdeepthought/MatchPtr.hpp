#ifndef MATCHPTR_HPP
#define MATCHPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class MatchPtr : private Wt::Dbo::ptr<Match>
{
public:
  MatchPtr(AIManager& manager, GenerationPtr& generation, GenomePtr& genomeA, GenomePtr& genomeB);
  MatchPtr(const Wt::Dbo::ptr<Match>& match);

  void SetResult(bool winnerIsGenomeA, int scoreA, int scoreB);
};

#endif
