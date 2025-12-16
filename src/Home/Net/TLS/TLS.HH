// TLS.HH - TLS Protocol definitions for ZealOS
// See C:/Home/Net/Docs/TLSDesign.DD for full design documentation

// ============================================================================
// TLS VERSIONS
// ============================================================================

#define TLS_VERSION_1_0     0x0301
#define TLS_VERSION_1_1     0x0302
#define TLS_VERSION_1_2     0x0303
#define TLS_VERSION_1_3     0x0304

// Legacy version sent in record layer (TLS 1.3 uses 0x0303 for compatibility)
#define TLS_LEGACY_VERSION  0x0303

// ============================================================================
// CONTENT TYPES
// ============================================================================

#define TLS_CONTENT_CHANGE_CIPHER   20
#define TLS_CONTENT_ALERT           21
#define TLS_CONTENT_HANDSHAKE       22
#define TLS_CONTENT_APPLICATION     23

// TLS 1.3 inner content type (for encrypted records)
#define TLS_CONTENT_INNER_APP       23

// ============================================================================
// HANDSHAKE MESSAGE TYPES
// ============================================================================

#define TLS_HS_CLIENT_HELLO             1
#define TLS_HS_SERVER_HELLO             2
#define TLS_HS_NEW_SESSION_TICKET       4
#define TLS_HS_END_OF_EARLY_DATA        5
#define TLS_HS_ENCRYPTED_EXTENSIONS     8
#define TLS_HS_CERTIFICATE             11
#define TLS_HS_CERTIFICATE_REQUEST     13
#define TLS_HS_CERTIFICATE_VERIFY      15
#define TLS_HS_FINISHED                20
#define TLS_HS_KEY_UPDATE              24
#define TLS_HS_MESSAGE_HASH           254

// ============================================================================
// ALERT LEVELS
// ============================================================================

#define TLS_ALERT_WARNING   1
#define TLS_ALERT_FATAL     2

// ============================================================================
// ALERT DESCRIPTIONS
// ============================================================================

#define TLS_ALERT_CLOSE_NOTIFY                   0
#define TLS_ALERT_UNEXPECTED_MESSAGE            10
#define TLS_ALERT_BAD_RECORD_MAC                20
#define TLS_ALERT_RECORD_OVERFLOW               22
#define TLS_ALERT_HANDSHAKE_FAILURE             40
#define TLS_ALERT_BAD_CERTIFICATE               42
#define TLS_ALERT_UNSUPPORTED_CERTIFICATE       43
#define TLS_ALERT_CERTIFICATE_REVOKED           44
#define TLS_ALERT_CERTIFICATE_EXPIRED           45
#define TLS_ALERT_CERTIFICATE_UNKNOWN           46
#define TLS_ALERT_ILLEGAL_PARAMETER             47
#define TLS_ALERT_UNKNOWN_CA                    48
#define TLS_ALERT_ACCESS_DENIED                 49
#define TLS_ALERT_DECODE_ERROR                  50
#define TLS_ALERT_DECRYPT_ERROR                 51
#define TLS_ALERT_PROTOCOL_VERSION              70
#define TLS_ALERT_INSUFFICIENT_SECURITY         71
#define TLS_ALERT_INTERNAL_ERROR                80
#define TLS_ALERT_INAPPROPRIATE_FALLBACK        86
#define TLS_ALERT_USER_CANCELED                 90
#define TLS_ALERT_MISSING_EXTENSION            109
#define TLS_ALERT_UNSUPPORTED_EXTENSION        110
#define TLS_ALERT_UNRECOGNIZED_NAME            112
#define TLS_ALERT_BAD_CERTIFICATE_STATUS_RESP  113
#define TLS_ALERT_UNKNOWN_PSK_IDENTITY         115
#define TLS_ALERT_CERTIFICATE_REQUIRED         116
#define TLS_ALERT_NO_APPLICATION_PROTOCOL      120

// ============================================================================
// CIPHER SUITES (TLS 1.3)
// ============================================================================

#define TLS_AES_128_GCM_SHA256          0x1301
#define TLS_AES_256_GCM_SHA384          0x1302
#define TLS_CHACHA20_POLY1305_SHA256    0x1303
#define TLS_AES_128_CCM_SHA256          0x1304
#define TLS_AES_128_CCM_8_SHA256        0x1305

