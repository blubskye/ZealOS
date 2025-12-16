// RateLimit.HH - Discord Rate Limiting Definitions for ZealOS
// Part of ZealOS Discord implementation
// Implements per-route rate limit tracking with automatic retry

// ============================================================================
// RATE LIMIT CONSTANTS
// ============================================================================

#define RATELIMIT_MAX_BUCKETS      64      // Maximum tracked buckets
#define RATELIMIT_BUCKET_HASH_LEN  64      // Max length of bucket hash
#define RATELIMIT_MAX_ROUTE_LEN    128     // Max route length
#define RATELIMIT_MAX_RETRIES      3       // Max retry attempts on 429
#define RATELIMIT_GLOBAL_WAIT_MS   1000    // Default global wait (1 second)

// ============================================================================
// RATE LIMIT BUCKET
// ============================================================================

// Per-bucket rate limit info
// Discord groups related endpoints into buckets
class CRateLimitBucket
{
	U8      hash[RATELIMIT_BUCKET_HASH_LEN];    // X-RateLimit-Bucket header
	U8      route[RATELIMIT_MAX_ROUTE_LEN];     // Route pattern (for lookup)
	I64     limit;                              // Max requests in window
	I64     remaining;                          // Remaining requests
	U64     reset_at;                           // Reset time (ms since epoch)
	U64     reset_after;                        // Reset after (ms)
	Bool    active;                             // Whether this bucket is in use
};

// ============================================================================
// RATE LIMITER
// ============================================================================

// Global rate limiter state
class CRateLimiter
{
	CRateLimitBucket  buckets[RATELIMIT_MAX_BUCKETS];    // Bucket storage
	I64               bucket_count;                      // Active bucket count
	Bool              global_limited;                    // Global rate limit active
	U64               global_reset;                      // Global reset time (ms)
	U64               last_request;                      // Last request time
};

// ============================================================================
// ROUTE INFO FOR RATE LIMIT SCOPE
// ============================================================================

// Discord rate limits are per-route with some major parameters
// e.g., /channels/{channel_id}/messages shares a bucket
class CRateLimitRoute
{
	U8      method[8];                          // GET, POST, etc.
	U8      pattern[RATELIMIT_MAX_ROUTE_LEN];   // Route pattern
	U8      major_param[32];                    // Major parameter value (channel_id, guild_id)
};

