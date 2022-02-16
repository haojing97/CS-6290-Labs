// --------------------------------------------------------------------- //
// For part B, you will need to modify this file.                        //
// You may add any code you need, as long as you correctly implement the //
// three required BPred methods already listed in this file.             //
// --------------------------------------------------------------------- //

// bpred.cpp
// Implements the branch predictor class.

#include <stdio.h>
#include "bpred.h"

/**
 * Construct a branch predictor with the given policy.
 * 
 * In part B of the lab, you must implement this constructor.
 * 
 * @param policy the policy this branch predictor should use
 */
BPred::BPred(BPredPolicy policy)
{
    // TODO: Initialize member variables here.
    this->policy = policy;
    this->stat_num_branches = 0;
    this->stat_num_mispred = 0;

    // As a reminder, you can declare any additional member variables you need
    // in the BPred class in bpred.h and initialize them here.
    this->ghr = 0;
    for (unsigned int i = 0; i < PHT_SIZE; i++)
    {
        this->pht[i] = 2;
    }
}

/**
 * Get a prediction for the branch with the given address.
 * 
 * In part B of the lab, you must implement this method.
 * 
 * @param pc the address (program counter) of the branch to predict
 * @return the prediction for whether the branch is taken or not taken
 */
BranchDirection BPred::predict(uint64_t pc)
{
    // TODO: Return a prediction for whether the branch at address pc will be
    // TAKEN or NOT_TAKEN according to this branch predictor's policy.

    // Note that you do not have to handle the BPRED_PERFECT policy here; this
    // function will not be called for that policy.
    if (this->policy == BPRED_ALWAYS_TAKEN)
    {
        return TAKEN;
    }

    else if (this->policy == BPRED_GSHARE)
    {
        uint64_t index = (pc ^ this->ghr) & GHR_MASK;
        if (this->pht[index] >= 2)
        {
            return TAKEN;
        }
        else
        {
            return NOT_TAKEN;
        }
    }

    else
    {
        return TAKEN;
    }
}

/**
 * Update the branch predictor statistics (stat_num_branches and
 * stat_num_mispred), as well as any other internal state you may need to
 * update in the branch predictor.
 * 
 * In part B of the lab, you must implement this method.
 * 
 * @param pc the address (program counter) of the branch
 * @param prediction the prediction made by the branch predictor
 * @param resolution the actual outcome of the branch
 */
void BPred::update(uint64_t pc, BranchDirection prediction,
                   BranchDirection resolution)
{
    // TODO: Update the stat_num_branches and stat_num_mispred member variables
    // according to the prediction and resolution of the branch.
    this->stat_num_branches += 1;
    if (prediction != resolution)
    {
        this->stat_num_mispred += 1;
    }

    uint64_t index = (pc ^ this->ghr) & GHR_MASK;

    if (resolution == TAKEN)
    {
        this->pht[index] = sat_increment(this->pht[index], 3);
    }
    else
    {
        this->pht[index] = sat_decrement(this->pht[index]);
    }

    // TODO: Update any other internal state you may need to keep track of.
    if (resolution == TAKEN)
    {
        this->ghr = (this->ghr << 1) | 1;
    }
    else
    {
        this->ghr <<= 1;
    }

    // Note that you do not have to handle the BPRED_PERFECT policy here; this
    // function will not be called for that policy.
}
