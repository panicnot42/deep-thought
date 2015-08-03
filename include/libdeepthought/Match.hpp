#ifndef MATCH_HPP
#define MATCH_HPP

#include <string>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>

#include "Genome.hpp"

class Match
{
public:
  typedef boost::signals2::signal<void (const Match&)> CompleteSignal;
  
  Match(Genome* a, Genome* b);
  virtual ~Match();

  const boost::signals2::connection& AddOnComplete(const CompleteSignal::slot_type& slot) const;
private:
  mutable CompleteSignal onComplete;
};

#endif