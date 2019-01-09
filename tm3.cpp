#include <iostream>
using std::cout;
#include <deque>
#include <vector>
#include <utility>                      // std::swap()

#include <assert.h>                     // assert()
#include <stdlib.h>                     // drand48(), atoi()

typedef unsigned int data_t;
typedef std::vector<data_t> v_data_t;
typedef std::deque<data_t> tape_t;

namespace
{
    unsigned int n;                     // global

    enum
    {
        HAIL_ERIS   = 17,
        ITERATIONS  = 1,
        MAX_N       = 42,
        MAX_VALUE   = 169
    };

    enum test_type { MANUAL, AUTOMATIC };

    #define RAND(a,b) static_cast<a>(drand48() * (b))
}

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

bool is_full(tape_t *t);
bool is_end(tape_t *t1);
bool read(tape_t *t, data_t *d);
int read(tape_t *t1, tape_t *t2, data_t *d);
void write(tape_t *t, data_t data);
void write(data_t data, tape_t *t1, tape_t *t2);
void rewind(tape_t *tape);
bool is_sorted (tape_t *t1, tape_t *t2);

void print_single(tape_t *t);
void print(tape_t *t1, tape_t *t2);
void print_all(data_t x,
               data_t y,
               tape_t *s1,
               tape_t *s2,
               tape_t *d1,
               tape_t *d2);

bool write_data(tape_t **to_write,
                tape_t *d1,
                tape_t *d2,
                tape_t *source,
                data_t *data,
                bool cross);

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

bool
read(tape_t *t, data_t *d)
{
    bool got_data = false;
    if (!is_end(t))
    {
        *d = t->front();
        t->pop_front();
        got_data = true;
    }
    return got_data;
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

void
write(tape_t *t, data_t data)
{
    assert(t && !is_full(t));
    t->push_back(data);
}

/*!
    Put value 'data' on one of t1 or t2: treat them as a single,
    longer tape of size 'n'.  Having both tapes full makes us 'splode.
*/

void
write(tape_t *t1, tape_t *t2, data_t data)
{
    assert(!is_full(t1) || !is_full(t2));

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

    v_data_t v;
    v.insert(v.end(), t1->begin(), t1->end());
    v.insert(v.end(), t2->begin(), t2->end());

    data_t monotonic = v[0];
    unsigned int size = v.size();
    for (unsigned i = 1; i < size; ++i)
    {
        if (v[i] >= monotonic)  // not strictly monotonic: can have
            monotonic = v[i];   // several of the same value
        else
            return false;
    }
    return true;
}

/*!
    Contents of a single tape.
*/

void
print_single(tape_t *t)
{
    if (t->empty())
        cout << "empty ";
    else
    {
        tape_t::const_iterator i = t->begin();
        const tape_t::const_iterator e = t->end();
        for ( ; i != e; ++i)
            cout << *i << " ";
    }

}

void
print(tape_t *t1, tape_t *t2)
{
    print_single(t1);
    cout << " | ";
    print_single(t2);
}

void
print_all
(
    data_t x,
    data_t y,
    tape_t *s1,
    tape_t *s2,
    tape_t *d1,
    tape_t *d2
)
{
    cout << "\nx: " << x << " y: " << y << "\n";
    print(s1, s2);
    cout << "\t\t";
    print(d1, d2);
    cout << "\n";
}


bool
write_data
(
    tape_t **to_write,
    tape_t *d1,
    tape_t *d2,
    tape_t *source,
    tape_t *alternate,
    data_t *data,
    bool cross
)
{
    assert(to_write);
    assert(*to_write);
    assert(d1);
    assert(d2);
    assert(source);
    assert(data);

    data_t d = *data;
    bool got_data = false;

    if (cross)
    {
        // if on 'cross', smallest value goes on opposite of last tape
        // we wrote to
        write(*to_write, d);

        // Here is what switches destination tapes
        *to_write = ((*to_write) == d1 ? d2 : d1);
        // Need a new value: read next element from tape
        got_data = read(source, data);
        if (!got_data)
        {
            got_data = read(alternate, &d);
            while (got_data)
            {
                write(d1, d2, d);
                got_data = read(alternate, &d);
            }
        }
    } else
    {
        // else just write linearly along tape
        write(d1, d2, d);
        // Need a new x: read next element from tape where
        // we got x: this is source1
        got_data = read(source, data);
        if (!got_data)
        {
            got_data = read(alternate, &d);
            while (got_data)
            {
                write(d1, d2, d);
                got_data = read(alternate, &d);
            }
        }
    }
    return got_data;
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
    tape_t *to_write = t3;

    unsigned int x, y;
    unsigned int count = 17;    // whatever, as long as > 3
    bool cross = true, got_data = false;
    x = y = 0;

    // small "hand-coded" sorts for n < 3
    if (n < 3)
    {
        count = 0;
        count += read(source1, source2, &x);
        count += read(source1, source2, &y);
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
        print_all(x, y, source1, source2, dest1, dest2);
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
    }

    print_all(x, y, source1, source2, dest1, dest2);

    // "big" routine needs tweaking for odd numbers: n / 2 not so good.
    count = 0;
    while (!is_sorted(source1, source2))
    {
        // read initial 2 elements: 1 from each source array
        got_data = read(source1, &x) && read(source2, &y);
        // Damn well better not be empty on 1st pass (caught above)
        while (got_data)
        {
            if (x < y)
                write_data(&to_write, dest1, dest2, source1, source2, &x, cross);
            else
                write_data(&to_write, dest1, dest2, source2, source1, &y, cross);

            print_all(x, y, source1, source2, dest1, dest2);
        }

        // source tapes are empty: switch source and dest pointers
        std::swap(source1, dest1);
        std::swap(source2, dest2);

        // Another pass (over n elements) complete: rewind tapes
        rewind(source1);
        rewind(source2);
        rewind(dest1);
        rewind(dest2);
        ++count;
        cross = !cross;
    }

    cout << "\n\nIn " << count << " passes: ";
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

    test_type bob = AUTOMATIC;

    switch (bob)
    {

    case MANUAL:

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

        break;

    case AUTOMATIC:
    {
        if (argv[1])
            n = atoi(argv[1]);
        else
        {
            while (!n)              // sometimes drand48() returns 0.  Boring.
            {
                // generate a random length for the array
                n = RAND(unsigned int, MAX_N);
                if (n & 1)          // 'n' odd.  Details: blah.  Make even.
                    ++n;
            }
        }

        cout << "n == '" << n << "'\n";

        for (unsigned int i = 0; i < ITERATIONS; ++i)
        {
            cout << "\nIteration " << i << " of " << ITERATIONS << "\n";
            t1.clear();
            t2.clear();
            t3.clear();
            t4.clear();

            // generate random data
            for (unsigned int i = 0; i < n; ++i)
            {
                unsigned int d = RAND(unsigned int, MAX_VALUE);
                cout << "Picked random # " << d << "\n";
                write(&t1, &t2, d);
            }

            print(&t1, &t2);
            sort(&t1, &t2, &t3, &t4);
        }
    }
    break;

    default:
        cout << "Unknown test " << bob << "\n";
    }

    return 0;
}