// ============================================================================
// NAMED GROUPS (for key exchange)
// ============================================================================

// Elliptic Curves
#define TLS_GROUP_SECP256R1     0x0017  // P-256
#define TLS_GROUP_SECP384R1     0x0018  // P-384
#define TLS_GROUP_SECP521R1     0x0019  // P-521
#define TLS_GROUP_X25519        0x001D  // Curve25519
#define TLS_GROUP_X448          0x001E  // Curve448

// Finite Field Groups
#define TLS_GROUP_FFDHE2048     0x0100
#define TLS_GROUP_FFDHE3072     0x0101
#define TLS_GROUP_FFDHE4096     0x0102

// ============================================================================
// SIGNATURE ALGORITHMS
// ============================================================================

// RSASSA-PKCS1-v1_5
#define TLS_SIG_RSA_PKCS1_SHA256        0x0401
#define TLS_SIG_RSA_PKCS1_SHA384        0x0501
#define TLS_SIG_RSA_PKCS1_SHA512        0x0601

// ECDSA
#define TLS_SIG_ECDSA_SECP256R1_SHA256  0x0403
#define TLS_SIG_ECDSA_SECP384R1_SHA384  0x0503
#define TLS_SIG_ECDSA_SECP521R1_SHA512  0x0603

// RSASSA-PSS with SHA-256/384/512
#define TLS_SIG_RSA_PSS_RSAE_SHA256     0x0804
#define TLS_SIG_RSA_PSS_RSAE_SHA384     0x0805
#define TLS_SIG_RSA_PSS_RSAE_SHA512     0x0806

// Ed25519/Ed448
#define TLS_SIG_ED25519                 0x0807
#define TLS_SIG_ED448                   0x0808

// ============================================================================
// EXTENSIONS
// ============================================================================

#define TLS_EXT_SERVER_NAME                 0   // SNI
#define TLS_EXT_MAX_FRAGMENT_LENGTH         1
#define TLS_EXT_STATUS_REQUEST              5   // OCSP stapling
#define TLS_EXT_SUPPORTED_GROUPS           10   // Named groups
#define TLS_EXT_EC_POINT_FORMATS           11
#define TLS_EXT_SIGNATURE_ALGORITHMS       13
#define TLS_EXT_USE_SRTP                   14
#define TLS_EXT_HEARTBEAT                  15
#define TLS_EXT_ALPN                       16   // Application-Layer Protocol Negotiation
#define TLS_EXT_SIGNED_CERT_TIMESTAMP      18
#define TLS_EXT_CLIENT_CERT_TYPE           19
#define TLS_EXT_SERVER_CERT_TYPE           20
#define TLS_EXT_PADDING                    21
#define TLS_EXT_ENCRYPT_THEN_MAC           22
#define TLS_EXT_EXTENDED_MASTER_SECRET     23
#define TLS_EXT_SESSION_TICKET             35
#define TLS_EXT_PRE_SHARED_KEY             41
#define TLS_EXT_EARLY_DATA                 42
#define TLS_EXT_SUPPORTED_VERSIONS         43
#define TLS_EXT_COOKIE                     44
#define TLS_EXT_PSK_KEY_EXCHANGE_MODES     45
#define TLS_EXT_CERTIFICATE_AUTHORITIES    47
#define TLS_EXT_OID_FILTERS                48
#define TLS_EXT_POST_HANDSHAKE_AUTH        49
#define TLS_EXT_SIGNATURE_ALGORITHMS_CERT  50
#define TLS_EXT_KEY_SHARE                  51

// ============================================================================
// SIZE LIMITS
// ============================================================================

#define TLS_MAX_RECORD_SIZE         16384   // 2^14 bytes
#define TLS_MAX_HANDSHAKE_SIZE      65536   // 2^16 bytes
#define TLS_MAX_CERTIFICATE_SIZE    65536
#define TLS_RECORD_HEADER_SIZE          5   // ContentType(1) + Version(2) + Length(2)
#define TLS_HANDSHAKE_HEADER_SIZE       4   // Type(1) + Length(3)

// Encrypted record overhead: tag (16) + content type (1)
#define TLS_AEAD_OVERHEAD              17

