#include "tasn1/octetsequence.hpp"
#include "tasn1/tasn1.h"

namespace tasn1 {

OctetSequence::OctetSequence(const uint8_t *po, uint16_t co):
    Node(::tasn1_new_octet_sequence(po, co, true))
{
}

OctetSequence::OctetSequence(const std::string &s):
    Node(::tasn1_new_string(s.c_str(), true))
{
}

} // end namespace tasn1 //
