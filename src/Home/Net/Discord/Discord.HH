// Discord.HH - Discord API definitions for ZealOS
// Minimal Discord bot implementation

// ============================================================================
// DISCORD API CONSTANTS
// ============================================================================

#define DISCORD_API_VERSION     "10"
#define DISCORD_API_BASE        "https://discord.com/api/v10"
#define DISCORD_GATEWAY_URL     "wss://gateway.discord.gg/?v=10&encoding=json"

// ============================================================================
// GATEWAY OPCODES
// ============================================================================

#define DISCORD_OP_DISPATCH             0   // Event dispatch
#define DISCORD_OP_HEARTBEAT            1   // Heartbeat
#define DISCORD_OP_IDENTIFY             2   // Identify
#define DISCORD_OP_PRESENCE_UPDATE      3   // Update presence
#define DISCORD_OP_VOICE_STATE_UPDATE   4   // Voice state update
#define DISCORD_OP_RESUME               6   // Resume
#define DISCORD_OP_RECONNECT            7   // Reconnect
#define DISCORD_OP_REQUEST_MEMBERS      8   // Request guild members
#define DISCORD_OP_INVALID_SESSION      9   // Invalid session
#define DISCORD_OP_HELLO               10   // Hello
#define DISCORD_OP_HEARTBEAT_ACK       11   // Heartbeat ACK

// ============================================================================
// GATEWAY INTENTS
// ============================================================================

#define DISCORD_INTENT_GUILDS                       (1 << 0)
#define DISCORD_INTENT_GUILD_MEMBERS                (1 << 1)
#define DISCORD_INTENT_GUILD_MODERATION             (1 << 2)
#define DISCORD_INTENT_GUILD_EMOJIS                 (1 << 3)
#define DISCORD_INTENT_GUILD_INTEGRATIONS           (1 << 4)
#define DISCORD_INTENT_GUILD_WEBHOOKS               (1 << 5)
#define DISCORD_INTENT_GUILD_INVITES                (1 << 6)
#define DISCORD_INTENT_GUILD_VOICE_STATES           (1 << 7)
#define DISCORD_INTENT_GUILD_PRESENCES              (1 << 8)
#define DISCORD_INTENT_GUILD_MESSAGES               (1 << 9)
#define DISCORD_INTENT_GUILD_MESSAGE_REACTIONS      (1 << 10)
#define DISCORD_INTENT_GUILD_MESSAGE_TYPING         (1 << 11)
#define DISCORD_INTENT_DIRECT_MESSAGES              (1 << 12)
#define DISCORD_INTENT_DIRECT_MESSAGE_REACTIONS     (1 << 13)
#define DISCORD_INTENT_DIRECT_MESSAGE_TYPING        (1 << 14)
#define DISCORD_INTENT_MESSAGE_CONTENT              (1 << 15)
#define DISCORD_INTENT_GUILD_SCHEDULED_EVENTS       (1 << 16)

// Default intents (non-privileged)
#define DISCORD_INTENTS_DEFAULT \
	(DISCORD_INTENT_GUILDS | \
	 DISCORD_INTENT_GUILD_MESSAGES | \
	 DISCORD_INTENT_DIRECT_MESSAGES)

// ============================================================================
// DISCORD ERROR CODES
// ============================================================================

#define DISCORD_OK                       0
#define DISCORD_ERR_INVALID_TOKEN       -1
#define DISCORD_ERR_CONNECTION_FAILED   -2
#define DISCORD_ERR_GATEWAY_FAILED      -3
#define DISCORD_ERR_HEARTBEAT_TIMEOUT   -4
#define DISCORD_ERR_INVALID_SESSION     -5
#define DISCORD_ERR_RATE_LIMITED        -6
#define DISCORD_ERR_API_ERROR           -7

// ============================================================================
// DISCORD STATE
// ============================================================================

