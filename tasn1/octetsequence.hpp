#ifndef TASN1_OCTETSEQUENCE_HPP
#define TASN1_OCTETSEQUENCE_HPP

#include "node.hpp"
#include "tasn1/types.h"

#include <string>

namespace tasn1 {

class OctetSequence: public Node
{
public:
    OctetSequence(const TASN1_OCTET_T *po, TASN1_SIZE_T co);
    OctetSequence(const std::string &s);
    ~OctetSequence();

private:
    octet_sequence_t octet_sequence;
    TASN1_OCTET_T *pCopy;
    TASN1_SIZE_T   cCopy;
};

} // end namespace tasn1 //

#endif // TASN1_OCTETSEQUENCE_HPP
