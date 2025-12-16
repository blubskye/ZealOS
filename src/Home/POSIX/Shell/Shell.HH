// Shell.HH - POSIX Shell Definitions for ZealOS
// Part of ZealOS POSIX compatibility layer
//
// Implements a POSIX-compliant shell with:
// - Command parsing and execution
// - Pipelines and redirections
// - Variables and expansion
// - Control flow (if/while/for/case)
// - Job control

// ============================================================================
// TOKEN TYPES
// ============================================================================

#define TOK_EOF             0
#define TOK_NEWLINE         1
#define TOK_WORD            2
#define TOK_ASSIGNMENT      3       // name=value

// Operators
#define TOK_SEMI            10      // ;
#define TOK_AMP             11      // &
#define TOK_PIPE            12      // |
#define TOK_LPAREN          13      // (
#define TOK_RPAREN          14      // )
#define TOK_AND             15      // &&
#define TOK_OR              16      // ||
#define TOK_DSEMI           17      // ;;

// Redirections
#define TOK_LESS            20      // <
#define TOK_GREAT           21      // >
#define TOK_DLESS           22      // <<
#define TOK_DGREAT          23      // >>
#define TOK_LESSAND         24      // <&
#define TOK_GREATAND        25      // >&
#define TOK_LESSGREAT       26      // <>
#define TOK_CLOBBER         27      // >|
#define TOK_DLESSDASH       28      // <<-

// Reserved words
#define TOK_IF              30
#define TOK_THEN            31
#define TOK_ELSE            32
#define TOK_ELIF            33
#define TOK_FI              34
#define TOK_DO              35
#define TOK_DONE            36
#define TOK_CASE            37
#define TOK_ESAC            38
#define TOK_WHILE           39
#define TOK_UNTIL           40
#define TOK_FOR             41
#define TOK_IN              42
#define TOK_LBRACE          43      // {
#define TOK_RBRACE          44      // }
#define TOK_BANG            45      // !
#define TOK_FUNCTION        46

// ============================================================================
// LEXER STATE
// ============================================================================

#define LEXER_BUFFER_SIZE   4096

class CShellToken
{
	I64     type;                   // TOK_*
	U8     *value;                  // Token text
	I64     length;                 // Length of value
	I64     line;                   // Line number
	I64     column;                 // Column number
};

class CShellLexer
{
	// Input
	U8     *input;                  // Input string
	I64     input_len;              // Input length
	I64     pos;                    // Current position
	I64     line;                   // Current line
	I64     column;                 // Current column

	// Token buffer
	U8      buffer[LEXER_BUFFER_SIZE];
	I64     buffer_len;

	// Current token
	CShellToken current;

	// State
	Bool    at_line_start;          // For alias expansion
	Bool    in_case;                // Inside case statement
};

// ============================================================================
// QUOTING STATE
// ============================================================================

#define QUOTE_NONE          0
#define QUOTE_SINGLE        1       // 'string'
#define QUOTE_DOUBLE        2       // "string"
#define QUOTE_BACKSLASH     3       // \char
#define QUOTE_DOLLAR        4       // $'string' (ANSI-C)
#define QUOTE_BACKTICK      5       // `command`

// ============================================================================
// AST NODE TYPES
// ============================================================================

#define NODE_COMMAND        1       // Simple command
#define NODE_PIPELINE       2       // cmd | cmd
#define NODE_AND            3       // cmd && cmd
#define NODE_OR             4       // cmd || cmd
#define NODE_SEMI           5       // cmd ; cmd
#define NODE_BACKGROUND     6       // cmd &
#define NODE_SUBSHELL       7       // ( cmd )
#define NODE_BRACE          8       // { cmd }
#define NODE_IF             9       // if/then/else/fi
#define NODE_WHILE          10      // while/do/done
#define NODE_UNTIL          11      // until/do/done
#define NODE_FOR            12      // for/in/do/done
#define NODE_CASE           13      // case/in/esac
#define NODE_FUNCTION       14      // function definition
#define NODE_BANG           15      // ! cmd (negate)

// ============================================================================
// REDIRECTION
// ============================================================================

