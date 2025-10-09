# A basic_set
{ [i,j] : 0 < i < 10 and 0 < j < 10 and i < j }

# A set
{ [i, j] : 0 <= i <= 10 and 0 <= j <= 10 and (j > i or (6 <= i <= 9 and 0 < j <= 4)) }

# A union_set
{ A[i, j] : 0 <= i <= 10 and 0 <= j <= 10 ; B[i, j, k] : 6 <= i <= 9 and 0 < j <= 4 and 0 <= k < 2 }

# a basic_map
{
    [i, j] -> [a, b] : i = a + 1 and (b + j) mod 2 = 0
    and 0 <= i < 10 and 0 <= j < 10
    and 0 <= a < 10 and 0 <= b < 10
}

# a symbolic constant
[N] -> { [i, j] : 0 < i < N and j > i and 0 < j < N }


# an example that code-gens with an if guard
{ [i0, i1] -> [ct, slot] : (i0 - i1 + ct) mod 16 = 0 and (-i0 + slot) mod 16 = 0 and 0 <= i0 <= 9 and 0 <= i1 <= 15 and 0 <= ct <= 15 and 0 <= slot <= 1023 }


# An example of a 3d tensor mapping to vector registers
{
    S[i, j, k] -> [reg, lane] :
        0 <= i < 32
    and 0 <= j < 64
    and 0 <= k < 1024
    and 0 <= lane < 16
    and i + j * 32 + k * 32 * 64 = 16 * reg + lane
}
