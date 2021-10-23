#include "tasn1/octetsequence.hpp"
#include "tasn1.h"

namespace tasn1 {

OctetSequence::OctetSequence(const uint8_t *po, uint16_t co):
    Node(&octet_sequence.node)
{
        tasn1_init_octet_sequence(&octet_sequence, po, co);
}

OctetSequence::OctetSequence(const std::string &s):
    Node(&octet_sequence.node)
{
        tasn1_init_string(&octet_sequence, s.c_str());
}

} // end namespace tasn1 //
