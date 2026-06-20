#ifndef LEVELED_ACTOR_LEVEL_TABLE_H
#define LEVELED_ACTOR_LEVEL_TABLE_H
#include "z64.h"

typedef struct {
    const char* display;   // Display name, e.g. "Keese"
    const char* baseCvar;  // CVar for the per-enemy base-level override (0 = use area level)
    const char* scaleCvar; // CVar for the per-enemy scaling-cap multiplier (".ScaleMult"; ".OverMult" is derived)
    s16 actorIds[3];       // actor ids this entry covers (0 = unused slot)
} LeveledEnemyEntry;

typedef struct {
    const char* display;   // Display name, e.g. "Hyrule Field (Adult)"
    const char* cvar;      // CVar for the area level override (default = pre-determined level)
    s8 defaultLevel;       // pre-determined area level (CVar + slider default)
    s8 age;                // 0 = any, 1 = child only, 2 = adult only
    s16 sceneIds[6];       // scenes this entry covers (-1 = unused slot)
} LeveledAreaEntry;

#ifdef __cplusplus
extern "C" {
#endif

void Actor_GetLevelAndExperience(PlayState* play, Actor* actor, u16 actorIdOverride);

extern const LeveledEnemyEntry gLeveledEnemyTable[];
extern const s32 gLeveledEnemyTableCount;
extern const LeveledAreaEntry gLeveledAreaTable[];
extern const s32 gLeveledAreaTableCount;

s32 Leveled_GetEnemyBaseOverride(s16 actorId);
f32 Leveled_GetEnemyScaleOverride(s16 actorId);
f32 Leveled_GetEnemyScalingMultOverride(s16 actorId);

#ifdef __cplusplus
}
#endif

#endif