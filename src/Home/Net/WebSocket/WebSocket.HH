// WebSocket.HH - WebSocket Protocol definitions for ZealOS
// Part of ZealOS TLS/HTTP/WebSocket implementation
// RFC 6455 compliant

// ============================================================================
// WEBSOCKET OPCODES
// ============================================================================

#define WS_OPCODE_CONTINUATION  0x00
#define WS_OPCODE_TEXT          0x01
#define WS_OPCODE_BINARY        0x02
#define WS_OPCODE_CLOSE         0x08
#define WS_OPCODE_PING          0x09
#define WS_OPCODE_PONG          0x0A

// ============================================================================
// WEBSOCKET CLOSE CODES
// ============================================================================

#define WS_CLOSE_NORMAL           1000
#define WS_CLOSE_GOING_AWAY       1001
#define WS_CLOSE_PROTOCOL_ERROR   1002
#define WS_CLOSE_UNSUPPORTED      1003
#define WS_CLOSE_NO_STATUS        1005
#define WS_CLOSE_ABNORMAL         1006
#define WS_CLOSE_INVALID_DATA     1007
#define WS_CLOSE_POLICY_VIOLATION 1008
#define WS_CLOSE_MESSAGE_TOO_BIG  1009
#define WS_CLOSE_EXTENSION_NEEDED 1010
#define WS_CLOSE_UNEXPECTED_ERROR 1011

// ============================================================================
// WEBSOCKET STATE
// ============================================================================

#define WS_STATE_CONNECTING    0
#define WS_STATE_OPEN          1
#define WS_STATE_CLOSING       2
#define WS_STATE_CLOSED        3

// ============================================================================
// WEBSOCKET ERROR CODES
// ============================================================================

#define WS_OK                       0
#define WS_ERR_INVALID_URL         -1
#define WS_ERR_CONNECTION_FAILED   -2
#define WS_ERR_HANDSHAKE_FAILED    -3
#define WS_ERR_SEND_FAILED         -4
#define WS_ERR_RECV_FAILED         -5
#define WS_ERR_INVALID_FRAME       -6
#define WS_ERR_FRAME_TOO_LARGE     -7
#define WS_ERR_CLOSED              -8
#define WS_ERR_TIMEOUT             -9

// ============================================================================
// SIZE LIMITS
// ============================================================================

#define WS_MAX_FRAME_SIZE       (16 * 1024 * 1024)  // 16 MB max frame
#define WS_MAX_HEADER_SIZE      14  // Max WebSocket frame header
#define WS_GUID                 "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

// ============================================================================
// WEBSOCKET FRAME HEADER
// ============================================================================

// Frame format (from RFC 6455):
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
// |     Extended payload length continued, if payload len == 127  |
// + - - - - - - - - - - - - - - - +-------------------------------+
// |                               |Masking-key, if MASK set to 1  |
// +-------------------------------+-------------------------------+
// | Masking-key (continued)       |          Payload Data         |
// +-------------------------------- - - - - - - - - - - - - - - - +
// :                     Payload Data continued ...                :
// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
// |                     Payload Data continued ...                |
// +---------------------------------------------------------------+

class CWSFrame
{
	Bool      fin;          // Final fragment
	U8        opcode;       // Opcode
	Bool      masked;       // Is masked
	U64       payload_len;  // Payload length
	U8        mask_key[4];  // Masking key (if masked)
	U8       *payload;      // Payload data
};

// ============================================================================
// WEBSOCKET MESSAGE
// ============================================================================

class CWSMessage
{
	U8        opcode;       // Message type (text/binary)
	U8       *data;         // Message data
	I64       length;       // Message length
};

// ============================================================================
// WEBSOCKET CONNECTION
// ============================================================================

class CWebSocket
{
	CTLSSocket   *tls_socket;    // TLS connection (for WSS)
	CTCPSocket   *tcp_socket;    // Plain TCP (for WS)
	Bool          use_tls;       // Using secure WebSocket?
	I64           state;         // Connection state

	U8           *host;          // Server hostname
	U16           port;          // Server port
	U8           *path;          // Request path

	I64           timeout;       // Read timeout (ms)

	// Receive buffer for fragmented messages
	U8           *recv_buffer;
	I64           recv_buffer_size;
	I64           recv_buffer_len;
	U8            recv_opcode;   // Opcode of fragmented message

	// Close status
	U16           close_code;
	U8           *close_reason;
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

U8 *WSErrorString(I64 error)
{
	switch (error)
	{
		case WS_OK:                     return "Success";
		case WS_ERR_INVALID_URL:        return "Invalid URL";
		case WS_ERR_CONNECTION_FAILED:  return "Connection failed";
		case WS_ERR_HANDSHAKE_FAILED:   return "Handshake failed";
		case WS_ERR_SEND_FAILED:        return "Send failed";
		case WS_ERR_RECV_FAILED:        return "Receive failed";
		case WS_ERR_INVALID_FRAME:      return "Invalid frame";
		case WS_ERR_FRAME_TOO_LARGE:    return "Frame too large";
		case WS_ERR_CLOSED:             return "Connection closed";
		case WS_ERR_TIMEOUT:            return "Timeout";
		default:                        return "Unknown error";
	}
}