#define DISCORD_STATE_DISCONNECTED      0
#define DISCORD_STATE_CONNECTING        1
#define DISCORD_STATE_IDENTIFYING       2
#define DISCORD_STATE_CONNECTED         3
#define DISCORD_STATE_RESUMING          4
#define DISCORD_STATE_RECONNECTING      5

// ============================================================================
// SIZE LIMITS
// ============================================================================

#define DISCORD_MAX_MESSAGE_LEN         2000
#define DISCORD_MAX_EMBED_TOTAL         6000
#define DISCORD_MAX_TOKEN_LEN           100

// ============================================================================
// SNOWFLAKE TYPE
// ============================================================================

// Discord uses 64-bit snowflake IDs
// For ZealOS we use U64
typedef U64 CDiscordSnowflake;

// ============================================================================
// SIMPLE JSON BUILDER
// ============================================================================

// Simple JSON string builder for Discord payloads
class CJSONBuilder
{
	U8     *buffer;
	I64     capacity;
	I64     length;
	I64     depth;
	Bool    need_comma;
};

// ============================================================================
// DISCORD USER
// ============================================================================

class CDiscordUser
{
	CDiscordSnowflake  id;
	U8                *username;
	U8                *discriminator;
	U8                *avatar;
	Bool               bot;
};

// ============================================================================
// DISCORD MESSAGE
// ============================================================================

class CDiscordMessage
{
	CDiscordSnowflake  id;
	CDiscordSnowflake  channel_id;
	CDiscordSnowflake  guild_id;
	CDiscordUser       author;
	U8                *content;
	U64                timestamp;
	Bool               mention_everyone;
};

// ============================================================================
// DISCORD GUILD
// ============================================================================

class CDiscordGuild
{
	CDiscordSnowflake  id;
	U8                *name;
	U8                *icon;
	CDiscordSnowflake  owner_id;
	I64                member_count;
	U8                *description;
	Bool               unavailable;     // True if guild is unavailable
};

// ============================================================================
// DISCORD MEMBER
// ============================================================================

class CDiscordMember
{
	CDiscordUser       user;
	U8                *nick;            // Nickname in this guild (NULL if none)
	CDiscordSnowflake *roles;           // Array of role IDs
	I64                role_count;
	U64                joined_at;
	Bool               deaf;
	Bool               mute;
};

// ============================================================================
// DISCORD EMOJI
// ============================================================================

class CDiscordEmoji
{
	CDiscordSnowflake  id;              // NULL for unicode emoji
	U8                *name;            // Emoji name or unicode character
	Bool               animated;
};

// ============================================================================
// DISCORD REACTION
// ============================================================================

class CDiscordReaction
{
	CDiscordSnowflake  user_id;
	CDiscordSnowflake  channel_id;
	CDiscordSnowflake  message_id;
	CDiscordSnowflake  guild_id;
	CDiscordEmoji      emoji;
};

// ============================================================================
// DISCORD INTERACTION TYPES
// ============================================================================

#define DISCORD_INTERACTION_PING                    1
#define DISCORD_INTERACTION_APPLICATION_COMMAND     2
#define DISCORD_INTERACTION_MESSAGE_COMPONENT       3
#define DISCORD_INTERACTION_APPLICATION_COMMAND_AUTOCOMPLETE  4
#define DISCORD_INTERACTION_MODAL_SUBMIT            5

// Interaction Response Types
#define DISCORD_RESPONSE_PONG                       1
#define DISCORD_RESPONSE_CHANNEL_MESSAGE            4
#define DISCORD_RESPONSE_DEFERRED_CHANNEL_MESSAGE   5
#define DISCORD_RESPONSE_DEFERRED_UPDATE_MESSAGE    6
#define DISCORD_RESPONSE_UPDATE_MESSAGE             7
#define DISCORD_RESPONSE_APPLICATION_COMMAND_AUTOCOMPLETE_RESULT  8
#define DISCORD_RESPONSE_MODAL                      9

