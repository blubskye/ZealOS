# Full bzip2 Implementation Plan for ZealOS

## Overview

bzip2 is a block-sorting compressor that achieves excellent compression ratios. The algorithm consists of several stages that transform data to make it more compressible, then applies Huffman coding.

**Compression Pipeline:**
```
Input → RLE1 → BWT → MTF → RLE2 → Huffman → Output
```

**Decompression Pipeline:**
```
Input → Huffman → RLE2 → MTF → BWT⁻¹ → RLE1 → Output
```

---

## Algorithm Components

### 1. Run-Length Encoding (RLE) - Stage 1

**Purpose:** Reduce runs of identical bytes before BWT.

**Algorithm:**
- Replace runs of 4+ identical bytes with: `byte byte byte byte (count-4)`
- Count is 0-255, allowing runs up to 259 bytes
- Maximum run output: 5 bytes for 259 identical bytes

**Example:**
```
Input:  AAAAAAA (7 A's)
Output: AAAA\x03 (4 A's + count 3)
```

**Code structure:**
```c
class CRLE1State {
    U8 last_byte;
    I64 run_length;
};

I64 RLE1Encode(U8 *in, I64 in_len, U8 *out, I64 out_max);
I64 RLE1Decode(U8 *in, I64 in_len, U8 *out, I64 out_max);
```

### 2. Burrows-Wheeler Transform (BWT)

**Purpose:** Group similar bytes together, making data more compressible.

**Algorithm:**
1. Form all rotations of the input block
2. Sort rotations lexicographically
3. Output the last column of the sorted matrix
4. Record the row index of the original string

**Example:**
```
Input: "BANANA"

All rotations:       Sorted:           Last column:
BANANA              ANANA$B            B
ANANA$              BANANA$            $
NANA$B              NA$BANA            A
ANA$BA              NANA$BA            A
NA$BAN              $BANANA            A
A$BANA              ANA$BAN            N
$BANAN              A$BANAN            N

Output: "B$AAANN", index=3 (original "BANANA$" is row 3)
```

**Efficient Implementation:**

Instead of storing all rotations (O(n²) space), use suffix array:

```c
#define BWT_BLOCK_SIZE  900000  // 900KB default

class CBWTState {
    U8  *block;         // Input block
    I64 block_size;     // Size of block
    I64 *suffix_array;  // Sorted suffix indices
    I64 original_idx;   // Index of original string
};

// Suffix array construction using induced sorting (SA-IS) or qsort
U0 BWTBuildSuffixArray(CBWTState *state);

// Transform using suffix array
I64 BWTTransform(U8 *in, I64 in_len, U8 *out, I64 *orig_idx);

// Inverse transform (for decompression)
I64 BWTInverse(U8 *in, I64 in_len, U8 *out, I64 orig_idx);
```

**Suffix Array Sorting:**

For simplicity, use a comparison-based sort with careful handling:

```c
// Compare suffixes starting at positions i and j
I64 BWTCompareSuffixes(CBWTState *state, I64 i, I64 j)
{
    U8 *block = state->block;
    I64 n = state->block_size;

    for (I64 k = 0; k < n; k++) {
        U8 ci = block[(i + k) % n];
        U8 cj = block[(j + k) % n];
        if (ci != cj)
            return ci - cj;
    }
    return 0;
}

// Quicksort for suffix array
U0 BWTSortSuffixes(CBWTState *state, I64 lo, I64 hi);
```

**Inverse BWT (for decompression):**

```c
I64 BWTInverse(U8 *L, I64 n, U8 *out, I64 orig_idx)
{
    // Count occurrences of each byte
    I64 count[256] = {0};
    for (I64 i = 0; i < n; i++)
        count[L[i]]++;

    // Cumulative counts (first position of each byte in F column)
    I64 cumul[256];
    I64 sum = 0;
    for (I64 i = 0; i < 256; i++) {
        cumul[i] = sum;
        sum += count[i];
    }

    // Build transformation vector T
    // T[i] = position in F of the i-th occurrence of L[i]
    I64 *T = MAlloc(n * sizeof(I64));
    I64 occ[256] = {0};
    for (I64 i = 0; i < n; i++) {
        T[i] = cumul[L[i]] + occ[L[i]]++;
    }

    // Follow T from original index to reconstruct
    I64 idx = orig_idx;
    for (I64 i = n - 1; i >= 0; i--) {
        out[i] = L[idx];
        idx = T[idx];
    }

    Free(T);
    return n;
}
```

