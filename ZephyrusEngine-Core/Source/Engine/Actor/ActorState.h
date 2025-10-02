#pragma once
#include <string>

namespace Zephyrus::ActorComponent {
    enum class ActorState {
        Active,
        Paused,
        Dead
    };

    inline ActorState StringToActorState(const std::string& str) {
        if (str == "active") return ActorState::Active;
        if (str == "paused") return ActorState::Paused;
        if (str == "dead") return ActorState::Dead;
        return ActorState::Active;
    }

    inline const char* ActorStateToString(ActorState state) {
        switch (state) {
        case ActorState::Active: return "active";
        case ActorState::Paused: return "paused";
        case ActorState::Dead:   return "dead";
        }
        return "active";
    }
}