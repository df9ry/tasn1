#include "tasn1/octetsequence.hpp"
#include "tasn1.h"

namespace tasn1 {

OctetSequence::OctetSequence(const TASN1_OCTET_T *po, TASN1_SIZE_T co): Node()
{
        pCopy = new TASN1_OCTET_T[co];
        memcpy(pCopy, po, co);
        cCopy = co;
        tasn1_init_octet_sequence(&octet_sequence, pCopy, cCopy);
        setNode(&octet_sequence.node);
}

OctetSequence::OctetSequence(const std::string &s) : Node()
{
    size_t sz = s.length() + 1;
    if (sz >= USHRT_MAX)
            sz = USHRT_MAX;
    pCopy = new TASN1_OCTET_T[sz];
    memcpy(pCopy, s.c_str(), sz);
    pCopy[sz-1] = '\0';
    cCopy = sz;
    tasn1_init_octet_sequence(&octet_sequence, pCopy, cCopy);
    setNode(&octet_sequence.node);
}

OctetSequence::~OctetSequence()
{
    delete pCopy;
}

} // end namespace tasn1 //