### 3. Move-to-Front Transform (MTF)

**Purpose:** Convert BWT output to smaller numbers (recently seen bytes become 0).

**Algorithm:**
1. Maintain a list of all 256 bytes in order
2. For each input byte, output its position in the list
3. Move that byte to the front of the list

**Example:**
```
Input:  B $ A A A N N
List:   [0,1,2,...,65=A,66=B,...,78=N,...,36=$,...]

B: position=66, output 66, list=[B,0,1,2,...A,...N,...$,...]
$: position=37, output 37, list=[$,B,0,1,...A,...N,...]
A: position=67, output 67, list=[A,$,B,0,1,...N,...]
A: position=0,  output 0,  list=[A,$,B,0,1,...N,...]  (A already at front)
A: position=0,  output 0
N: position=80, output 80, list=[N,A,$,B,0,1,...]
N: position=0,  output 0

Output: [66, 37, 67, 0, 0, 80, 0]
```

**Code:**
```c
I64 MTFEncode(U8 *in, I64 in_len, U8 *out)
{
    U8 list[256];
    for (I64 i = 0; i < 256; i++)
        list[i] = i;

    for (I64 i = 0; i < in_len; i++) {
        U8 byte = in[i];

        // Find position
        I64 pos = 0;
        while (list[pos] != byte)
            pos++;

        out[i] = pos;

        // Move to front
        for (I64 j = pos; j > 0; j--)
            list[j] = list[j-1];
        list[0] = byte;
    }

    return in_len;
}

I64 MTFDecode(U8 *in, I64 in_len, U8 *out)
{
    U8 list[256];
    for (I64 i = 0; i < 256; i++)
        list[i] = i;

    for (I64 i = 0; i < in_len; i++) {
        U8 pos = in[i];
        U8 byte = list[pos];

        out[i] = byte;

        // Move to front
        for (I64 j = pos; j > 0; j--)
            list[j] = list[j-1];
        list[0] = byte;
    }

    return in_len;
}
```

### 4. Run-Length Encoding - Stage 2 (Zero Runs)

**Purpose:** Encode runs of zeros (common after MTF) efficiently.

**Algorithm:**
- Zeros are encoded using RUNA (symbol 0) and RUNB (symbol 1) in binary
- Other symbols are shifted by 1
- Run length N is encoded as binary: N+1 in RUNA/RUNB symbols

**Example:**
```
Run of 5 zeros:
5 + 1 = 6 = binary 110
Read right-to-left with RUNA=0, RUNB=1: RUNB RUNA RUNA
But bzip2 uses: value = sum of (symbol+1) * 2^position
```

**Encoding scheme:**
```
0 zeros:  (nothing)
1 zero:   RUNA                    (1)
2 zeros:  RUNB                    (2)
3 zeros:  RUNA RUNA              (1 + 2)
4 zeros:  RUNB RUNA              (2 + 2)
5 zeros:  RUNA RUNB              (1 + 4)
...
```

```c
#define RUNA 0
#define RUNB 1

U0 RLE2EncodeZeros(I64 count, CBitWriter *out)
{
    while (count > 0) {
        if (count & 1) {
            BitWrite(out, RUNA, sym_bits);
            count--;
        } else {
            BitWrite(out, RUNB, sym_bits);
            count -= 2;
        }
        count >>= 1;
    }
}
```

### 5. Huffman Coding

**Purpose:** Variable-length encoding based on symbol frequency.

**bzip2 specifics:**
- Uses canonical Huffman codes
- Maximum code length: 20 bits
- Multiple Huffman tables per block (2-6 tables)
- Selector chooses which table for each 50-symbol group

**Structures:**
```c
#define HUFFMAN_MAX_SYMBOLS  258  // 256 bytes + RUNA + RUNB + EOB
#define HUFFMAN_MAX_TABLES   6
#define HUFFMAN_GROUP_SIZE   50

class CHuffmanTable {
    I64 code[HUFFMAN_MAX_SYMBOLS];   // Huffman codes
    I64 length[HUFFMAN_MAX_SYMBOLS]; // Code lengths
    I64 min_len, max_len;
};

class CHuffmanState {
    CHuffmanTable tables[HUFFMAN_MAX_TABLES];
    I64 num_tables;
    U8 *selectors;      // Which table for each group
    I64 num_selectors;
};
```

