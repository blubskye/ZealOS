// HTTP.HH - HTTP Protocol definitions for ZealOS
// Part of ZealOS TLS/HTTP/WebSocket implementation

// ============================================================================
// HTTP METHODS
// ============================================================================

#define HTTP_METHOD_GET     0
#define HTTP_METHOD_POST    1
#define HTTP_METHOD_PUT     2
#define HTTP_METHOD_DELETE  3
#define HTTP_METHOD_PATCH   4
#define HTTP_METHOD_HEAD    5
#define HTTP_METHOD_OPTIONS 6

// ============================================================================
// HTTP VERSIONS
// ============================================================================

#define HTTP_VERSION_1_0    0
#define HTTP_VERSION_1_1    1

// ============================================================================
// COMMON HTTP STATUS CODES
// ============================================================================

#define HTTP_STATUS_OK                  200
#define HTTP_STATUS_CREATED             201
#define HTTP_STATUS_NO_CONTENT          204
#define HTTP_STATUS_MOVED_PERMANENTLY   301
#define HTTP_STATUS_FOUND               302
#define HTTP_STATUS_NOT_MODIFIED        304
#define HTTP_STATUS_BAD_REQUEST         400
#define HTTP_STATUS_UNAUTHORIZED        401
#define HTTP_STATUS_FORBIDDEN           403
#define HTTP_STATUS_NOT_FOUND           404
#define HTTP_STATUS_METHOD_NOT_ALLOWED  405
#define HTTP_STATUS_TOO_MANY_REQUESTS   429
#define HTTP_STATUS_INTERNAL_ERROR      500
#define HTTP_STATUS_BAD_GATEWAY         502
#define HTTP_STATUS_SERVICE_UNAVAILABLE 503
#define HTTP_STATUS_GATEWAY_TIMEOUT     504

// ============================================================================
// HTTP ERROR CODES
// ============================================================================

#define HTTP_OK                      0
#define HTTP_ERR_INVALID_URL        -1
#define HTTP_ERR_CONNECTION_FAILED  -2
#define HTTP_ERR_TLS_FAILED         -3
#define HTTP_ERR_SEND_FAILED        -4
#define HTTP_ERR_RECV_FAILED        -5
#define HTTP_ERR_INVALID_RESPONSE   -6
#define HTTP_ERR_TIMEOUT            -7
#define HTTP_ERR_BUFFER_OVERFLOW    -8
#define HTTP_ERR_OUT_OF_MEMORY      -9

// ============================================================================
// SIZE LIMITS
// ============================================================================

#define HTTP_MAX_HEADER_SIZE        8192
#define HTTP_MAX_URL_SIZE           2048
#define HTTP_MAX_HEADERS            64
#define HTTP_DEFAULT_PORT           80
#define HTTPS_DEFAULT_PORT          443

// ============================================================================
// HTTP HEADER
// ============================================================================

class CHTTPHeader
{
	U8  *name;
	U8  *value;
};

// ============================================================================
// HTTP REQUEST
// ============================================================================

class CHTTPRequest
{
	I64           method;           // HTTP_METHOD_*
	U8           *url;              // Full URL
	U8           *host;             // Hostname
	U8           *path;             // Path (including query string)
	U16           port;             // Port number
	Bool          use_tls;          // HTTPS?

	CHTTPHeader   headers[HTTP_MAX_HEADERS];
	I64           header_count;

	U8           *body;             // Request body (for POST/PUT)
	I64           body_length;
};

// ============================================================================
// HTTP RESPONSE
// ============================================================================

class CHTTPResponse
{
	I64           status_code;      // HTTP status code
	U8           *status_text;      // Status text (e.g., "OK")

	CHTTPHeader   headers[HTTP_MAX_HEADERS];
	I64           header_count;

	U8           *body;             // Response body
	I64           body_length;
	I64           body_capacity;    // Allocated size

	// Parsed headers
	I64           content_length;   // Content-Length header value (-1 if not present)
	Bool          chunked;          // Transfer-Encoding: chunked
	Bool          keep_alive;       // Connection: keep-alive
};

// ============================================================================
// HTTP CLIENT
// ============================================================================

class CHTTPClient
{
	CTLSSocket   *tls_socket;       // TLS connection (for HTTPS)
	CTCPSocket   *tcp_socket;       // Plain TCP connection (for HTTP)
	Bool          use_tls;          // Using HTTPS?
	Bool          connected;        // Is connected?
	U8           *current_host;     // Current connected host
	U16           current_port;     // Current connected port
	I64           timeout;          // Timeout in ms
};

// ============================================================================
// HELPER FUNCTION DECLARATIONS
// ============================================================================

// Get method string from enum
U8 *HTTPMethodString(I64 method)
{
	switch (method)
	{
		case HTTP_METHOD_GET:     return "GET";
		case HTTP_METHOD_POST:    return "POST";
		case HTTP_METHOD_PUT:     return "PUT";
		case HTTP_METHOD_DELETE:  return "DELETE";
		case HTTP_METHOD_PATCH:   return "PATCH";
		case HTTP_METHOD_HEAD:    return "HEAD";
		case HTTP_METHOD_OPTIONS: return "OPTIONS";
		default:                  return "GET";
	}
}

// Get error string
U8 *HTTPErrorString(I64 error)
{
	switch (error)
	{
		case HTTP_OK:                   return "Success";
		case HTTP_ERR_INVALID_URL:      return "Invalid URL";
		case HTTP_ERR_CONNECTION_FAILED:return "Connection failed";
		case HTTP_ERR_TLS_FAILED:       return "TLS handshake failed";
		case HTTP_ERR_SEND_FAILED:      return "Send failed";
		case HTTP_ERR_RECV_FAILED:      return "Receive failed";
		case HTTP_ERR_INVALID_RESPONSE: return "Invalid response";
		case HTTP_ERR_TIMEOUT:          return "Timeout";
		case HTTP_ERR_BUFFER_OVERFLOW:  return "Buffer overflow";
		case HTTP_ERR_OUT_OF_MEMORY:    return "Out of memory";
		default:                        return "Unknown error";
	}
}
