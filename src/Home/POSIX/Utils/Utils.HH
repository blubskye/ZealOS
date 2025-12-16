// Utils.HH - POSIX Utilities Definitions for ZealOS
// Part of ZealOS POSIX compatibility layer
//
// These are NOFORK utilities that can run in-process

// ============================================================================
// UTILITY FUNCTION TYPE
// ============================================================================

typedef I64 (*PosixUtilMain)(I64 argc, U8 **argv);

// ============================================================================
// UTILITY REGISTRY
// ============================================================================

class CPosixUtil
{
	U8 *name;
	PosixUtilMain main;
	U8 *description;
};

// Find and run a utility
I64 PosixUtilRun(U8 *name, I64 argc, U8 **argv);

// ============================================================================
// UTILITY DECLARATIONS
// ============================================================================

// File utilities
I64 UtilCat(I64 argc, U8 **argv);
I64 UtilHead(I64 argc, U8 **argv);
I64 UtilTail(I64 argc, U8 **argv);
I64 UtilWc(I64 argc, U8 **argv);
I64 UtilCp(I64 argc, U8 **argv);
I64 UtilMv(I64 argc, U8 **argv);
I64 UtilRm(I64 argc, U8 **argv);
I64 UtilMkdir(I64 argc, U8 **argv);
I64 UtilRmdir(I64 argc, U8 **argv);
I64 UtilLs(I64 argc, U8 **argv);
I64 UtilTouch(I64 argc, U8 **argv);
I64 UtilChmod(I64 argc, U8 **argv);
I64 UtilLn(I64 argc, U8 **argv);

// Text utilities
I64 UtilEcho(I64 argc, U8 **argv);
I64 UtilPrintf(I64 argc, U8 **argv);
I64 UtilCut(I64 argc, U8 **argv);
I64 UtilTr(I64 argc, U8 **argv);
I64 UtilSort(I64 argc, U8 **argv);
I64 UtilUniq(I64 argc, U8 **argv);
I64 UtilGrep(I64 argc, U8 **argv);
I64 UtilSed(I64 argc, U8 **argv);
I64 UtilTee(I64 argc, U8 **argv);

// System utilities
I64 UtilBasename(I64 argc, U8 **argv);
I64 UtilDirname(I64 argc, U8 **argv);
I64 UtilEnv(I64 argc, U8 **argv);
I64 UtilExpr(I64 argc, U8 **argv);
I64 UtilSeq(I64 argc, U8 **argv);
I64 UtilSleep(I64 argc, U8 **argv);
I64 UtilTrue(I64 argc, U8 **argv);
I64 UtilFalse(I64 argc, U8 **argv);
I64 UtilYes(I64 argc, U8 **argv);
I64 UtilDate(I64 argc, U8 **argv);
I64 UtilUname(I64 argc, U8 **argv);
I64 UtilWhoami(I64 argc, U8 **argv);
I64 UtilHostname(I64 argc, U8 **argv);
I64 UtilId(I64 argc, U8 **argv);
I64 UtilPwd(I64 argc, U8 **argv);
I64 UtilClear(I64 argc, U8 **argv);

// Comparison utilities
I64 UtilCmp(I64 argc, U8 **argv);
I64 UtilDiff(I64 argc, U8 **argv);

// Archive utilities
I64 UtilXxd(I64 argc, U8 **argv);
I64 UtilOd(I64 argc, U8 **argv);
I64 UtilHexdump(I64 argc, U8 **argv);