**Building Huffman table:**
```c
U0 HuffmanBuildTable(I64 *freq, I64 num_symbols, CHuffmanTable *table)
{
    // 1. Build initial code lengths using package-merge or similar
    // 2. Limit lengths to 20 bits
    // 3. Generate canonical codes from lengths

    // Sort symbols by code length, then by symbol value
    // Assign codes sequentially
}

U0 HuffmanGenerateCanonical(CHuffmanTable *table, I64 num_symbols)
{
    I64 code = 0;
    I64 last_len = 0;

    // Symbols sorted by (length, symbol)
    for (I64 i = 0; i < num_symbols; i++) {
        if (table->length[i] > last_len) {
            code <<= (table->length[i] - last_len);
            last_len = table->length[i];
        }
        table->code[i] = code++;
    }
}
```

---

## File Format

### bzip2 Stream Structure

```
Stream     = StreamHeader Block+ StreamFooter
StreamHeader = 'BZ' 'h' BlockSize
Block      = BlockHeader BlockData
BlockHeader = Magic(48 bits) + CRC(32) + Randomized(1) + OrigPtr(24)
             + MapL1(16) + MapL2(variable) + NumTables(3) + NumSelectors(15)
             + Selectors(variable) + HuffmanTables(variable)
BlockData  = HuffmanCodedSymbols
StreamFooter = Magic(48 bits) + CombinedCRC(32)

BlockMagic    = 0x314159265359 (BCD for pi)
StreamEndMagic = 0x177245385090 (BCD for sqrt(pi))
```

### Block Size

The digit after 'BZh' indicates block size:
- '1' = 100KB
- '9' = 900KB (default, best compression)

---

## Implementation Structure

### Header File: bzip2_impl.HH

```c
// bzip2_impl.HH - Internal bzip2 implementation

#define BZIP2_BLOCK_100K    100000
#define BZIP2_BLOCK_900K    900000

#define BZIP2_MAX_ALPHA_SIZE  258
#define BZIP2_MAX_CODE_LEN    23
#define BZIP2_RUNA            0
#define BZIP2_RUNB            1
#define BZIP2_N_GROUPS        6
#define BZIP2_G_SIZE          50
#define BZIP2_N_ITERS         4
#define BZIP2_MAX_SELECTORS   18002

// CRC32 (reuse from gzip)
extern U32 GzipCrc32(U32 crc, U8 *buf, I64 len);

// Bit I/O
class CBitReader {
    FILE    *fp;
    U32     buffer;
    I64     bits_in_buffer;
};

class CBitWriter {
    FILE    *fp;
    U32     buffer;
    I64     bits_in_buffer;
};

// BWT state
class CBWTState {
    U8      *block;
    I64     block_size;
    I64     *suffix_array;
    I64     *quadrant;      // For faster comparison
    I64     original_ptr;
    I64     budget;         // For fallback sort
};

// Huffman state
class CHuffmanState {
    I64     len[BZIP2_N_GROUPS][BZIP2_MAX_ALPHA_SIZE];
    I64     code[BZIP2_N_GROUPS][BZIP2_MAX_ALPHA_SIZE];
    I64     rfreq[BZIP2_N_GROUPS][BZIP2_MAX_ALPHA_SIZE];
    U8      selector[BZIP2_MAX_SELECTORS];
    I64     num_selectors;
};

// Main compression state
class CBzip2Compress {
    // Input
    U8      *in_buf;
    I64     in_pos;
    I64     in_len;

    // Block buffer
    U8      *block;
    I64     block_size;
    I64     block_pos;
    I64     block_level;    // 1-9

    // BWT
    CBWTState bwt;

    // MTF output
    U16     *mtf_values;
    I64     mtf_len;

    // Huffman
    CHuffmanState huff;

    // Output
    CBitWriter out;

    // CRC
    U32     block_crc;
    U32     combined_crc;
};

// Main decompression state
class CBzip2Decompress {
    CBitReader in;

    U8      *block;
    I64     block_size;
    I64     block_level;

    I64     original_ptr;
    U32     stored_block_crc;
    U32     stored_combined_crc;

    CHuffmanState huff;

    // Decode state
    I64     *tt;            // Transformation table for inverse BWT
    I64     t_pos;
    I64     k0;
    I64     n_block_used;

    U32     calculated_block_crc;
    U32     calculated_combined_crc;
};
```

### Main Implementation: bzip2_impl.ZC

