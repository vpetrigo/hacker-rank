import sys

def binary_search(arr, value):
    left, right = 0, len(arr)

    while (left < right):
        mid = left + (right - left) // 2

        if arr[mid] == value:
            return mid
        elif arr[mid] > value:
            right = mid
        else:
            left = mid + 1

    return -1

input_data = (tuple(map(int, line.split())) for line in sys.stdin)
# value for search
[V] = next(input_data)
# size of an array
[n] = next(input_data)
# sorted array
[arr] = list(input_data)
assert len(arr) == n, "Wrong size of an input array"
index = binary_search(arr, V)
print(index)
