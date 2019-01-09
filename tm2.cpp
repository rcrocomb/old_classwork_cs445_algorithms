#include <iostream>
    using std::cout;
#include <deque>
#include <vector>
#include <utility>
#include <assert.h>

typedef unsigned int data_t;

typedef std::deque<data_t> tape_t;

unsigned int n;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

bool is_full(tape_t *t);
bool is_end(tape_t *t1);
int read(tape_t *t1, tape_t *t2, data_t *d);
void write(data_t data, tape_t *t1, tape_t *t2);
void rewind(tape_t *tape);
bool is_sorted (tape_t *t1, tape_t *t2);
void sort_3(data_t *a, data_t *b, data_t *c);

void print(tape_t *t1, tape_t *t2);
void print_all(data_t x,
               data_t y,
               data_t z,
               tape_t *s1,
               tape_t *s2,
               tape_t *d1,
               tape_t *d2);

void sort(tape_t *t1, tape_t *t2, tape_t *t3, tape_t *t4);




////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/*!
    Might have to jiggle if 'n' is odd.

    Returns true if tape is full.

    is_full() and is_end() together form the proper IsEnd() of the
    problem.  It's just clearer to me for the simulated tape to have
    two routines.

*/

bool
is_full(tape_t *t)
{
    return t->size() == n / 2;
}

/*!
    You're at the end of the tape if:

        a) Read all data off tape
        b) Written tape until it's full

    I stuck the 'b' sense in is_full().
*/

bool
is_end(tape_t *t)
{
    return t->empty();
}

/*!
    For my simulated tape a read is destructive: for a deque it's
    annoying to write over previous values.  The ultimate behavior is
    the same as you'd get with the tape in the problem.
*/

int
read(tape_t *t1, tape_t *t2, data_t *d)
{
    int got_data = 0;

    if (!is_end(t1))
    {
        *d = t1->front();
        t1->pop_front();
        got_data = 1;
    } else if (!is_end(t2))
    {
        *d = t2->front();
        t2->pop_front();
        got_data = 1;
    }
    return got_data;
}

/*!
    Put value 'data' on one of t1 or t2: treat them as a single,
    longer tape of size 'n'.  Having both tapes full makes us 'splode.
*/

void
write(tape_t *t1, tape_t *t2, data_t data)
{
    assert((is_full(t1) && is_full(t2)) || "both tapes full");

    if (!is_full(t1))
        t1->push_back(data);
    else
        t2->push_back(data);
}

/*!
    There's nothing to do here, since I'm only modeling the tape.
    This is just so you can see where a rewind would be placed.

    The goober code is so that I don't get a warning about an unused
    variable.

    Uhm, I consider rewind()s to be O(c).
*/

void
rewind(tape_t *tape)
{
    if (!tape)
        cout << "Poopers\n";
}

/*!
    This is O(n).
*/

bool
is_sorted (tape_t *t1, tape_t *t2)
{
    rewind(t1);
    rewind(t2);

    std::vector<data_t> v;
    v.insert(v.end(), t1->begin(), t1->end());
    v.insert(v.end(), t2->begin(), t2->end());

    data_t monotonic = v[0];
    unsigned int size = v.size();
    for (unsigned i = 1; i < size; ++i)
    {
        if (v[i] >= monotonic)  // not strictly monotonic: can have multiple
            monotonic = v[i];
        else
            return false;
    }
    return true;
}

/*!
    Puts data pointed to by 'a', 'b', 'c' in sorted order: smallest in
    'a', etc.
*/

void
sort_3(data_t *a, data_t *b, data_t *c)
{
    data_t x = *a;
    data_t y = *b;
    data_t z = *c;

    if (z < y)
        std::swap(y,z);

    if (y < x)
        std::swap(y,x);

    if (z < y)
        std::swap(z, y);

    *a = x;
    *b = y;
    *c = z;
}