```c
// bzip2_impl.ZC - Full bzip2 implementation

// ============================================================================
// BIT I/O
// ============================================================================

U0 BitWriterInit(CBitWriter *bw, FILE *fp)
{
    bw->fp = fp;
    bw->buffer = 0;
    bw->bits_in_buffer = 0;
}

U0 BitWrite(CBitWriter *bw, U32 value, I64 bits)
{
    while (bits > 0) {
        I64 room = 8 - bw->bits_in_buffer;
        if (bits <= room) {
            bw->buffer = (bw->buffer << bits) | value;
            bw->bits_in_buffer += bits;
            bits = 0;
        } else {
            bw->buffer = (bw->buffer << room) | (value >> (bits - room));
            bw->bits_in_buffer = 8;
            value &= (1 << (bits - room)) - 1;
            bits -= room;
        }

        if (bw->bits_in_buffer == 8) {
            fputc(bw->buffer, bw->fp);
            bw->buffer = 0;
            bw->bits_in_buffer = 0;
        }
    }
}

U0 BitWriterFlush(CBitWriter *bw)
{
    if (bw->bits_in_buffer > 0) {
        bw->buffer <<= (8 - bw->bits_in_buffer);
        fputc(bw->buffer, bw->fp);
    }
}

U0 BitReaderInit(CBitReader *br, FILE *fp)
{
    br->fp = fp;
    br->buffer = 0;
    br->bits_in_buffer = 0;
}

U32 BitRead(CBitReader *br, I64 bits)
{
    U32 result = 0;

    while (bits > 0) {
        if (br->bits_in_buffer == 0) {
            I64 c = fgetc(br->fp);
            if (c < 0) c = 0;
            br->buffer = c;
            br->bits_in_buffer = 8;
        }

        I64 take = (bits < br->bits_in_buffer) ? bits : br->bits_in_buffer;
        result = (result << take) | (br->buffer >> (br->bits_in_buffer - take));
        br->buffer &= (1 << (br->bits_in_buffer - take)) - 1;
        br->bits_in_buffer -= take;
        bits -= take;
    }

    return result;
}

// ============================================================================
// RLE STAGE 1
// ============================================================================

I64 RLE1Encode(U8 *in, I64 in_len, U8 *out)
{
    I64 out_pos = 0;
    I64 i = 0;

    while (i < in_len) {
        U8 byte = in[i];
        I64 run = 1;

        while (i + run < in_len && in[i + run] == byte && run < 259)
            run++;

        if (run >= 4) {
            // Output 4 copies + count
            out[out_pos++] = byte;
            out[out_pos++] = byte;
            out[out_pos++] = byte;
            out[out_pos++] = byte;
            out[out_pos++] = run - 4;
            i += run;
        } else {
            out[out_pos++] = byte;
            i++;
        }
    }

    return out_pos;
}

I64 RLE1Decode(U8 *in, I64 in_len, U8 *out)
{
    I64 out_pos = 0;
    I64 i = 0;

    while (i < in_len) {
        U8 byte = in[i++];
        out[out_pos++] = byte;

        // Check for run of 4
        I64 run = 1;
        while (run < 4 && i < in_len && in[i] == byte) {
            out[out_pos++] = byte;
            run++;
            i++;
        }

        if (run == 4 && i < in_len) {
            // Read count
            I64 extra = in[i++];
            for (I64 j = 0; j < extra; j++)
                out[out_pos++] = byte;
        }
    }

    return out_pos;
}

// ============================================================================
// BURROWS-WHEELER TRANSFORM
// ============================================================================

// Simple suffix array construction using quicksort
// For production, use SA-IS algorithm for O(n) construction

I64 bwt_block_size;
U8 *bwt_block;

I64 BWTCompareSuffix(I64 *a, I64 *b)
{
    I64 i = *a, j = *b;
    I64 n = bwt_block_size;

    for (I64 k = 0; k < n; k++) {
        I64 ci = bwt_block[(i + k) % n];
        I64 cj = bwt_block[(j + k) % n];
        if (ci != cj)
            return ci - cj;
    }
    return 0;
}

I64 BWTTransform(U8 *in, I64 n, U8 *out, I64 *orig_idx)
{
    // Allocate suffix array
    I64 *sa = MAlloc(n * sizeof(I64));
    for (I64 i = 0; i < n; i++)
        sa[i] = i;

    // Sort suffix array
    bwt_block = in;
    bwt_block_size = n;
    qsort(sa, n, sizeof(I64), &BWTCompareSuffix);

    // Output last column and find original
    *orig_idx = 0;
    for (I64 i = 0; i < n; i++) {
        out[i] = in[(sa[i] + n - 1) % n];
        if (sa[i] == 0)
            *orig_idx = i;
    }

    Free(sa);
    return n;
}

I64 BWTInverse(U8 *L, I64 n, U8 *out, I64 orig_idx)
{
    // Count occurrences
    I64 count[256];
    MemSet(count, 0, sizeof(count));
    for (I64 i = 0; i < n; i++)
        count[L[i]]++;

    // Cumulative counts
    I64 cumul[256];
    I64 sum = 0;
    for (I64 i = 0; i < 256; i++) {
        cumul[i] = sum;
        sum += count[i];
    }

    // Build T vector
    I64 *T = MAlloc(n * sizeof(I64));
    I64 occ[256];
    MemSet(occ, 0, sizeof(occ));
    for (I64 i = 0; i < n; i++) {
        T[i] = cumul[L[i]] + occ[L[i]]++;
    }

    // Reconstruct
    I64 idx = orig_idx;
    for (I64 i = n - 1; i >= 0; i--) {
        out[i] = L[idx];
        idx = T[idx];
    }

    Free(T);
    return n;
}

// ============================================================================
// MOVE-TO-FRONT
// ============================================================================

I64 MTFEncode(U8 *in, I64 n, U8 *out)
{
    U8 list[256];
    for (I64 i = 0; i < 256; i++)
        list[i] = i;

    for (I64 i = 0; i < n; i++) {
        U8 byte = in[i];
        I64 pos = 0;
        while (list[pos] != byte)
            pos++;

        out[i] = pos;

        // Move to front
        while (pos > 0) {
            list[pos] = list[pos - 1];
            pos--;
        }
        list[0] = byte;
    }

    return n;
}

I64 MTFDecode(U8 *in, I64 n, U8 *out)
{
    U8 list[256];
    for (I64 i = 0; i < 256; i++)
        list[i] = i;

    for (I64 i = 0; i < n; i++) {
        I64 pos = in[i];
        U8 byte = list[pos];
        out[i] = byte;

        while (pos > 0) {
            list[pos] = list[pos - 1];
            pos--;
        }
        list[0] = byte;
    }

    return n;
}

// ============================================================================
// HUFFMAN CODING
// ============================================================================

// ... (extensive Huffman implementation)
// See separate section below

// ============================================================================
// BLOCK COMPRESSION
// ============================================================================

I64 Bzip2CompressBlock(CBzip2Compress *state)
{
    I64 n = state->block_pos;
    if (n == 0) return 0;

    U8 *block = state->block;

    // Calculate block CRC
    state->block_crc = GzipCrc32(0, block, n);
    state->combined_crc = ((state->combined_crc << 1) |
                           (state->combined_crc >> 31)) ^ state->block_crc;

    // Stage 1: RLE
    U8 *rle1_out = MAlloc(n + n/4 + 100);
    I64 rle1_len = RLE1Encode(block, n, rle1_out);

    // Stage 2: BWT
    U8 *bwt_out = MAlloc(rle1_len);
    I64 orig_ptr;
    BWTTransform(rle1_out, rle1_len, bwt_out, &orig_ptr);
    Free(rle1_out);

    // Stage 3: MTF
    U8 *mtf_out = MAlloc(rle1_len);
    MTFEncode(bwt_out, rle1_len, mtf_out);
    Free(bwt_out);

    // Stage 4: RLE2 + Huffman (write to output)
    // ... write block header
    BitWrite(&state->out, 0x314159265359ULL >> 24, 24);
    BitWrite(&state->out, 0x314159265359ULL & 0xFFFFFF, 24);
    BitWrite(&state->out, state->block_crc, 32);
    BitWrite(&state->out, 0, 1);  // not randomized
    BitWrite(&state->out, orig_ptr, 24);

    // ... encode with Huffman
    // (Complex: involves creating multiple Huffman tables,
    //  selecting best table for each 50-symbol group, etc.)

    Free(mtf_out);
    return 0;
}

// ============================================================================
// MAIN COMPRESS FUNCTION
// ============================================================================

I64 Bzip2CompressFull(U8 *infile, U8 *outfile, I64 level)
{
    FILE *fin = fopen(infile, "rb");
    if (!fin) return -1;

    FILE *fout = fopen(outfile, "wb");
    if (!fout) {
        fclose(fin);
        return -1;
    }

    CBzip2Compress state;
    MemSet(&state, 0, sizeof(CBzip2Compress));

    I64 block_size = level * 100000;
    state.block = MAlloc(block_size);
    state.block_level = level;
    BitWriterInit(&state.out, fout);

    // Write stream header
    fputc('B', fout);
    fputc('Z', fout);
    fputc('h', fout);
    fputc('0' + level, fout);

    // Process blocks
    while (1) {
        state.block_pos = fread(state.block, 1, block_size, fin);
        if (state.block_pos == 0)
            break;

        Bzip2CompressBlock(&state);
    }

    // Write stream footer
    BitWrite(&state.out, 0x177245385090ULL >> 24, 24);
    BitWrite(&state.out, 0x177245385090ULL & 0xFFFFFF, 24);
    BitWrite(&state.out, state.combined_crc, 32);
    BitWriterFlush(&state.out);

    Free(state.block);
    fclose(fin);
    fclose(fout);

    return 0;
}
```