// ============================================================================
// TLS STATE MACHINE
// ============================================================================

#define TLS_STATE_INIT                  0
#define TLS_STATE_CLIENT_HELLO_SENT     1
#define TLS_STATE_SERVER_HELLO_RECV     2
#define TLS_STATE_HANDSHAKE_KEYS        3
#define TLS_STATE_ENCRYPTED_EXTS        4
#define TLS_STATE_CERTIFICATE           5
#define TLS_STATE_CERT_VERIFY           6
#define TLS_STATE_FINISHED_RECV         7
#define TLS_STATE_FINISHED_SENT         8
#define TLS_STATE_CONNECTED             9
#define TLS_STATE_CLOSING              10
#define TLS_STATE_CLOSED               11
#define TLS_STATE_ERROR                12

// ============================================================================
// TLS ERROR CODES
// ============================================================================

#define TLS_OK                           0
#define TLS_ERR_HANDSHAKE_FAILED        -1
#define TLS_ERR_INVALID_RECORD          -2
#define TLS_ERR_INVALID_VERSION         -3
#define TLS_ERR_INVALID_MESSAGE         -4
#define TLS_ERR_CERTIFICATE_ERROR       -5
#define TLS_ERR_DECRYPT_FAILED          -6
#define TLS_ERR_AUTH_FAILED             -7
#define TLS_ERR_UNEXPECTED_MESSAGE      -8
#define TLS_ERR_BUFFER_OVERFLOW         -9
#define TLS_ERR_CONNECTION_CLOSED      -10
#define TLS_ERR_ALERT_RECEIVED         -11
#define TLS_ERR_UNSUPPORTED_CIPHER     -12
#define TLS_ERR_UNSUPPORTED_VERSION    -13
#define TLS_ERR_NO_COMMON_CIPHER       -14
#define TLS_ERR_INTERNAL               -15
#define TLS_ERR_TIMEOUT                -16

// ============================================================================
// TLS RECORD HEADER
// ============================================================================

class CTLSRecordHeader
{
	U8   content_type;
	U8   version_major;     // version >> 8
	U8   version_minor;     // version & 0xFF
	U8   length_high;       // length >> 8
	U8   length_low;        // length & 0xFF
};

// ============================================================================
// TLS HANDSHAKE HEADER
// ============================================================================

class CTLSHandshakeHeader
{
	U8   msg_type;
	U8   length_high;       // 24-bit length
	U8   length_mid;
	U8   length_low;
};

// ============================================================================
// TLS CONNECTION STATE
// ============================================================================

class CTLSState
{
	// Connection info
	U8              state;              // TLS_STATE_* enum
	U16             version;            // Negotiated TLS version
	U16             cipher_suite;       // Negotiated cipher suite
	I64             hash_size;          // Hash size for current cipher (32 or 48)

	// Traffic encryption keys
	U8              client_write_key[32];
	U8              server_write_key[32];
	U8              client_write_iv[12];
	U8              server_write_iv[12];

	// Sequence numbers (for nonce construction in TLS 1.3)
	U64             client_seq;
	U64             server_seq;

	// Crypto contexts
	CGCMContext     client_gcm;
	CGCMContext     server_gcm;

	// Handshake transcript hashes
	CSHA256Context  transcript_sha256;
	CSHA512Context  transcript_sha384;
	Bool            use_sha384;         // TRUE if using SHA-384 cipher suite

	// TLS 1.3 Key Schedule secrets
	U8              early_secret[48];
	U8              handshake_secret[48];
	U8              master_secret[48];
	U8              client_handshake_traffic_secret[48];
	U8              server_handshake_traffic_secret[48];
	U8              client_application_traffic_secret[48];
	U8              server_application_traffic_secret[48];

	// X25519 key pair for ECDHE key exchange
	U8              ecdh_private[32];
	U8              ecdh_public[32];

	// Server's public key (received in key_share extension)
	U8              server_public[32];

	// Random values from ClientHello and ServerHello
	U8              client_random[32];
	U8              server_random[32];

	// Receive buffer for partial records
	U8             *recv_buffer;
	I64             recv_buffer_size;
	I64             recv_buffer_len;

	// Decrypted application data buffer
	U8             *app_buffer;
	I64             app_buffer_size;
	I64             app_buffer_len;
	I64             app_buffer_pos;     // Read position