#define REDIR_INPUT         1       // < file
#define REDIR_OUTPUT        2       // > file
#define REDIR_APPEND        3       // >> file
#define REDIR_HEREDOC       4       // << delimiter
#define REDIR_HEREDOC_STRIP 5       // <<- delimiter
#define REDIR_DUP_INPUT     6       // <& fd
#define REDIR_DUP_OUTPUT    7       // >& fd
#define REDIR_RDWR          8       // <> file
#define REDIR_CLOBBER       9       // >| file

class CShellRedir
{
	I64     type;                   // REDIR_*
	I64     fd;                     // File descriptor (default: 0 for input, 1 for output)
	U8     *target;                 // Filename or fd number
	CShellRedir *next;              // Next redirection
};

// ============================================================================
// AST NODES
// ============================================================================

class CShellWord
{
	U8     *text;                   // Word text (before expansion)
	CShellWord *next;               // Next word in list
};

class CShellCommand;

class CShellSimpleCmd
{
	CShellWord *words;              // Command and arguments
	CShellWord *assignments;        // VAR=value before command
	CShellRedir *redirects;         // Redirections
};

class CShellPipeline
{
	CShellCommand **commands;       // Commands in pipeline
	I64     command_count;
	Bool    negated;                // ! prefix
};

class CShellIf
{
	CShellCommand *condition;       // Condition
	CShellCommand *then_part;       // Then branch
	CShellCommand *else_part;       // Else branch (may be another if for elif)
};

class CShellLoop
{
	CShellCommand *condition;       // While/until condition
	CShellCommand *body;            // Loop body
};

class CShellFor
{
	U8     *variable;               // Loop variable name
	CShellWord *words;              // Word list (or $@ if NULL)
	CShellCommand *body;            // Loop body
};

class CShellCaseItem
{
	CShellWord *patterns;           // Patterns to match
	CShellCommand *body;            // Commands if matched
	CShellCaseItem *next;           // Next case item
};

class CShellCase
{
	CShellWord *word;               // Word to match against
	CShellCaseItem *items;          // Case items
};

class CShellFunction
{
	U8     *name;                   // Function name
	CShellCommand *body;            // Function body
};

class CShellCommand
{
	I64     type;                   // NODE_*

	union {
		CShellSimpleCmd simple;     // NODE_COMMAND
		CShellPipeline  pipeline;   // NODE_PIPELINE
		CShellIf        if_cmd;     // NODE_IF
		CShellLoop      loop;       // NODE_WHILE, NODE_UNTIL
		CShellFor       for_cmd;    // NODE_FOR
		CShellCase      case_cmd;   // NODE_CASE
		CShellFunction  function;   // NODE_FUNCTION
		struct {
			CShellCommand *left;
			CShellCommand *right;
		};                          // NODE_AND, NODE_OR, NODE_SEMI, NODE_BACKGROUND
		CShellCommand  *child;      // NODE_SUBSHELL, NODE_BRACE, NODE_BANG
	};

	CShellRedir *redirects;         // Redirections (for compound commands)
};

// ============================================================================
// PARSER STATE
// ============================================================================

class CShellParser
{
	CShellLexer lexer;
	Bool    error;
	U8     *error_msg;
	I64     error_line;
};

// ============================================================================
// SHELL VARIABLES
// ============================================================================

class CShellVariable
{
	U8     *name;
	U8     *value;
	Bool    exported;               // In environment
	Bool    readonly;               // Cannot be changed
	CShellVariable *next;
};

// ============================================================================
// SHELL FUNCTION TABLE
// ============================================================================

class CShellFunc
{
	U8     *name;
	CShellCommand *body;
	CShellFunc *next;
};

// ============================================================================
// ALIAS TABLE
// ============================================================================

class CShellAlias
{
	U8     *name;
	U8     *value;
	CShellAlias *next;
};

// ============================================================================
// JOB CONTROL
// ============================================================================

#define JOB_RUNNING         1
#define JOB_STOPPED         2
#define JOB_DONE            3

class CShellJob
{
	I64     id;                     // Job number
	I64     pgid;                   // Process group ID
	I64    *pids;                   // Process IDs in job
	I64     pid_count;
	I64     status;                 // JOB_*
	U8     *command;                // Command text
	Bool    foreground;             // Was started in foreground
	CShellJob *next;
};

// ============================================================================
// SHELL STATE
// ============================================================================

