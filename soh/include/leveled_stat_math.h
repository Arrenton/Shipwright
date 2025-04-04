#ifndef LEVELED_STAT_MATH_H
#define LEVELED_STAT_MATH_H
#include "z64.h"

#ifdef __cplusplus
extern "C"
{
#endif

u16 GetActorStat_DisplayAttack(u16 attack, u8 power);
u16 GetActorStat_Attack(u16 attack, u8 power);
u8 GetActorStat_Power(u8 level);
u8 GetActorStat_Courage(u8 level);
u8 GetActorStat_PlayerPower(u8 level);
u8 GetActorStat_PlayerCourage(u8 level);
u16 GetActorStat_EnemyMaxHealth(u16 baseHealth, u8 level);
u8 GetPlayerStat_BonusHearts(u8 level);
u8 GetPlayerStat_MagicUnits(u8 level);
u16 GetPlayerStat_GetModifiedHealthCapacity(u16 baseHealth, u8 level);
u16 GetPlayerStat_NextLevelExpAtLevel(u8 level);
u16 GetActorStat_NextLevelExp(u8 level, u32 currentExp);
u16 GetEnemyExperienceReward(u8 level, u16 expRate);
f32 Leveled_DamageFormula(f32 attack, u8 power, u8 courage);
f32 Leveled_DamageModify(Actor * actor, Actor * attackingActor, f32 attack);
u16 Leveled_GoldSkulltulaExperience(u8 tokens);
void Leveled_SetPlayerModifiedStats(Player * player);
u8 Leveled_GetHealthAttackMultiplier();
s8 Leveled_GetSceneLevel(s16 sceneId);

#ifdef __cplusplus
}
#endif

#endif