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
