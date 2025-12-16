// X509.HH - X.509 Certificate Definitions for ZealOS
// Part of ZealOS TLS implementation
// Provides X.509 certificate parsing and validation

// ============================================================================
// X.509 ERROR CODES
// ============================================================================

#define X509_OK                         0
#define X509_ERR_PARSE_FAILED          -1
#define X509_ERR_INVALID_SIGNATURE     -2
#define X509_ERR_EXPIRED               -3
#define X509_ERR_NOT_YET_VALID         -4
#define X509_ERR_HOSTNAME_MISMATCH     -5
#define X509_ERR_UNTRUSTED             -6
#define X509_ERR_SELF_SIGNED           -7
#define X509_ERR_CHAIN_TOO_LONG        -8
#define X509_ERR_UNSUPPORTED_ALGORITHM -9

// ============================================================================
// X.509 LIMITS
// ============================================================================

#define X509_MAX_CHAIN_LENGTH          10
#define X509_MAX_SAN_COUNT             32
#define X509_MAX_NAME_LEN             256

// ============================================================================
// X.509 NAME (DN Component)
// ============================================================================

class CX509Name
{
	U8     *common_name;          // CN
	U8     *organization;         // O
	U8     *organizational_unit;  // OU
	U8     *country;              // C
	U8     *state;                // ST
	U8     *locality;             // L
};

// ============================================================================
// X.509 CERTIFICATE
// ============================================================================

class CX509Certificate
{
	// Certificate version (1, 2, or 3)
	I64     version;

	// Serial number (raw bytes)
	U8     *serial_number;
	I64     serial_number_len;

	// Signature algorithm OID
	U8     *signature_algorithm;

	// Issuer and Subject names
	CX509Name  issuer;
	CX509Name  subject;

	// Validity period (Unix timestamps)
	U64     not_before;
	U64     not_after;

	// Public key info
	U8     *public_key_algorithm;  // OID string
	U8     *public_key;            // Raw public key bytes
	I64     public_key_len;

	// Subject Alternative Names (SANs)
	U8    **san_dns;               // DNS names
	I64     san_dns_count;
	U8    **san_ip;                // IP addresses
	I64     san_ip_count;

	// Signature
	U8     *signature;
	I64     signature_len;

	// TBS (To Be Signed) certificate data
	U8     *tbs_data;
	I64     tbs_len;

	// Raw DER data
	U8     *raw_der;
	I64     raw_der_len;

	// Flags
	Bool    is_ca;                 // Certificate Authority
	I64     path_len_constraint;   // Max chain length (-1 = no constraint)
};

// ============================================================================
// X.509 TRUST STORE
// ============================================================================

class CX509TrustStore
{
	CX509Certificate **certificates;
	I64                 count;
	I64                 capacity;
};

// ============================================================================
// COMMON OIDs (raw bytes)
// ============================================================================

// Signature algorithms
// sha256WithRSAEncryption: 1.2.840.113549.1.1.11
extern U8 OID_SHA256_RSA[9];
// sha384WithRSAEncryption: 1.2.840.113549.1.1.12
extern U8 OID_SHA384_RSA[9];
// sha512WithRSAEncryption: 1.2.840.113549.1.1.13
extern U8 OID_SHA512_RSA[9];
// ecdsa-with-SHA256: 1.2.840.10045.4.3.2
extern U8 OID_ECDSA_SHA256[8];
// ecdsa-with-SHA384: 1.2.840.10045.4.3.3
extern U8 OID_ECDSA_SHA384[8];

// Name attributes
// commonName: 2.5.4.3
extern U8 OID_COMMON_NAME[3];
// organizationName: 2.5.4.10
extern U8 OID_ORGANIZATION[3];
// organizationalUnitName: 2.5.4.11
extern U8 OID_ORG_UNIT[3];
// countryName: 2.5.4.6
extern U8 OID_COUNTRY[3];
// stateOrProvinceName: 2.5.4.8
extern U8 OID_STATE[3];
// localityName: 2.5.4.7
extern U8 OID_LOCALITY[3];

// Extensions
// subjectAltName: 2.5.29.17
extern U8 OID_SUBJECT_ALT_NAME[3];
// basicConstraints: 2.5.29.19
extern U8 OID_BASIC_CONSTRAINTS[3];
// keyUsage: 2.5.29.15
extern U8 OID_KEY_USAGE[3];

