#include "tournament_deme.hh"
#include <math.h>
#include <cassert>
#include <algorithm>
#include <queue>

Chromosome*
TournamentDeme::select_parent() {
    //Select P parents. S.t. P is a const pow of 2 <= pop_size().
    //P = 2 ** floor( log_2 ( POP_SIZE ))
    auto POP_SIZE = pop_.size();
    assert(POP_SIZE > 0);       //Sanity check.

    auto p_exp = floor(log2(POP_SIZE));
    unsigned P = pow(2, p_exp);     //Tournament size.
    assert(P <= POP_SIZE);      //Sanity check.

    //Tournament of P parents:
    //Compare the first pair of parents, etc... until you have P/2 parents.
    //Then repeat until you have one parent left.
    //
    //Shuffle POP_SIZE indices, put the first P into a FIFO Queue.
    std::vector<unsigned> cmp_idx(POP_SIZE);
    std::iota(cmp_idx.begin(), cmp_idx.end(), 0);
    std::shuffle(cmp_idx.begin(), cmp_idx.end(), generator_);
    std::queue<unsigned> bracket;
    for (unsigned i = 0; i < P; i++) {
        bracket.push(cmp_idx[i]);
    }
    assert(bracket.size() == P);

    //Compare pairs of chromosomes until bracket has one element.
    //Bracket is guaranteed to have one element since it is a power of 2.
    while (bracket.size() > 1) {
        auto first = bracket.front();
        bracket.pop();
        auto second = bracket.front();
        bracket.pop();

        if (pop_[first]->get_fitness() > pop_[second]->get_fitness()) {
            bracket.push(first);
        } else {
            bracket.push(second);
        }
    }
    assert(bracket.size() == 1);
    return pop_[bracket.front()];
}