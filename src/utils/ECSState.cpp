//
// Created by kevin on 6/15/2022.
//

#include "ECSState.h"

#include <utility>

ECSState::ECSState(std::string name_,
                   std::vector<IRedline*>& redlines_,
                   const CommandData& config_,
                   ECSState& failState_):
    name(std::move(name_)),
    redlines(redlines_),
    config(config_),
    failState(failState_)
{}
