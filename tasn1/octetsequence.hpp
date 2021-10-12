#ifndef TASN1_OCTETSEQUENCE_HPP
#define TASN1_OCTETSEQUENCE_HPP

#include "node.hpp"

#include <string>

namespace tasn1 {

class OctetSequence: public Node
{
public:
    OctetSequence(const uint8_t *po, uint16_t co);
    OctetSequence(const std::string &s);
};

} // end namespace tasn1 //

#endif // TASN1_OCTETSEQUENCE_HPP
