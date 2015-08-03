#include "libtetrinet/FieldTransform.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>

using namespace std;
using namespace boost;

FieldTransform::FieldTransform(const Field& field)
  : field(field), fieldWidth(field.GetWidth()), fieldHeight(field.GetHeight()),
    fieldSize(fieldWidth * fieldHeight), transforms(new TransformType())
{
}


FieldTransform::FieldTransform(const Field& field, const Piece& piece, sCoord x, sCoord y, FieldElement element)
  : FieldTransform(field)
{
  for (sCoord i = 0; i < numeric_cast<sCoord>(piece.GetWidth() * piece.GetHeight()); ++i)
  {
    uCoord x2 = numeric_cast<uCoord>(i % piece.GetWidth()),
           y2 = numeric_cast<uCoord>(i / piece.GetWidth());
    if (piece(x2, y2))
      (*this)(x + x2, y + y2) = element;
  }
}

FieldTransform::FieldTransform(const FieldTransform& transform)
  : field(transform.field),
    fieldWidth(transform.fieldWidth),
    fieldHeight(transform.fieldHeight),
    fieldSize(transform.fieldSize),
    transforms(new TransformType(*transform.transforms))
{
}

ostream& operator << (ostream& os, const FieldTransform& fieldTransform)
{
  for (pair<const uCoord, FieldElement>& element : *fieldTransform.transforms)
    os << (uCoord)element.first << " = " << element.second << endl;
  return os;
}

FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source)
{
  for (const auto& element : source)
    (*destination.transforms)[element.first] = element.second;
  return destination;
}

FieldElement& FieldTransform::operator() (uCoord x, uCoord y)
{ 
  return const_cast<FieldElement&>(
    static_cast<const FieldTransform&>(*this)(x, y));
}


FieldElement& FieldTransform::operator() (uCoord i)
{
  return const_cast<FieldElement&>(
    static_cast<const FieldTransform&>(*this)(i));
}

const FieldElement& FieldTransform::operator() (uCoord x, uCoord y) const
{
  if (!(x < fieldWidth))
    throw out_of_range("x = " + lexical_cast<string>(x));
  if (!(y < fieldHeight))
    throw out_of_range("y = " + lexical_cast<string>(y));
  return (*this)((y * fieldWidth) + x);
}

// TODO these violate const, fix
const FieldElement& FieldTransform::operator() (uCoord i) const
{
  if (!(i < fieldSize))
    throw out_of_range("i = " + lexical_cast<string>(i));
  return transforms->count(i) ? transforms->at(i) : (transforms->emplace(i, FieldElement::NONE)).first->second;
}

const FieldTransform::TransformType::const_iterator FieldTransform::begin() const
{
  return transforms->begin();
}

const FieldTransform::TransformType::const_iterator FieldTransform::end() const
{
  return transforms->end();
}

bool FieldTransform::ContainsTransform(const FieldTransform& transform) const
{
  return all_of(transform.begin(), transform.end(), [this](pair<uCoord, FieldElement> element)
		{
		  return (*transforms)[element.first] != FieldElement::NONE;
		});
}