class CShellState
{
	// Variables
	CShellVariable *variables;

	// Functions
	CShellFunc *functions;

	// Aliases
	CShellAlias *aliases;

	// Jobs
	CShellJob *jobs;
	I64 next_job_id;

	// Special variables
	I64     last_status;            // $?
	I64     last_pid;               // $!
	U8     *argv0;                  // $0

	// Positional parameters
	U8    **positional;             // $1, $2, ...
	I64     positional_count;       // $#

	// Options
	Bool    interactive;
	Bool    login_shell;
	Bool    errexit;                // -e: exit on error
	Bool    nounset;                // -u: error on unset variable
	Bool    xtrace;                 // -x: trace execution
	Bool    noexec;                 // -n: don't execute
	Bool    noclobber;              // -C: don't overwrite with >
	Bool    allexport;              // -a: export all variables

	// Input state
	U8     *ps1;                    // Primary prompt
	U8     *ps2;                    // Secondary prompt (continuation)
	U8     *ps4;                    // Trace prefix

	// Linked POSIX process
	CPosixProcess *posix_proc;

	// Heredoc processing
	CShellRedir *heredocs;          // Pending heredocs
};

// ============================================================================
// FUNCTION DECLARATIONS - Lexer
// ============================================================================

U0 ShellLexerInit(CShellLexer *lex, U8 *input);
I64 ShellLexerNext(CShellLexer *lex);
I64 ShellLexerPeek(CShellLexer *lex);
U0 ShellLexerFree(CShellLexer *lex);

// ============================================================================
// FUNCTION DECLARATIONS - Parser
// ============================================================================

U0 ShellParserInit(CShellParser *parser, U8 *input);
CShellCommand *ShellParse(CShellParser *parser);
U0 ShellCommandFree(CShellCommand *cmd);
U0 ShellParserFree(CShellParser *parser);

// ============================================================================
// FUNCTION DECLARATIONS - Execution
// ============================================================================

I64 ShellExecute(CShellState *sh, CShellCommand *cmd);
I64 ShellExecutePipeline(CShellState *sh, CShellPipeline *pipeline);
I64 ShellExecuteSimple(CShellState *sh, CShellSimpleCmd *cmd);

// ============================================================================
// FUNCTION DECLARATIONS - Expansion
// ============================================================================

U8 *ShellExpand(CShellState *sh, U8 *word);
U8 **ShellExpandWords(CShellState *sh, CShellWord *words, I64 *count);
U8 *ShellExpandVariable(CShellState *sh, U8 *name);
U8 **ShellExpandGlob(U8 *pattern, I64 *count);

// ============================================================================
// FUNCTION DECLARATIONS - Variables
// ============================================================================

U8 *ShellGetVar(CShellState *sh, U8 *name);
I64 ShellSetVar(CShellState *sh, U8 *name, U8 *value);
I64 ShellUnsetVar(CShellState *sh, U8 *name);
I64 ShellExportVar(CShellState *sh, U8 *name);

// ============================================================================
// FUNCTION DECLARATIONS - Functions
// ============================================================================

CShellFunc *ShellGetFunc(CShellState *sh, U8 *name);
I64 ShellDefineFunc(CShellState *sh, U8 *name, CShellCommand *body);
I64 ShellUndefFunc(CShellState *sh, U8 *name);

// ============================================================================
// FUNCTION DECLARATIONS - Builtins
// ============================================================================

I64 ShellBuiltinCd(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinEcho(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinPwd(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinExport(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinUnset(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinExit(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinRead(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinTest(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinSet(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinShift(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinSource(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinExec(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinEval(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinWait(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinKill(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinTrap(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinType(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinAlias(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinUnalias(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinJobs(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinFg(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinBg(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinTrue(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinFalse(CShellState *sh, I64 argc, U8 **argv);
I64 ShellBuiltinColon(CShellState *sh, I64 argc, U8 **argv);

// ============================================================================
// FUNCTION DECLARATIONS - Shell Control
// ============================================================================

CShellState *ShellNew();
U0 ShellFree(CShellState *sh);
I64 ShellRun(CShellState *sh, U8 *command);
I64 ShellRunFile(CShellState *sh, U8 *filename);
I64 ShellInteractive(CShellState *sh);