---

## Implementation Phases

### Phase 1: Core Algorithms (~500 lines)
- [x] Bit I/O (BitReader, BitWriter)
- [ ] RLE Stage 1 (encode/decode)
- [ ] BWT Transform (simple quicksort version)
- [ ] BWT Inverse
- [ ] MTF Transform (encode/decode)

### Phase 2: Huffman Coding (~600 lines)
- [ ] Build Huffman table from frequencies
- [ ] Generate canonical codes
- [ ] Encode symbols
- [ ] Decode symbols
- [ ] Multiple table support (2-6 tables)
- [ ] Selector encoding/decoding

### Phase 3: RLE Stage 2 (~200 lines)
- [ ] Zero-run encoding with RUNA/RUNB
- [ ] Zero-run decoding
- [ ] Symbol remapping (+1 for non-zeros)

### Phase 4: Block Handling (~400 lines)
- [ ] Block header parsing/writing
- [ ] CRC calculation
- [ ] Symbol map handling
- [ ] Combined CRC tracking

### Phase 5: Stream Handling (~200 lines)
- [ ] Stream header parsing/writing
- [ ] Multi-block support
- [ ] End-of-stream handling
- [ ] Error detection

### Phase 6: Optimization (~300 lines)
- [ ] Faster BWT using radix sort or SA-IS
- [ ] Optimized MTF with linked list
- [ ] Better Huffman table selection
- [ ] Memory usage optimization

