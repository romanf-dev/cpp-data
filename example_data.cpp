//
// Example binary data.
//

constexpr struct
{
    unsigned int a;
    unsigned int b;
}
pairs[] = 
{
    {1,2},
    {3,4},
    {5,6}
};

constexpr unsigned int arr[] = {1,2,3,4,5};

constexpr Node nested_folder[] = 
{
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
};

constexpr Node bar[] = 
{
    {"my_array", arr},
    {"pairs", pairs},
    {"nested", nested_folder}
};

constexpr Node foo[] = 
{
    {"number", 5},
    {"string", "fffff"},
    {"folder", bar}
};

SET_ROOT(foo);

