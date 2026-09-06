#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"
#include "soh/ShipInit.hpp"

extern "C" {
#include "variables.h"
#include "leveled_stat_math.h"
#include "macros.h"
extern PlayState* gPlayState;
}

static constexpr int32_t CVAR_HURT_CONTAINER_DEFAULT = 0;
#define CVAR_HURT_CONTAINER_NAME CVAR_ENHANCEMENT("HurtContainer")
#define CVAR_HURT_CONTAINER_VALUE CVarGetInteger(CVAR_HURT_CONTAINER_NAME, CVAR_HURT_CONTAINER_DEFAULT)

static bool hurtEnabled = false;

static void UpdateHurtContainerModeState() {
    hurtEnabled = CVAR_HURT_CONTAINER_VALUE;
    uint16_t heartPieceContainers = gSaveContext.ship.stats.heartPieces / 4;
    uint16_t heartContainers = gSaveContext.ship.stats.heartContainers;
    uint16_t healthCapacityMod = (heartPieceContainers + heartContainers) * FULL_HEART_HEALTH;

    if (hurtEnabled != CVAR_HURT_CONTAINER_DEFAULT) {
        gSaveContext.healthCapacity = MAX_HEALTH - healthCapacityMod;
    } else {
        gSaveContext.healthCapacity = STARTING_HEALTH + healthCapacityMod;
    }
    gSaveContext.healthCapacity2 = GetPlayerStat_GetModifiedHealthCapacity(gSaveContext.healthCapacity, GET_PLAYER(gPlayState)->actor.level);
}

static void RegisterHurtContainer() {
    if (GameInteractor::IsSaveLoaded(false)) {
        UpdateHurtContainerModeState();
    }

    COND_HOOK(OnLoadGame, hurtEnabled != CVAR_HURT_CONTAINER_VALUE, [](int32_t) { UpdateHurtContainerModeState(); });

    COND_VB_SHOULD(VB_HEARTS_INCREASE_WITH_CONTAINERS, CVAR_HURT_CONTAINER_VALUE, {
        *should = false;
        gSaveContext.healthCapacity -= FULL_HEART_HEALTH;
        gSaveContext.health -= FULL_HEART_HEALTH;
    });
}

static RegisterShipInitFunc initFunc(RegisterHurtContainer, { CVAR_HURT_CONTAINER_NAME });
