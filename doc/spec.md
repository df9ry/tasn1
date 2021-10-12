#
<center><h1>ASN.1 like specification for <strong>tasn1</strong> package</h1></center>

    Value             ::= Map | Array | OctetSequence | Number
    Map               ::= Header(0) _ [SEQUENCE](Item)
    Item              ::= Key _ Value
    Key               ::= OctetSequence
    Array             ::= Header(1) _ [SEQUENCE](Value)
    OctetSequence     ::= Header(2) _ [SEQUENCE](OCTET[0..n])
    Number            ::= Header(3) _ [SEQUENCE](OCTET[0..2])
    Header(type)      ::= BITS[1] = (Literal.length > 32) ? LongHeader(t) : ShortHeader(type)
    ShortHeader(type) ::= BITS[1](0) _ BITS[2](type) _ BITS[5](Literal.length)
    LongHeader(type)  ::= BITS[1](1) _ BITS[2](type) _ BITS[5](Length.length) _ OCTET[1..2](Literal.length)

The first octet contains type information and either the number of following content octets (Short form - when bit 7 = 0) or the number of following length octets (Long form - when bit 7 = 1) followed by the length octets (MSB...LSB) Followed by the content data.

<table style="border: 1px solid black">
    <tr>
        <td style="border: 1px solid black" colspan=8><center><strong>Octet 1</strong></center></td>
        <td style="border: 1px solid black" colspan=8><center><strong>Octet 2..n</strong></center></td>
        <td style="border: 1px solid black" colspan=8><center><strong>Octet n+1..m</strong></center></td>
    </tr>
    <tr>
        <td style="border: 1px solid black"><strong>7</strong></td>
        <td style="border: 1px solid black"><strong>6</strong></td>
        <td style="border: 1px solid black"><strong>5</strong></td>
        <td style="border: 1px solid black"><strong>4</strong></td>
        <td style="border: 1px solid black"><strong>3</strong></td>
        <td style="border: 1px solid black"><strong>2</strong></td>
        <td style="border: 1px solid black"><strong>1</strong></td>
        <td style="border: 1px solid black"><strong>0</strong></td>
        <td style="border: 1px solid black"><strong>7</strong></td>
        <td style="border: 1px solid black"><strong>6</strong></td>
        <td style="border: 1px solid black"><strong>5</strong></td>
        <td style="border: 1px solid black"><strong>4</strong></td>
        <td style="border: 1px solid black"><strong>3</strong></td>
        <td style="border: 1px solid black"><strong>2</strong></td>
        <td style="border: 1px solid black"><strong>1</strong></td>
        <td style="border: 1px solid black"><strong>0</strong></td>
        <td style="border: 1px solid black"><strong>7</strong></td>
        <td style="border: 1px solid black"><strong>6</strong></td>
        <td style="border: 1px solid black"><strong>5</strong></td>
        <td style="border: 1px solid black"><strong>4</strong></td>
        <td style="border: 1px solid black"><strong>3</strong></td>
        <td style="border: 1px solid black"><strong>2</strong></td>
        <td style="border: 1px solid black"><strong>1</strong></td>
        <td style="border: 1px solid black"><strong>0</strong></td>
    </tr>
    <tr>
        <td style="border: 1px solid black">'0'</td>
        <td style="border: 1px solid black" colspan=2><center>Type</center></td>
        <td style="border: 1px solid black" colspan=5><center>Content length</center></td>
        <td style="border: 1px solid black" colspan=8><center>Content data</center></td>
    </tr>
    <tr>
        <td style="border: 1px solid black">'1'</td>
        <td style="border: 1px solid black" colspan=2><center>Type</center></td>
        <td style="border: 1px solid black" colspan=5><center>Length length</center></td>
        <td style="border: 1px solid black" colspan=8><center>Length data</center></td>
        <td style="border: 1px solid black" colspan=8><center>Content data</center></td>
    </tr>

</table>
 