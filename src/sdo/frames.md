 SDO Block download initiate request
 +---------------+---------------+---------------+---------------+---
 |       0       |       1       |       2       |       3       | 4..7
 | ccs | | |R|s|C|            Index              | Subindex      | Size
 |1|1|0|x|x| | |0|                               |               |
 +---------------+---------------+---------------+---------------+---

 SDO Block upload initiate response
 +---------------+---------------+---------------+---------------+---
 |       0       |       1       |       2       |       3       | 4..7
 | scs | | |R|s|S|            Index              | Subindex      | Size
 |1|1|0|x|x| | |0|                               |               |
 +---------------+---------------+---------------+---------------+---

On a R et s à donner.

- CCS/SCS
- CRCMixin
- SizeSpecifiedBit1Mixin
- IndexSubindexMixin
- setSize(size), size()

 SDO Block download initiate response
 +---------------+---------------+---------------+---------------+
 |       0       |       1       |       2       |       3       |
 | scs | | |R| S |            Index              | Blksize       |
 |1|0|1|x|x| |0|0|                               |               |
 +---------------+---------------+---------------+---------------+

- CRCMixin
- IndexSubindexMixin
- setBlockSize(size), blockSize()


 SDO Block upload/download subblock request
 +---------------+-------------...
 |       0       |       1..7
 |c| seqno       | Data...
 | | | | | | | | |
 +---------------+-------------...

- FinalTransferMixinBit7
- seqno(), setSeqNo(seqno)
- setData(*data, size)
- data(*data)

 SDO Block download subblock response
 +---------------+---------------+---------------+
 |       0       |       1       |       2       |
 | scs | | | | S | Ackseq        | Blksize       |
 |1|0|1|x|x|x|1|0|               |               |
 +---------------+---------------+---------------+

 SDO Block upload subblock response
 +---------------+---------------+---------------+
 |       0       |       1       |       2       |
 | ccs | | | | C | Ackseq        | Blksize       |
 |1|0|1|x|x|x|1|0|               |               |
 +---------------+---------------+---------------+

- setAcqSeq(seqno), ackSeq()
- setBlockSize(size), blockSize()

 SDO Block download end request
 +---------------+---------------+---------------+
 |       0       |       1       |       2       |
 | ccs |  n  | |C|              CRC              |
 |1|1|0| | | |x|0|                               |
 +---------------+---------------+---------------+

 SDO Block upload end request
 +---------------+---------------+---------------+
 |       0       |       1       |       2       |
 | scs |  n  | |S|              CRC              |
 |1|1|0| | | |x|1|                               |
 +---------------+---------------+---------------+

- Size3Mixin
- setCRC(crc), crc()

 SDO Block download end response
 +---------------+
 |       0       |
 | scs |     | S |
 |1|0|1|x|x|x|0|1|
 +---------------+

 SDO Block download end response
 +---------------+
 |       0       |
 | ccs |     | C |
 |1|0|1|x|x|x|1|1|
 +---------------+

 SDO Block upload initiate request
 +---------------+---------------+---------------+---------------+---------------+---------------+
 |       0       |       1       |       2       |       3       |      4        |      5        |
 | ccs | | |R| C |            Index              | Subindex      | Blksize       | Pst           |
 |1|1|0|x|x| |0|0|                               |               |               |               |
 +---------------+---------------+---------------+---------------+---------------+---------------+

-

R (cc/sc): 0: not CRC support, 1: CRC support
s: 0: size not specified, 1: size specified
S (ss): 0: initiate download response
        1: end block download response
        2: block download response
C (cs): 0: initiate download request
        1: end block download request
n: number in the last segment that don't contain data
c: 0: more segments to come, 1: last segment
pst: 0: No change of transfer protocol, pst > 0: switch to SDO upload, if data less than.