void
print(tape_t *t1, tape_t *t2)
{
    if (t1->empty())
        cout << "empty ";
    else
    {
        tape_t::const_iterator i = t1->begin();
        const tape_t::const_iterator e = t1->end();
        for ( ; i != e; ++i)
            cout << *i << " ";
    }

    cout << " | ";

    if (t2->empty())
        cout << "empty";
    else
    {
        tape_t::const_iterator i = t2->begin();
        const tape_t::const_iterator e = t2->end();
        for ( ; i != e; ++i)
            cout << *i << " ";
    }
}

void
print_all
(
    data_t x,
    data_t y,
    data_t z,
    tape_t *s1,
    tape_t *s2,
    tape_t *d1,
    tape_t *d2
)
{
    cout << "x " << x << " y " << y << " z " << z << "\t\t";
    print(s1, s2);
    cout << "\t\t";
    print(d1, d2);
    cout << "\n";
}


/*!
    Does the real work.
*/

void
sort
(
    tape_t *t1,
    tape_t *t2,
    tape_t *t3,
    tape_t *t4
)
{
    tape_t *source1 = t1;
    tape_t *source2 = t2;
    tape_t *dest1 = t3;
    tape_t *dest2 = t4;

    unsigned int x, y, z;
    unsigned int count = 17;    // whatever, as long as > 3
    x = y = z = 0;

    // small "hand-coded" sorts for n <= 3
    if (n < 4)
    {
        count = 0;
        count += read(source1, source2, &x);
        count += read(source1, source2, &y);
        count += read(source1, source2, &z);
    }

    switch (count)
    {
    case 0:
        // no elements to sort
        print(dest1, dest2);
        return;

    case 1:
        // sorted already;
        write(dest1, dest2, x);
        print(dest1, dest2);
        return;

    case 2:
        if (x < y)
        {
            write(dest1, dest2, x);
            write(dest1, dest2, y);
        } else
        {
            write(dest1, dest2, y);
            write(dest1, dest2, x);
        }

        print(dest1, dest2);
        return;

    case 3:
        sort_3(&x, &y, &z);
        write(dest1, dest2, x);
        write(dest1, dest2, y);
        write(dest1, dest2, z);
        return;
    }

    print_all(x, y, z, source1, source2, dest1, dest2);

    // "big" routine needs tweaking for odd numbers: n / 2 not so good.

    while (!is_sorted(source1, source2))
    {
        count = 0;
        read(source1, source2, &x);
        read(source1, source2, &y);
        read(source1, source2, &z);
        sort_3(&x, &y, &z);

 //     print_all(x, y, z, source1, source2, dest1, dest2);

        // x < y < z
        write(dest1, dest2, x);
//      print_all(x, y, z, source1, source2, dest1, dest2);

        // read until source tapes are empty, always writing smallest
        // value into destination tapes
        while (read(source1, source2, &x))
        {
            // puts smallest of x, y, z into x
            sort_3(&x, &y, &z);
//          print_all(x, y, z, source1, source2, dest1, dest2);
            write(dest1, dest2, x);
//          print_all(x, y, z, source1, source2, dest1, dest2);
        }
        // two values remain: in 'y' and 'z', with y < z: write them out.
        write(dest1, dest2, y);
        write(dest1, dest2, z);

        print_all(x, y, z, source1, source2, dest1, dest2);

        // source tapes are empty: switch source and dest pointers
        std::swap(source1, dest1);
        std::swap(source2, dest2);

        // Another pass (over n elements) complete: rewind tapes
        rewind(source1);
        rewind(source2);
        rewind(dest1);
        rewind(dest2);
    }

    cout << "\n\nSorted list:    ";
    print(source1, source2);
    cout << "\n\n\n";
}

int
main(int argc, char *argv[])
{
    tape_t t1;
    tape_t t2;
    tape_t t3;
    tape_t t4;

    // fill the tape
    n = 8;
    write(&t1, &t2, 1);
    write(&t1, &t2, 19);
    write(&t1, &t2, 17);
    write(&t1, &t2, 3);
    write(&t1, &t2, 56);
    write(&t1, &t2, 42);
    write(&t1, &t2, 5);
    write(&t1, &t2, 18);

    sort(&t1, &t2, &t3, &t4);
    return 0;
}