// Application Command Option Types
#define DISCORD_OPTION_SUB_COMMAND       1
#define DISCORD_OPTION_SUB_COMMAND_GROUP 2
#define DISCORD_OPTION_STRING            3
#define DISCORD_OPTION_INTEGER           4
#define DISCORD_OPTION_BOOLEAN           5
#define DISCORD_OPTION_USER              6
#define DISCORD_OPTION_CHANNEL           7
#define DISCORD_OPTION_ROLE              8
#define DISCORD_OPTION_MENTIONABLE       9
#define DISCORD_OPTION_NUMBER           10
#define DISCORD_OPTION_ATTACHMENT       11

// ============================================================================
// DISCORD INTERACTION
// ============================================================================

class CDiscordInteractionOption
{
	U8                *name;
	I64                type;
	U8                *string_val;      // For string options
	I64                int_val;         // For integer/boolean options
	CDiscordSnowflake  snowflake_val;   // For user/channel/role options
};

class CDiscordInteractionData
{
	CDiscordSnowflake  id;              // Command ID
	U8                *name;            // Command name
	I64                type;            // Command type
	CDiscordInteractionOption *options; // Array of options
	I64                option_count;
	U8                *custom_id;       // For components
};

class CDiscordInteraction
{
	CDiscordSnowflake     id;
	CDiscordSnowflake     application_id;
	I64                   type;
	CDiscordInteractionData data;
	CDiscordSnowflake     guild_id;
	CDiscordSnowflake     channel_id;
	CDiscordMember        member;       // For guild interactions
	CDiscordUser          user;         // For DM interactions
	U8                   *token;        // Interaction token for responses
};

// ============================================================================
// DISCORD TYPING
// ============================================================================

class CDiscordTyping
{
	CDiscordSnowflake  channel_id;
	CDiscordSnowflake  guild_id;
	CDiscordSnowflake  user_id;
	U64                timestamp;
};

// ============================================================================
// DISCORD PRESENCE
// ============================================================================

#define DISCORD_STATUS_ONLINE       0
#define DISCORD_STATUS_DND          1
#define DISCORD_STATUS_IDLE         2
#define DISCORD_STATUS_INVISIBLE    3
#define DISCORD_STATUS_OFFLINE      4

class CDiscordPresence
{
	CDiscordUser       user;
	CDiscordSnowflake  guild_id;
	I64                status;         // DISCORD_STATUS_*
	U8                *status_text;    // Custom status
};

// ============================================================================
// EVENT CALLBACKS
// ============================================================================

// Forward declaration
class CDiscordClient;

// Event callback types - Connection
typedef U0 (*DiscordOnReady)(CDiscordClient *client, CDiscordUser *user);
typedef U0 (*DiscordOnError)(CDiscordClient *client, I64 error, U8 *message);

// Event callback types - Messages
typedef U0 (*DiscordOnMessage)(CDiscordClient *client, CDiscordMessage *msg);
typedef U0 (*DiscordOnMessageUpdate)(CDiscordClient *client, CDiscordMessage *msg);
typedef U0 (*DiscordOnMessageDelete)(CDiscordClient *client, CDiscordSnowflake channel_id, CDiscordSnowflake message_id, CDiscordSnowflake guild_id);

// Event callback types - Guild
typedef U0 (*DiscordOnGuildCreate)(CDiscordClient *client, CDiscordGuild *guild);
typedef U0 (*DiscordOnGuildUpdate)(CDiscordClient *client, CDiscordGuild *guild);
typedef U0 (*DiscordOnGuildDelete)(CDiscordClient *client, CDiscordSnowflake guild_id);

// Event callback types - Members
typedef U0 (*DiscordOnMemberAdd)(CDiscordClient *client, CDiscordSnowflake guild_id, CDiscordMember *member);
typedef U0 (*DiscordOnMemberRemove)(CDiscordClient *client, CDiscordSnowflake guild_id, CDiscordUser *user);
typedef U0 (*DiscordOnMemberUpdate)(CDiscordClient *client, CDiscordSnowflake guild_id, CDiscordMember *member);