**Total: ~2,200 lines**

---

## Testing

### Unit Tests

```c
// Test RLE1
U0 TestRLE1()
{
    U8 in[] = "AAAAAABBBBCCCCCCCC";
    U8 out[100], decoded[100];

    I64 enc_len = RLE1Encode(in, 18, out);
    I64 dec_len = RLE1Decode(out, enc_len, decoded);

    assert(dec_len == 18);
    assert(memcmp(in, decoded, 18) == 0);
}

// Test BWT
U0 TestBWT()
{
    U8 in[] = "BANANA";
    U8 bwt_out[6], decoded[6];
    I64 orig_idx;

    BWTTransform(in, 6, bwt_out, &orig_idx);
    BWTInverse(bwt_out, 6, decoded, orig_idx);

    assert(memcmp(in, decoded, 6) == 0);
}

// Test MTF
U0 TestMTF()
{
    U8 in[] = {65, 65, 66, 65, 67, 67};  // AABAC C
    U8 mtf_out[6], decoded[6];

    MTFEncode(in, 6, mtf_out);
    MTFDecode(mtf_out, 6, decoded);

    assert(memcmp(in, decoded, 6) == 0);
}
```

### Integration Tests

```bash
# Compress and decompress, compare with original
./bzip2 -k testfile.txt
./bzip2 -dk testfile.txt.bz2
diff testfile.txt testfile.txt.out

# Compare with reference bzip2
bzip2 -k reference.txt
./bzip2 -d reference.txt.bz2 -c > our_output.txt
diff reference.txt our_output.txt
```

---

## References

1. [bzip2 and libbzip2 Manual](https://sourceware.org/bzip2/manual/manual.html)
2. [Burrows-Wheeler Transform](https://en.wikipedia.org/wiki/Burrows%E2%80%93Wheeler_transform)
3. [Move-to-front Transform](https://en.wikipedia.org/wiki/Move-to-front_transform)
4. [bzip2 Source Code](https://sourceware.org/git/?p=bzip2.git)
5. [SA-IS Algorithm](https://sites.google.com/site/yaborisk/suffix-array-algorithm)
