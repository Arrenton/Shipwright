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
u32 GetCumulativeExp(u8 level);
u8 Leveled_GetPlayerMaxLevel();
u32 Leveled_GetPlayerMaxExp();
u8 Leveled_GetProgressLevelCap();
u16 GetEnemyExperienceReward(u8 level, u16 expRate);
f32 Leveled_DamageFormula(f32 attack, u8 power, u8 courage);
f32 Leveled_DamageModify(Actor * actor, Actor * attackingActor, f32 attack);
u16 Leveled_GoldSkulltulaExperience(u8 tokens);
void Leveled_SetPlayerModifiedStats(Player * player);
u8 Leveled_GetHealthAttackMultiplier();
s16 Leveled_GetSceneLevel(s16 sceneId);

// Per-item leveling (weapons/equipment each have their own EXP bar). Indices must stay < 32 to fit
// gSaveContext.itemLevels/itemExp. C-items first, then equipment.
typedef enum {
    LEVELED_ITEM_DEKU_STICK, LEVELED_ITEM_DEKU_NUT, LEVELED_ITEM_SLINGSHOT, LEVELED_ITEM_BOMB,
    LEVELED_ITEM_BOMBCHU, LEVELED_ITEM_BOOMERANG, LEVELED_ITEM_BOW, LEVELED_ITEM_FIRE_ARROW,
    LEVELED_ITEM_ICE_ARROW, LEVELED_ITEM_LIGHT_ARROW, LEVELED_ITEM_HOOKSHOT, LEVELED_ITEM_HAMMER,
    LEVELED_ITEM_LENS, LEVELED_ITEM_DINS_FIRE, LEVELED_ITEM_FARORES_WIND, LEVELED_ITEM_NAYRUS_LOVE,
    LEVELED_ITEM_KOKIRI_SWORD, LEVELED_ITEM_MASTER_SWORD, LEVELED_ITEM_BIGGORON_SWORD,
    LEVELED_ITEM_DEKU_SHIELD, LEVELED_ITEM_HYLIAN_SHIELD, LEVELED_ITEM_MIRROR_SHIELD,
    LEVELED_ITEM_KOKIRI_TUNIC, LEVELED_ITEM_GORON_TUNIC, LEVELED_ITEM_ZORA_TUNIC,
    LEVELED_ITEM_KOKIRI_BOOTS, LEVELED_ITEM_IRON_BOOTS, LEVELED_ITEM_HOVER_BOOTS,
    LEVELED_ITEM_MAX
} LeveledItemId;

s32 Leveled_ItemIdToLeveledItem(s32 itemId);         // map an inventory ITEM_* id to a LeveledItemId (-1 if none)
const char* Leveled_GetItemDisplayName(s32 item);    // short display name for HUD/menus
const char* Leveled_GetItemCvarPrefix(s32 item);     // e.g. "gLeveled.Item.Bow"
u8 Leveled_GetItemLevelCap(s32 item);                // per-item configurable cap (1-99, default 99)
u8 Leveled_GetItemLevel(s32 item);                   // current level (1-cap)
u32 Leveled_GetItemExp(s32 item);                    // raw stored EXP (for menu readout)
f32 Leveled_GetItemLevelFraction(s32 item);          // 0.0 at level 1, 1.0 at the level cap
void Leveled_GainItemExp(s32 item, u16 exp);         // add EXP, recompute level (no-op if exp == 0)
f32 Leveled_GetItemEffectMult(s32 item, const char* capSuffix, f32 defaultCap); // 1.0 -> cap as it levels

// Bonus application.
f32 Leveled_GetWeaponDamageMult(Actor* source, Player* player); // per-weapon damage scaling (1.0 if N/A)
s16 Leveled_ModifyMagicArrowCost(s32 magicArrowType, s16 baseCost); // fire/ice/light arrow magic cost
s16 Leveled_ModifyMagicSpellCost(s32 magicSpell, s16 baseCost);     // Farore's/Nayru's/Din's magic cost
f32 Leveled_GetEquipmentStatScale(s32 item); // multiplier on an item's stat bonus (1.0 -> 2.0 as it levels)
s16 Leveled_ScaleStunDuration(Actor* actor, s16 duration); // lengthen a stun caused by a leveled stun item
s32 Leveled_DekuStickSurvives(void);         // true if a leveled Deku Stick avoids breaking this hit
s16 Leveled_GetLensDrainInterval(s16 baseInterval); // frames between Lens magic ticks (grows with level)

// EXP-gain hooks.
void Leveled_AwardWeaponExpOnHit(Actor* target, Actor* source, PlayState* play); // weapon/projectile contact
void Leveled_AwardOnEnemyDefeated(Player* player, u16 enemyExp);                  // equipped tunic/boots + Nayru
void Leveled_AwardShieldBlockExp(Player* player, Actor* attacker);               // shield blocked an attack
void Leveled_AwardStunExp(Actor* target, s32 item);                              // deku nut / stun-based items
void Leveled_AwardStaticExp(s32 item, const char* expSuffix, u16 defaultExp);   // Farore's Wind, Lens of Truth

#ifdef __cplusplus
}
#endif

#endif