// Event callback types - Reactions
typedef U0 (*DiscordOnReactionAdd)(CDiscordClient *client, CDiscordReaction *reaction);
typedef U0 (*DiscordOnReactionRemove)(CDiscordClient *client, CDiscordReaction *reaction);
typedef U0 (*DiscordOnReactionRemoveAll)(CDiscordClient *client, CDiscordSnowflake channel_id, CDiscordSnowflake message_id, CDiscordSnowflake guild_id);

// Event callback types - Interactions
typedef U0 (*DiscordOnInteraction)(CDiscordClient *client, CDiscordInteraction *interaction);

// Event callback types - Typing
typedef U0 (*DiscordOnTypingStart)(CDiscordClient *client, CDiscordTyping *typing);

// Event callback types - Presence
typedef U0 (*DiscordOnPresenceUpdate)(CDiscordClient *client, CDiscordPresence *presence);

// ============================================================================
// DISCORD CLIENT
// ============================================================================

// Forward declaration for rate limiter
class CRateLimiter;

class CDiscordClient
{
	// Authentication
	U8                 *token;
	I64                 intents;

	// Gateway connection
	CWebSocket         *gateway;
	I64                 state;

	// Session info
	U8                 *session_id;
	I64                 sequence;
	U8                 *resume_gateway_url;

	// Heartbeat
	I64                 heartbeat_interval;
	I64                 last_heartbeat;
	I64                 last_heartbeat_ack;
	Bool                heartbeat_acked;

	// Bot user info
	CDiscordUser        self;

	// HTTP client for REST API
	CHTTPClient        *http;

	// Rate limiter for API requests
	CRateLimiter       *rate_limiter;

	// Event callbacks - Connection
	DiscordOnReady           on_ready;
	DiscordOnError           on_error;

	// Event callbacks - Messages
	DiscordOnMessage         on_message;
	DiscordOnMessageUpdate   on_message_update;
	DiscordOnMessageDelete   on_message_delete;

	// Event callbacks - Guild
	DiscordOnGuildCreate     on_guild_create;
	DiscordOnGuildUpdate     on_guild_update;
	DiscordOnGuildDelete     on_guild_delete;

	// Event callbacks - Members
	DiscordOnMemberAdd       on_member_add;
	DiscordOnMemberRemove    on_member_remove;
	DiscordOnMemberUpdate    on_member_update;

	// Event callbacks - Reactions
	DiscordOnReactionAdd     on_reaction_add;
	DiscordOnReactionRemove  on_reaction_remove;
	DiscordOnReactionRemoveAll on_reaction_remove_all;

	// Event callbacks - Interactions
	DiscordOnInteraction     on_interaction;

	// Event callbacks - Typing
	DiscordOnTypingStart     on_typing_start;

	// Event callbacks - Presence
	DiscordOnPresenceUpdate  on_presence_update;

	// User data
	U0                 *user_data;

	// Running flag
	Bool                running;
};

// ============================================================================
// HELPER FUNCTION DECLARATIONS
// ============================================================================

U8 *DiscordErrorString(I64 error)
{
	switch (error)
	{
		case DISCORD_OK:                    return "Success";
		case DISCORD_ERR_INVALID_TOKEN:     return "Invalid token";
		case DISCORD_ERR_CONNECTION_FAILED: return "Connection failed";
		case DISCORD_ERR_GATEWAY_FAILED:    return "Gateway error";
		case DISCORD_ERR_HEARTBEAT_TIMEOUT: return "Heartbeat timeout";
		case DISCORD_ERR_INVALID_SESSION:   return "Invalid session";
		case DISCORD_ERR_RATE_LIMITED:      return "Rate limited";
		case DISCORD_ERR_API_ERROR:         return "API error";
		default:                            return "Unknown error";
	}
}
