// ASN1.HH - ASN.1/DER Parser Definitions for ZealOS
// Part of ZealOS TLS implementation
// Provides ASN.1 DER parsing for X.509 certificates

// ============================================================================
// ASN.1 TAG CLASSES
// ============================================================================

#define ASN1_CLASS_UNIVERSAL        0x00
#define ASN1_CLASS_APPLICATION      0x40
#define ASN1_CLASS_CONTEXT          0x80
#define ASN1_CLASS_PRIVATE          0xC0

#define ASN1_CONSTRUCTED            0x20

// ============================================================================
// ASN.1 UNIVERSAL TAGS
// ============================================================================

#define ASN1_TAG_BOOLEAN            0x01
#define ASN1_TAG_INTEGER            0x02
#define ASN1_TAG_BIT_STRING         0x03
#define ASN1_TAG_OCTET_STRING       0x04
#define ASN1_TAG_NULL               0x05
#define ASN1_TAG_OID                0x06
#define ASN1_TAG_UTF8_STRING        0x0C
#define ASN1_TAG_SEQUENCE           0x10
#define ASN1_TAG_SET                0x11
#define ASN1_TAG_PRINTABLE_STRING   0x13
#define ASN1_TAG_IA5_STRING         0x16
#define ASN1_TAG_UTC_TIME           0x17
#define ASN1_TAG_GENERALIZED_TIME   0x18

// Constructed versions
#define ASN1_TAG_SEQUENCE_OF        (ASN1_TAG_SEQUENCE | ASN1_CONSTRUCTED)
#define ASN1_TAG_SET_OF             (ASN1_TAG_SET | ASN1_CONSTRUCTED)

// Context-specific tags (common in X.509)
#define ASN1_CONTEXT_0              (ASN1_CLASS_CONTEXT | 0x00)
#define ASN1_CONTEXT_1              (ASN1_CLASS_CONTEXT | 0x01)
#define ASN1_CONTEXT_2              (ASN1_CLASS_CONTEXT | 0x02)
#define ASN1_CONTEXT_3              (ASN1_CLASS_CONTEXT | 0x03)
#define ASN1_CONTEXT_0_CONSTRUCTED  (ASN1_CLASS_CONTEXT | ASN1_CONSTRUCTED | 0x00)
#define ASN1_CONTEXT_3_CONSTRUCTED  (ASN1_CLASS_CONTEXT | ASN1_CONSTRUCTED | 0x03)

// ============================================================================
// ASN.1 ERROR CODES
// ============================================================================

#define ASN1_OK                     0
#define ASN1_ERR_INVALID_TAG       -1
#define ASN1_ERR_INVALID_LENGTH    -2
#define ASN1_ERR_TRUNCATED         -3
#define ASN1_ERR_OVERFLOW          -4
#define ASN1_ERR_UNEXPECTED_TAG    -5
#define ASN1_ERR_UNSUPPORTED       -6

// ============================================================================
// ASN.1 ELEMENT STRUCTURE
// ============================================================================

class CASN1Element
{
	U8      tag;            // Tag byte
	I64     length;         // Content length
	U8     *content;        // Pointer to content data
	U8     *raw;            // Pointer to raw element (including tag/length)
	I64     raw_length;     // Total length of raw element
};

// ============================================================================
// ASN.1 PARSER STATE
// ============================================================================

class CASN1Parser
{
	U8     *data;           // Input DER data
	I64     length;         // Total data length
	I64     pos;            // Current position
	I64     error;          // Error code
};

