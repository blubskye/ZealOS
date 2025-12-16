// JSON.HH - JSON Parser Type Definitions for ZealOS
// Part of ZealOS Discord implementation
// Provides proper JSON parsing with array and nested object support

// ============================================================================
// JSON VALUE TYPES
// ============================================================================

#define JSON_NULL       0
#define JSON_BOOL       1
#define JSON_NUMBER     2
#define JSON_STRING     3
#define JSON_ARRAY      4
#define JSON_OBJECT     5

// ============================================================================
// JSON ERROR CODES
// ============================================================================

#define JSON_OK                     0
#define JSON_ERR_UNEXPECTED_CHAR   -1
#define JSON_ERR_UNEXPECTED_END    -2
#define JSON_ERR_INVALID_VALUE     -3
#define JSON_ERR_INVALID_STRING    -4
#define JSON_ERR_INVALID_NUMBER    -5
#define JSON_ERR_MAX_DEPTH         -6
#define JSON_ERR_OUT_OF_MEMORY     -7

// ============================================================================
// LIMITS
// ============================================================================

#define JSON_MAX_DEPTH          64      // Maximum nesting depth
#define JSON_INITIAL_CAPACITY   8       // Initial array/object capacity

// ============================================================================
// JSON VALUE STRUCTURE
// ============================================================================

// Forward declaration for recursive structure
class CJSONValue;

// Array storage
class CJSONArray
{
	CJSONValue **items;     // Array of value pointers
	I64          count;     // Number of items
	I64          capacity;  // Allocated capacity
};

// Object storage (key-value pairs)
class CJSONObject
{
	U8         **keys;      // Array of key strings
	CJSONValue **values;    // Array of value pointers
	I64          count;     // Number of pairs
	I64          capacity;  // Allocated capacity
};

// Main JSON value structure
class CJSONValue
{
	U8  type;               // JSON_NULL, JSON_BOOL, etc.

	// Value storage (only one is valid based on type)
	Bool           bool_val;    // JSON_BOOL
	I64            int_val;     // JSON_NUMBER (integer part)
	F64            float_val;   // JSON_NUMBER (for decimals)
	Bool           is_float;    // TRUE if number has decimal
	U8            *string_val;  // JSON_STRING (allocated)
	CJSONArray     array;       // JSON_ARRAY
	CJSONObject    object;      // JSON_OBJECT
};

// ============================================================================
// PARSER STATE
// ============================================================================

class CJSONParser
{
	U8     *input;          // Input JSON string
	I64     pos;            // Current position
	I64     length;         // Input length
	I64     line;           // Current line number (for errors)
	I64     col;            // Current column
	I64     depth;          // Current nesting depth
	I64     error;          // Error code if any
	U8      error_msg[128]; // Error message
};