	// Server name (for SNI extension)
	U8             *server_name;

	// Alert info
	U8              last_alert_level;
	U8              last_alert_desc;
};

// ============================================================================
// TLS SOCKET
// ============================================================================

class CTLSSocket
{
	CTCPSocket     *tcp_socket;     // Underlying TCP connection
	CTLSState       tls;            // TLS protocol state
	I64             timeout;        // Socket timeout (ms)
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Convert TLS error code to string
U8 *TLSErrorString(I64 error)
{
	switch (error)
	{
		case TLS_OK:                     return "Success";
		case TLS_ERR_HANDSHAKE_FAILED:   return "Handshake failed";
		case TLS_ERR_INVALID_RECORD:     return "Invalid record";
		case TLS_ERR_INVALID_VERSION:    return "Invalid TLS version";
		case TLS_ERR_INVALID_MESSAGE:    return "Invalid message";
		case TLS_ERR_CERTIFICATE_ERROR:  return "Certificate error";
		case TLS_ERR_DECRYPT_FAILED:     return "Decryption failed";
		case TLS_ERR_AUTH_FAILED:        return "Authentication failed";
		case TLS_ERR_UNEXPECTED_MESSAGE: return "Unexpected message";
		case TLS_ERR_BUFFER_OVERFLOW:    return "Buffer overflow";
		case TLS_ERR_CONNECTION_CLOSED:  return "Connection closed";
		case TLS_ERR_ALERT_RECEIVED:     return "Alert received";
		case TLS_ERR_UNSUPPORTED_CIPHER: return "Unsupported cipher";
		case TLS_ERR_UNSUPPORTED_VERSION:return "Unsupported version";
		case TLS_ERR_NO_COMMON_CIPHER:   return "No common cipher";
		case TLS_ERR_INTERNAL:           return "Internal error";
		case TLS_ERR_TIMEOUT:            return "Timeout";
		default:                         return "Unknown error";
	}
}

// Convert alert description to string
U8 *TLSAlertString(U8 alert_desc)
{
	switch (alert_desc)
	{
		case TLS_ALERT_CLOSE_NOTIFY:            return "close_notify";
		case TLS_ALERT_UNEXPECTED_MESSAGE:      return "unexpected_message";
		case TLS_ALERT_BAD_RECORD_MAC:          return "bad_record_mac";
		case TLS_ALERT_HANDSHAKE_FAILURE:       return "handshake_failure";
		case TLS_ALERT_BAD_CERTIFICATE:         return "bad_certificate";
		case TLS_ALERT_CERTIFICATE_UNKNOWN:     return "certificate_unknown";
		case TLS_ALERT_ILLEGAL_PARAMETER:       return "illegal_parameter";
		case TLS_ALERT_UNKNOWN_CA:              return "unknown_ca";
		case TLS_ALERT_DECODE_ERROR:            return "decode_error";
		case TLS_ALERT_DECRYPT_ERROR:           return "decrypt_error";
		case TLS_ALERT_PROTOCOL_VERSION:        return "protocol_version";
		case TLS_ALERT_INTERNAL_ERROR:          return "internal_error";
		case TLS_ALERT_MISSING_EXTENSION:       return "missing_extension";
		case TLS_ALERT_UNSUPPORTED_EXTENSION:   return "unsupported_extension";
		default:                                return "unknown_alert";
	}
}

// Get hash size for cipher suite
I64 TLSCipherHashSize(U16 cipher_suite)
{
	switch (cipher_suite)
	{
		case TLS_AES_128_GCM_SHA256:
		case TLS_CHACHA20_POLY1305_SHA256:
			return 32;  // SHA-256
		case TLS_AES_256_GCM_SHA384:
			return 48;  // SHA-384
		default:
			return 32;  // Default to SHA-256
	}
}

// Get key size for cipher suite
I64 TLSCipherKeySize(U16 cipher_suite)
{
	switch (cipher_suite)
	{
		case TLS_AES_128_GCM_SHA256:
			return 16;  // AES-128
		case TLS_AES_256_GCM_SHA384:
			return 32;  // AES-256
		case TLS_CHACHA20_POLY1305_SHA256:
			return 32;  // ChaCha20
		default:
			return 16;
	}
}
