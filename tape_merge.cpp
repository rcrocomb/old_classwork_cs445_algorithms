#include <iostream>
#include <assert.h>

#include <vector>

/*!
    Tapes will be represented by vectors.

    Front of tape is back of vector

    So tape that holds "abcd" is like so:

    [0] [1] [2] [3]
     d   c   b   a
*/
typedef std::vector<int> tape_t;

/*!
    Write element 'data' to end of tape
*/

void
write(tape_t *d, unsigned int data)
{
    d->insert(d->begin(), 1, data);
}

unsigned int
read(tape_t *s)
{
    assert((!s->empty()) && "read: empty tape");
    unsigned int data = s->back();
    s->pop_back();
    return data;
}

bool
is_end(tape_t *t)
{
    return t->empty();
}

void
alice
(
    unsigned int x,
    unsigned int y,
    tape_t *s1,
    tape_t *t2,
    tape_t *s3,
    tape_t *d,
    tape_t *o
)
{
    while (!is_end(s3))
    {
        if (is_end(d))
            write(o, x);
        else
            write(d, x)
        x = read(s3);
    }
    sorty_thing(d, o, d, s1, s2);
}

/*!
    Return value is tape number 1..4 where sorted keys start: extra keys are on
    "tape + 1".  If value is 4, then I dunno.
*/

unsigned int
sorty_thing(tape_t *s1, tape_t *t2, tape_t *s3, tape_t *d, tape_t *o)
{
    static unsigned int x, y;
    x = read(s1);
    y = read(s2);
    if (x < y)
    {
        write(d, x);
        write(d, y);
        x = y;
    } else
    {
        write(d, y);
        write(d, x);
    }

    y = read(s3);
    if (is_end(s3))
    {
        if (y > x)
        {
            write();
        }
    } else
    {
        alice(x, y, s1, s2, s3, d, o);
    }
}

void
merge(tape_t *t1, tape_t *t2, tape_t *t3, tape_t *t4)
{
    unsigned int n = t1->size() + t2->size();

    switch (n)
    {
    case 0:
        // blank tapes!
        return;
    case 1:
        //  single element on tape 1.  Done!
        return;
    case 2:
    {
        // two possibilities: smallest on 1st tape, or smallest on 2nd
        assert((t1->size() == 1) && "t1 is screwed up");
        assert((t2->size() == 1) && "t2 is screwed up");

        unsigned int x = (*t1)[0];
        unsigned int y = (*t2)[0];
        if (x < y)
            return; // came in sorted order (smallest on 1st): done
        else
        {
            (*t1)[0] = y;   // reverse sorted (smallest on 2nd): fix
            (*t2)[0] = x;
            // done!
            return;
        }
    }
    break;

    default:
        // unknown number of elements
        sorty_thing(t1,t1,t2,t3,t4);
    }

}

/*!

*/

int
main(int argc, char *argv[])
{
    tape_t t1();
    tape_t t2();
    tape_t t3();
    tape_t t4();

    merge(&t1, &t2, &t3, &t4);

    return 0;
}
