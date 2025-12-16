// Crypto.HH - Common cryptographic definitions for ZealOS TLS implementation
// See C:/Home/Net/Docs/TLSDesign.DD for full design documentation

// ============================================================================
// HASH FUNCTION CONSTANTS
// ============================================================================

#define HASH_SHA256     0
#define HASH_SHA384     1
#define HASH_SHA512     2

#define SHA256_BLOCK_SIZE    64
#define SHA256_DIGEST_SIZE   32

#define SHA384_BLOCK_SIZE   128
#define SHA384_DIGEST_SIZE   48

#define SHA512_BLOCK_SIZE   128
#define SHA512_DIGEST_SIZE   64

// ============================================================================
// AES CONSTANTS
// ============================================================================

#define AES_BLOCK_SIZE   16
#define AES_128_KEY_SIZE 16
#define AES_256_KEY_SIZE 32
#define AES_128_ROUNDS   10
#define AES_256_ROUNDS   14

// ============================================================================
// GCM CONSTANTS
// ============================================================================

#define GCM_NONCE_SIZE   12
#define GCM_TAG_SIZE     16

// ============================================================================
// X25519 CONSTANTS
// ============================================================================

#define X25519_KEY_SIZE      32
#define X25519_SHARED_SIZE   32

// ============================================================================
// HMAC CONSTANTS
// ============================================================================

#define HMAC_SHA256_SIZE  32
#define HMAC_SHA384_SIZE  48
#define HMAC_SHA512_SIZE  64

// ============================================================================
// SHA-256 CONTEXT
// ============================================================================

class CSHA256Context
{
	U32  state[8];      // Hash state (A-H)
	U64  count;         // Number of bits processed
	U8   buffer[64];    // Input buffer
	I64  buffer_len;    // Current buffer length
};

// ============================================================================
// SHA-512/384 CONTEXT
// ============================================================================

class CSHA512Context
{
	U64  state[8];      // Hash state
	U64  count_lo;      // Number of bits processed (low)
	U64  count_hi;      // Number of bits processed (high)
	U8   buffer[128];   // Input buffer
	I64  buffer_len;    // Current buffer length
};

// ============================================================================
// HMAC CONTEXT
// ============================================================================

class CHMACContext
{
	I64             hash_type;      // HASH_SHA256, HASH_SHA384, etc.
	U8              ipad_key[128];  // Inner padded key
	U8              opad_key[128];  // Outer padded key
	CSHA256Context  sha256;         // For SHA-256
	CSHA512Context  sha512;         // For SHA-384/512
};

// ============================================================================
// AES CONTEXT
// ============================================================================

class CAESContext
{
	U32  enc_round_keys[60];  // Expanded encryption key schedule
	U32  dec_round_keys[60];  // Expanded decryption key schedule
	I64  rounds;              // 10 for AES-128, 14 for AES-256
	I64  key_size;            // Key size in bytes
};

// ============================================================================
// GCM CONTEXT
// ============================================================================

class CGCMContext
{
	CAESContext  aes;        // AES cipher context
	U8           H[16];      // Hash subkey (AES(0))
	U8           J0[16];     // Pre-counter block
};

// ============================================================================
// X25519/FIELD ELEMENT
// ============================================================================

// Field element for Curve25519: 256-bit number in limb form
// Using 5 limbs of 51 bits each (plus carry bits)
class CFieldElement
{
	I64  limb[5];
};

// ============================================================================
// RANDOM CONTEXT
// ============================================================================

class CRandomContext
{
	U8   pool[64];       // Entropy pool
	U64  counter;        // Reseed counter
	Bool initialized;    // Has been seeded
};

// ============================================================================
// ERROR CODES
// ============================================================================

#define CRYPTO_OK                    0
#define CRYPTO_ERR_INVALID_PARAM    -1
#define CRYPTO_ERR_BUFFER_TOO_SMALL -2
#define CRYPTO_ERR_AUTH_FAILED      -3
#define CRYPTO_ERR_NOT_INITIALIZED  -4

// ============================================================================
// UTILITY MACROS
// ============================================================================

// Rotate right (32-bit)
#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

// Rotate right (64-bit)
#define ROTR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))

// Big-endian load/store for 32-bit
#define BE32_LOAD(p) \
	(((U32)(p)[0] << 24) | ((U32)(p)[1] << 16) | \
	 ((U32)(p)[2] <<  8) | ((U32)(p)[3]))

#define BE32_STORE(p, v) do { \
	(p)[0] = (U8)((v) >> 24); \
	(p)[1] = (U8)((v) >> 16); \
	(p)[2] = (U8)((v) >>  8); \
	(p)[3] = (U8)(v); \
} while (0)

// Big-endian load/store for 64-bit
#define BE64_LOAD(p) \
	(((U64)(p)[0] << 56) | ((U64)(p)[1] << 48) | \
	 ((U64)(p)[2] << 40) | ((U64)(p)[3] << 32) | \
	 ((U64)(p)[4] << 24) | ((U64)(p)[5] << 16) | \
	 ((U64)(p)[6] <<  8) | ((U64)(p)[7]))

#define BE64_STORE(p, v) do { \
	(p)[0] = (U8)((v) >> 56); \
	(p)[1] = (U8)((v) >> 48); \
	(p)[2] = (U8)((v) >> 40); \
	(p)[3] = (U8)((v) >> 32); \
	(p)[4] = (U8)((v) >> 24); \
	(p)[5] = (U8)((v) >> 16); \
	(p)[6] = (U8)((v) >>  8); \
	(p)[7] = (U8)(v); \
} while (0)

// Little-endian load/store for 64-bit
#define LE64_LOAD(p) \
	(((U64)(p)[7] << 56) | ((U64)(p)[6] << 48) | \
	 ((U64)(p)[5] << 40) | ((U64)(p)[4] << 32) | \
	 ((U64)(p)[3] << 24) | ((U64)(p)[2] << 16) | \
	 ((U64)(p)[1] <<  8) | ((U64)(p)[0]))

#define LE64_STORE(p, v) do { \
	(p)[0] = (U8)(v); \
	(p)[1] = (U8)((v) >>  8); \
	(p)[2] = (U8)((v) >> 16); \
	(p)[3] = (U8)((v) >> 24); \
	(p)[4] = (U8)((v) >> 32); \
	(p)[5] = (U8)((v) >> 40); \
	(p)[6] = (U8)((v) >> 48); \
	(p)[7] = (U8)((v) >> 56); \
} while (0)

// Constant-time comparison (returns 0 if equal)
// This is important for preventing timing attacks
I64 CryptoConstantTimeCompare(U8 *a, U8 *b, I64 length)
{
	I64 result = 0;
	I64 i;
	for (i = 0; i < length; i++)
		result |= a[i] ^ b[i];
	return result;
}

// Secure memory zeroing (not optimized away by compiler)
U0 CryptoSecureZero(U8 *ptr, I64 length)
{
	volatile U8 *p = ptr;
	I64 i;
	for (i = 0; i < length; i++)
		p[i] = 0;
}
