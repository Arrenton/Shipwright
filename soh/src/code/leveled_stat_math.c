#include "global.h"
#include "leveled_stat_math.h"
#include <stdio.h>

static u32 sExpTable[] = { 0,      30,     64,     105,    155,    217,    292,    385,    497,    632,    792,
                           981,    1202,   1458,   1753,   2089,   2471,   2903,   3388,   3930,   4534,   5203,
                           5942,   6755,   7648,   8625,   9691,   10851,  12110,  13474,  14949,  16540,  18253,
                           20094,  22070,  24188,  26453,  28874,  31456,  34209,  37139,  40254,  43562,  47073,
                           50794,  54734,  58904,  63312,  67968,  72883,  78067,  83531,  89286,  95345,  101719,
                           108421, 115463, 122859, 130623, 138770, 147314, 156270, 165655, 175485, 185778, 196552,
                           207824, 219614, 231943, 244831, 258300, 272372, 287071, 302422, 318450, 335182, 352645,
                           370870, 389885, 409724, 430419, 452005, 474520, 498001, 522489, 548027, 574659, 602434,
                           631400, 661611, 693122, 725993, 760287, 796070, 833414, 872395, 913094, 955597, 999999,
                           1044999 }; // index 99: level-99->100 threshold; also the base for the >99 formula
/* Old table
{ 0,      30,     65,     110,    167,    240,    334,    452,    598,    777,    993,
  1251,   1555,   1910,   2320,   2791,   3329,   3937,   4621,   5388,   6242,   7189,
  8235,   9386,   10647,  12026,  13528,  15160,  16929,  18840,  20902,  23120,  25501,
  28054,  30784,  33700,  36809,  40119,  43637,  47371,  51329,  55520,  59951,  64630,
  69567,  74770,  80248,  86008,  92061,  98416,  105080, 112065, 119379, 127032, 135033,
  143392, 152119, 161225, 170719, 180611, 190912, 201632, 212783, 224374, 236417, 248922,
  261901, 275365, 289325, 303792, 318779, 334297, 350358, 366973, 384155, 401916, 420268,
  439224, 458796, 478997, 499839, 521336, 543501, 566346, 589885, 614131, 639098, 664799,
  691249, 718461, 746448, 775226, 804807, 835208, 866441, 898523, 931466, 965287, 999999 };*/

u8 Leveled_GetHealthAttackMultiplier() {
    if (!CVarGetInteger("gLeveled.Master", 1)) {
        return 1; // Master off: no HP/attack multiplier
    }
    return CLAMP(CVarGetInteger("gLeveled.Enhancements.AttackAndHPMultiplier", 9), 1, 32);
}

u16 GetActorStat_DisplayAttack(u16 attack, u8 power) {
    return GetActorStat_Attack(attack, power) / (1 + (float)power / 30.0f);
}

u16 GetActorStat_Attack(u16 attack, u8 power) {
    return (float)attack * (1 + (power - 2) * (0.14f + (power - 2) * 0.0006f));
}

f32 GetActorStat_EnemyAttack(u16 attack, u8 power) {
    return (float)attack * (1 + (power - 2) * (0.007f + (power - 2) * 0.0002f));
}

u8 GetActorStat_Power(u8 level) {
    return 3 + (u8)(84 * level / 99.0f);
}

u8 GetActorStat_Courage(u8 level) {
    return 2 + (u8)(75 * level / 99.0f);
}

u8 GetActorStat_PlayerPower(u8 level) {
    return 2 + (u8)(75 * level / 99.0f);
}

u8 GetActorStat_PlayerCourage(u8 level) {
    return 3 + (u8)(84 * level / 99.0f);
}

u16 GetActorStat_EnemyMaxHealth(u16 baseHealth, u8 level){
    if (!CVarGetInteger("gLeveled.Master", 1)) {
        return baseHealth; // Master off: enemies keep their vanilla HP
    }
    return (u16)(CLAMP((f32)GetActorStat_Attack(baseHealth * Leveled_GetHealthAttackMultiplier(), GetActorStat_PlayerPower(level)) * CVarGetFloat("gLeveled.Difficulty.Enemy.HPPercent", 1.0f), 1, 0xffff));
}

u8 GetPlayerStat_BonusHearts(u8 level){
    if (CVarGetInteger("gLeveled.Player.Enhancements.HeartsWithLevelUp", 1) == 0){
        return 0;
    }

    u8 bonusHearts = (level + 1) / 8;
    if (bonusHearts > 10){
        bonusHearts = 10;
    }
    return bonusHearts;
}

u8 GetPlayerStat_MagicUnits(u8 level){
    if (CVarGetInteger("gLeveled.Player.Enhancements.MagicWithLevelUp", 1) == 0){
        return 48;
    }

    u8 maximumMagic = 12 + (u8)((f32)level / 2.8f) * 2;
    if (maximumMagic > 72){
        maximumMagic = 72;
    }
    return maximumMagic;
}

u16 GetPlayerStat_GetModifiedHealthCapacity(u16 baseHealth, u8 level){
    s32 heartUnits = CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) << 2;
    u16 baseHearts = baseHealth / 16;
    return (baseHearts + GetPlayerStat_BonusHearts(level)) * heartUnits;
}

u16 GetPlayerStat_NextLevelExpAtLevel(u8 level) {
    if (level == 99)
        return 0;

    s32 nextLv = sExpTable[level] - sExpTable[level - 1];
}

// Cumulative EXP required to reach a given level. Levels 0-99 use the table; beyond 99 the curve is
// continued with a gentle quadratic so the Link "Break Level cap" stays meaningful (reachable to 255).
u32 GetCumulativeExp(u8 level) {
    if (level <= 99) {
        return sExpTable[level];
    }
    u32 n = (u32)level - 99u;
    return sExpTable[99] + n * 45000u + (n * (n - 1u) / 2u) * 3000u;
}

// 255 when the level cap is broken for Link ("Link Only" / "Enemies & Link"), otherwise 99.
u8 Leveled_GetPlayerMaxLevel() {
    s32 breakCap = CVarGetInteger("gLeveled.LevelCap.BreakCap", 1); // 0=None 1=Enemies 2=Link 3=Both
    return (breakCap == 2 || breakCap == 3) ? 255 : 99;
}

u32 Leveled_GetPlayerMaxExp() {
    return GetCumulativeExp(Leveled_GetPlayerMaxLevel());
}

// Progress-based level cap. Kid/Adult Limit are per-era hard caps; the dungeon sliders are sub-gates
// (cap = min(era cap, every gate not yet cleared)). The caller applies this only when Level Limits are on.
u8 Leveled_GetProgressLevelCap() {
    s32 cap;
    if (LINK_IS_ADULT) {
        cap = CVarGetInteger("gLeveled.LevelLimit.AdultLimit", 99);
        u8 medallions = 0;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST)) medallions++;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE)) medallions++;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_WATER)) medallions++;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) medallions++;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW)) medallions++;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_LIGHT)) medallions++;
        if (medallions < 1) { s32 g = CVarGetInteger("gLeveled.LevelLimit.Dungeon1", 99); if (g < cap) cap = g; }
        if (medallions < 2) { s32 g = CVarGetInteger("gLeveled.LevelLimit.Dungeon2", 99); if (g < cap) cap = g; }
        if (medallions < 3) { s32 g = CVarGetInteger("gLeveled.LevelLimit.Dungeon3", 99); if (g < cap) cap = g; }
        if (medallions < 4) { s32 g = CVarGetInteger("gLeveled.LevelLimit.Dungeon4", 99); if (g < cap) cap = g; }
        if (medallions < 5) { s32 g = CVarGetInteger("gLeveled.LevelLimit.Dungeon5", 99); if (g < cap) cap = g; }
    } else {
        cap = CVarGetInteger("gLeveled.LevelLimit.KidLimit", 99);
        if (!CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) { s32 g = CVarGetInteger("gLeveled.LevelLimit.DekuTree", 99); if (g < cap) cap = g; }
        if (!CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) { s32 g = CVarGetInteger("gLeveled.LevelLimit.DeathMountain", 99); if (g < cap) cap = g; }
        if (!CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) { s32 g = CVarGetInteger("gLeveled.LevelLimit.JabuJabu", 99); if (g < cap) cap = g; }
    }
    if (cap < 1) cap = 1;
    if (cap > 255) cap = 255;
    return (u8)cap;
}

u16 GetActorStat_NextLevelExp(u8 level, u32 currentExp) {
    if (level >= Leveled_GetPlayerMaxLevel())
        return 0;

    s32 nextLv = (s32)GetCumulativeExp(level) - (s32)currentExp;

    if (nextLv < 0)
        nextLv = 0;
    return (u16)nextLv; // HUD display only (may truncate past level 99); leveling uses GetCumulativeExp
}

u16 GetEnemyExperienceReward(u8 level, u16 expRate) {
    if (expRate == 0)
        return 0;

    return CLAMP(round((3 + CLAMP_MAX(floor(level / 6) * 4, 4) + (level - 1) * ((0.1 + (level / 95.0)) + pow(CLAMP_MIN(level - 8, 0), 1.25) / 80.0)) * expRate / 100.0), 1, 9999);
}

f32 Leveled_DamageFormula(f32 attack, u8 power, u8 courage) {
    f32 damage = GetActorStat_Attack(attack, power);
    if (power >= courage) {
        for (u8 i = 0; i < power - courage; i++) {
            damage *= 1.01f;
        }
    } else {
        for (u8 i = 0; i < courage - power; i++) {
            damage *= 0.97f;
        }
    }
    return damage;
}

f32 Leveled_DamageFormulaOnPlayer(f32 attack, u8 power, u8 courage) {
    f32 damage = attack;

    if (CVarGetInteger("gLeveled.Enemy.Enhancements.AttackScalesWithLevel", 1) == 1){
        damage = GetActorStat_EnemyAttack(attack, power);

        if (power >= courage) {
            for (u8 i = 0; i < power - courage; i++) {
                f32 multAddition = CLAMP_MIN((0.07f - (power * 0.0005f)) - (f32)i / (100.0f - (power * 0.33f)), 0);
                damage *= 1.04f + multAddition;
            }
        } else {
            for (u8 i = 0; i < courage - power; i++) {
                damage *= 0.96f;
            }
        }
    } else {
        if (power >= courage) {
            for (u8 i = 0; i < power - courage; i++) {
                damage *= 1.05f + CLAMP_MIN(0.05f - (f32)i / 100.0f, 0);
            }
        } else {
            for (u8 i = 0; i < courage - power; i++) {
                damage *= 0.96f;
            }
        }
    }
    return damage;
}

f32 Leveled_DamageModify(Actor* actor, Actor* attackingActor, f32 attack) {
    if (!CVarGetInteger("gLeveled.Master", 1)) {
        return attack; // Master off: damage passes through unmodified (vanilla)
    }
    f32 damage;
    if (actor->category == ACTORCAT_PLAYER) {
        damage = Leveled_DamageFormulaOnPlayer(attack, CLAMP(attackingActor->power + attackingActor->powerModifier, 0, 255), CLAMP(actor->courage + actor->courageModifier, 0, 255));
    } else {
        damage = Leveled_DamageFormula(attack, CLAMP(attackingActor->power + attackingActor->powerModifier, 0, 255), CLAMP(actor->courage + actor->courageModifier, 0, 255));
    }


    if (damage >= 1.25f)
        damage += Rand_ZeroOne() - 0.2f;

    if (damage >= 6)
        damage += Rand_ZeroFloat(damage * 0.12f) - damage * 0.06f;

    if (damage > 9999)
        damage = 9999;

    damage = (u16)(damage + 0.5f);

    return CLAMP_MIN(damage, attack > 0 ? 1 : 0);
}

u16 Leveled_GoldSkulltulaExperience(u8 tokens) {
    u16 experience = 5;
    u8 i;

    for (i = 0; i < tokens; i++) {
        experience += 5 + 5 * i / (f32)10.0;
    }
    return (u16)((f32)experience * CVarGetFloat("gLeveled.Difficulty.EXP.TokenRate", 1.0f));
}

// Per-item equipment stat bonus. Each level adds a configurable percentage (default +1%/level) to the
// item's stat contribution: level 1 = +2%, level 2 = +3%, ... level 99 = +100% (so the returned
// multiplier runs 1.02 -> 2.00). Gated by the Master switch and the equipment-scaling toggle.
f32 Leveled_GetEquipmentStatScale(s32 item) {
    if (!CVarGetInteger("gLeveled.Master", 1) || !CVarGetInteger("gLeveled.Item.EnableEquipment", 1)) {
        return 1.0f;
    }
    f32 perLevel = CVarGetFloat("gLeveled.Item.EquipStatPercentPerLevel", 5.0f);
    f32 bonus = perLevel * (f32)(Leveled_GetItemLevel(item) + 1) / 100.0f;
    return 1.0f + CLAMP_MIN(bonus, 0.0f);
}

// Adds an equipment item's (power, courage) contribution, boosting only the positive part by the item's
// level bonus so leveling is always beneficial (an item's negative tradeoffs are left untouched).
static void Leveled_AddEquipStat(s32 item, s32 power, s32 courage, s8* powerMod, s8* courageMod) {
    f32 scale = Leveled_GetEquipmentStatScale(item);
    *powerMod += (s8)((power > 0) ? (power * scale + 0.5f) : (f32)power);
    *courageMod += (s8)((courage > 0) ? (courage * scale + 0.5f) : (f32)courage);
}

void Leveled_SetPlayerModifiedStats(Player* player) {
    if (!CVarGetInteger("gLeveled.Master", 1)) {
        player->actor.powerModifier = 0;
        player->actor.courageModifier = 0;
        return; // Master off: no equipment-based stat modifiers
    }
    s8 powerModifier = 0;
    s8 courageModifier = 0;

    if (CVarGetInteger("gLeveled.Player.Enhancements.EquipmentStats", 1) == 1){
        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD)){
            case PLAYER_SWORD_KOKIRI:
                Leveled_AddEquipStat(LEVELED_ITEM_KOKIRI_SWORD, 2, 0, &powerModifier, &courageModifier);
                break;

            case PLAYER_SWORD_MASTER:
                Leveled_AddEquipStat(LEVELED_ITEM_MASTER_SWORD, 0, 1, &powerModifier, &courageModifier);
                break;

            case PLAYER_SWORD_BIGGORON:
                if (gBitFlags[3] & gSaveContext.inventory.equipment){
                    Leveled_AddEquipStat(LEVELED_ITEM_BIGGORON_SWORD, -7, -12, &powerModifier, &courageModifier);
                } else {
                    Leveled_AddEquipStat(LEVELED_ITEM_BIGGORON_SWORD, 2, -8, &powerModifier, &courageModifier);
                }
                break;

            default:
                break;
        }

        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) - 1){
            case PLAYER_TUNIC_KOKIRI:
                Leveled_AddEquipStat(LEVELED_ITEM_KOKIRI_TUNIC, 0, 2, &powerModifier, &courageModifier);
                break;

            case PLAYER_TUNIC_GORON:
                Leveled_AddEquipStat(LEVELED_ITEM_GORON_TUNIC, 3, -3, &powerModifier, &courageModifier);
                break;

            case PLAYER_TUNIC_ZORA:
                Leveled_AddEquipStat(LEVELED_ITEM_ZORA_TUNIC, -3, 3, &powerModifier, &courageModifier);
                break;

            default:
                break;
        }

        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD)){
            case PLAYER_SHIELD_DEKU:
                Leveled_AddEquipStat(LEVELED_ITEM_DEKU_SHIELD, 0, 1, &powerModifier, &courageModifier);
                break;

            case PLAYER_SHIELD_HYLIAN:
                Leveled_AddEquipStat(LEVELED_ITEM_HYLIAN_SHIELD, 0, 2, &powerModifier, &courageModifier);
                break;

            case PLAYER_SHIELD_MIRROR:
                Leveled_AddEquipStat(LEVELED_ITEM_MIRROR_SHIELD, -2, 3, &powerModifier, &courageModifier);
                break;

            default:
                break;
        }

        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_BOOTS) - 1){
            case PLAYER_BOOTS_KOKIRI:
                Leveled_AddEquipStat(LEVELED_ITEM_KOKIRI_BOOTS, 0, 2, &powerModifier, &courageModifier);
                break;

            case PLAYER_BOOTS_IRON:
                Leveled_AddEquipStat(LEVELED_ITEM_IRON_BOOTS, 2, 1, &powerModifier, &courageModifier);
                break;

            case PLAYER_BOOTS_HOVER:
                // +1 power scales with level; the -1 courage tradeoff stays fixed.
                Leveled_AddEquipStat(LEVELED_ITEM_HOVER_BOOTS, 1, -1, &powerModifier, &courageModifier);
                break;

            default:
                break;
        }

        powerModifier += Player_GetStrength();
    }

    player->actor.powerModifier = powerModifier;
    player->actor.courageModifier = courageModifier;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Per-item leveling
//////////////////////////////////////////////////////////////////////////////////////////////////

// CVar prefix per item; sub-CVars (.LevelCap, .DamageCap, .MagicCap, ...) are built from this. Order
// must match the LeveledItemId enum.
static const char* sItemCvarPrefix[LEVELED_ITEM_MAX] = {
    "gLeveled.Item.DekuStick",  "gLeveled.Item.DekuNut",    "gLeveled.Item.Slingshot",  "gLeveled.Item.Bomb",
    "gLeveled.Item.Bombchu",    "gLeveled.Item.Boomerang",  "gLeveled.Item.Bow",        "gLeveled.Item.FireArrow",
    "gLeveled.Item.IceArrow",   "gLeveled.Item.LightArrow", "gLeveled.Item.Hookshot",   "gLeveled.Item.Hammer",
    "gLeveled.Item.Lens",       "gLeveled.Item.DinsFire",   "gLeveled.Item.FaroresWind","gLeveled.Item.NayrusLove",
    "gLeveled.Item.KokiriSword","gLeveled.Item.MasterSword","gLeveled.Item.BiggoronSword",
    "gLeveled.Item.DekuShield", "gLeveled.Item.HylianShield","gLeveled.Item.MirrorShield",
    "gLeveled.Item.KokiriTunic","gLeveled.Item.GoronTunic", "gLeveled.Item.ZoraTunic",
    "gLeveled.Item.KokiriBoots","gLeveled.Item.IronBoots",  "gLeveled.Item.HoverBoots",
};

const char* Leveled_GetItemCvarPrefix(s32 item) {
    if (item < 0 || item >= LEVELED_ITEM_MAX) {
        return "gLeveled.Item.Unknown";
    }
    return sItemCvarPrefix[item];
}

// Short display names, in LeveledItemId order (for the in-game HUD and popups).
static const char* sItemDisplayName[LEVELED_ITEM_MAX] = {
    "Deku Stick",   "Deku Nut",     "Slingshot",    "Bomb",         "Bombchu",      "Boomerang",
    "Bow",          "Fire Arrow",   "Ice Arrow",    "Light Arrow",  "Hookshot",     "Hammer",
    "Lens",         "Din's Fire",   "Farore's Wind","Nayru's Love", "Kokiri Sword", "Master Sword",
    "Biggoron Sword","Deku Shield", "Hylian Shield","Mirror Shield","Kokiri Tunic", "Goron Tunic",
    "Zora Tunic",   "Kokiri Boots", "Iron Boots",   "Hover Boots",
};

const char* Leveled_GetItemDisplayName(s32 item) {
    if (item < 0 || item >= LEVELED_ITEM_MAX) {
        return "";
    }
    return sItemDisplayName[item];
}

// Maps an inventory ITEM_* id (e.g. a C-button or held item) to its LeveledItemId, or -1 if the item
// doesn't level. Swords map too (the B button holds the sword).
s32 Leveled_ItemIdToLeveledItem(s32 itemId) {
    switch (itemId) {
        case ITEM_STICK:        return LEVELED_ITEM_DEKU_STICK;
        case ITEM_NUT:          return LEVELED_ITEM_DEKU_NUT;
        case ITEM_SLINGSHOT:    return LEVELED_ITEM_SLINGSHOT;
        case ITEM_BOMB:         return LEVELED_ITEM_BOMB;
        case ITEM_BOMBCHU:      return LEVELED_ITEM_BOMBCHU;
        case ITEM_BOOMERANG:    return LEVELED_ITEM_BOOMERANG;
        case ITEM_BOW:          return LEVELED_ITEM_BOW;
        case ITEM_ARROW_FIRE:   return LEVELED_ITEM_FIRE_ARROW;
        case ITEM_ARROW_ICE:    return LEVELED_ITEM_ICE_ARROW;
        case ITEM_ARROW_LIGHT:  return LEVELED_ITEM_LIGHT_ARROW;
        case ITEM_HOOKSHOT:
        case ITEM_LONGSHOT:     return LEVELED_ITEM_HOOKSHOT;
        case ITEM_HAMMER:       return LEVELED_ITEM_HAMMER;
        case ITEM_LENS:         return LEVELED_ITEM_LENS;
        case ITEM_DINS_FIRE:    return LEVELED_ITEM_DINS_FIRE;
        case ITEM_FARORES_WIND: return LEVELED_ITEM_FARORES_WIND;
        case ITEM_NAYRUS_LOVE:  return LEVELED_ITEM_NAYRUS_LOVE;
        case ITEM_SWORD_KOKIRI: return LEVELED_ITEM_KOKIRI_SWORD;
        case ITEM_SWORD_MASTER: return LEVELED_ITEM_MASTER_SWORD;
        case ITEM_SWORD_BGS:    return LEVELED_ITEM_BIGGORON_SWORD;
        case ITEM_SHIELD_DEKU:   return LEVELED_ITEM_DEKU_SHIELD;
        case ITEM_SHIELD_HYLIAN: return LEVELED_ITEM_HYLIAN_SHIELD;
        case ITEM_SHIELD_MIRROR: return LEVELED_ITEM_MIRROR_SHIELD;
        case ITEM_TUNIC_KOKIRI:  return LEVELED_ITEM_KOKIRI_TUNIC;
        case ITEM_TUNIC_GORON:   return LEVELED_ITEM_GORON_TUNIC;
        case ITEM_TUNIC_ZORA:    return LEVELED_ITEM_ZORA_TUNIC;
        case ITEM_BOOTS_KOKIRI:  return LEVELED_ITEM_KOKIRI_BOOTS;
        case ITEM_BOOTS_IRON:    return LEVELED_ITEM_IRON_BOOTS;
        case ITEM_BOOTS_HOVER:   return LEVELED_ITEM_HOVER_BOOTS;
        default:                return -1;
    }
}

// Items from LEVELED_ITEM_KOKIRI_SWORD onward are "equipment" (swords/shields/tunics/boots); everything
// before is a "c-item". Each category has its own global enable toggle (default on).
static s32 Leveled_IsEquipmentItem(s32 item) {
    return item >= LEVELED_ITEM_KOKIRI_SWORD;
}

static s32 Leveled_ItemCategoryEnabled(s32 item) {
    if (Leveled_IsEquipmentItem(item)) {
        return CVarGetInteger("gLeveled.Item.EnableEquipment", 1);
    }
    return CVarGetInteger("gLeveled.Item.EnableCItems", 1);
}

u8 Leveled_GetItemLevelCap(s32 item) {
    char buf[96];
    snprintf(buf, sizeof(buf), "%s.LevelCap", Leveled_GetItemCvarPrefix(item));
    s32 cap = CVarGetInteger(buf, 99);
    return (u8)CLAMP(cap, 1, 99);
}

u8 Leveled_GetItemLevel(s32 item) {
    if (item < 0 || item >= LEVELED_ITEM_MAX) {
        return 1;
    }
    s32 lvl = gSaveContext.itemLevels[item];
    return (u8)CLAMP(lvl, 1, Leveled_GetItemLevelCap(item));
}

u32 Leveled_GetItemExp(s32 item) {
    if (item < 0 || item >= LEVELED_ITEM_MAX) {
        return 0;
    }
    return gSaveContext.itemExp[item];
}

f32 Leveled_GetItemLevelFraction(s32 item) {
    u8 cap = Leveled_GetItemLevelCap(item);
    if (cap <= 1) {
        return 0.0f; // can't level past 1 -> no bonus
    }
    f32 f = (f32)(Leveled_GetItemLevel(item) - 1) / (f32)(cap - 1);
    return CLAMP(f, 0.0f, 1.0f);
}

void Leveled_GainItemExp(s32 item, u16 exp) {
    if (item < 0 || item >= LEVELED_ITEM_MAX || exp == 0 || !CVarGetInteger("gLeveled.Master", 1) ||
        !Leveled_ItemCategoryEnabled(item)) {
        return;
    }
    u8 cap = Leveled_GetItemLevelCap(item);
    u32 maxExp = GetCumulativeExp(cap);
    if (gSaveContext.itemExp[item] >= maxExp) {
        return; // already maxed for the current cap
    }
    u32 newExp = gSaveContext.itemExp[item] + exp;
    if (newExp > maxExp) {
        newExp = maxExp;
    }
    gSaveContext.itemExp[item] = newExp;

    u8 lvl = 1;
    while (lvl < cap && newExp >= GetCumulativeExp(lvl)) {
        lvl++;
    }
    gSaveContext.itemLevels[item] = lvl;
}

// Generic effect multiplier: 1.0x at level 1, ramping to the configured cap (capSuffix, e.g. ".DamageCap")
// at the item's level cap. Returns 1.0 (no effect) when the Master switch is off.
f32 Leveled_GetItemEffectMult(s32 item, const char* capSuffix, f32 defaultCap) {
    if (!CVarGetInteger("gLeveled.Master", 1) || !Leveled_ItemCategoryEnabled(item)) {
        return 1.0f;
    }
    char buf[96];
    snprintf(buf, sizeof(buf), "%s%s", Leveled_GetItemCvarPrefix(item), capSuffix);
    f32 cap = CVarGetFloat(buf, defaultCap);
    return 1.0f + (cap - 1.0f) * Leveled_GetItemLevelFraction(item);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Per-item EXP-gain hooks
//////////////////////////////////////////////////////////////////////////////////////////////////

// EnArrow ArrowType values (see ovl_En_Arrow/z_en_arrow.h). Stable vanilla arrow-type ids; duplicated
// here to avoid pulling an overlay-private header into core code.
enum {
    LEVELED_ARROW_FIRE = 3,
    LEVELED_ARROW_ICE = 4,
    LEVELED_ARROW_LIGHT = 5,
    LEVELED_ARROW_SEED = 9,
    LEVELED_ARROW_NUT = 10,
};

// Percent of an enemy's EXP reward an item earns from a contact event. Per-item configurable; shields
// and other special hooks pass their own default (e.g. 5%).
static u16 Leveled_ItemExpShare(s32 item, u16 enemyExp, f32 defaultPercent) {
    if (enemyExp == 0) {
        return 0;
    }
    char buf[96];
    snprintf(buf, sizeof(buf), "%s.ExpPercent", Leveled_GetItemCvarPrefix(item));
    f32 pct = CVarGetFloat(buf, defaultPercent);
    if (pct <= 0.0f) {
        return 0; // gain explicitly disabled for this item
    }
    // Always advance at least 1: weak early enemies give only a few EXP, so a 10% share would otherwise
    // floor to 0 and items would never progress on a new save.
    f32 exp = (f32)enemyExp * pct / 100.0f;
    return (u16)CLAMP(exp, 1.0f, 65535.0f);
}

// Maps the actor that landed a hit to a LeveledItemId. `source` is the player for melee weapons, or a
// player-spawned projectile actor otherwise. Returns -1 when the source isn't a leveled weapon.
static s32 Leveled_MapAttackerToItem(Actor* source, Player* player) {
    if (source == NULL) {
        return -1;
    }
    if (source->id == ACTOR_PLAYER) {
        // Melee: the weapon being swung is whatever the player currently holds.
        switch (player->heldItemAction) {
            case PLAYER_IA_SWORD_KOKIRI: return LEVELED_ITEM_KOKIRI_SWORD;
            case PLAYER_IA_SWORD_MASTER: return LEVELED_ITEM_MASTER_SWORD;
            case PLAYER_IA_SWORD_BIGGORON: return LEVELED_ITEM_BIGGORON_SWORD;
            case PLAYER_IA_DEKU_STICK: return LEVELED_ITEM_DEKU_STICK;
            case PLAYER_IA_HAMMER: return LEVELED_ITEM_HAMMER;
            default: return -1;
        }
    }
    switch (source->id) {
        case ACTOR_EN_ARROW:
            switch (source->params) {
                case LEVELED_ARROW_SEED: return LEVELED_ITEM_SLINGSHOT;
                case LEVELED_ARROW_FIRE: return LEVELED_ITEM_FIRE_ARROW;
                case LEVELED_ARROW_ICE: return LEVELED_ITEM_ICE_ARROW;
                case LEVELED_ARROW_LIGHT: return LEVELED_ITEM_LIGHT_ARROW;
                case LEVELED_ARROW_NUT: return LEVELED_ITEM_DEKU_NUT; // thrown deku nut is an En_Arrow
                default: return LEVELED_ITEM_BOW; // normal/lit/horse/silent arrows
            }
        case ACTOR_EN_BOM: return LEVELED_ITEM_BOMB;
        case ACTOR_EN_BOM_CHU: return LEVELED_ITEM_BOMBCHU;
        case ACTOR_EN_BOOM: return LEVELED_ITEM_BOOMERANG;
        case ACTOR_ARMS_HOOK: return LEVELED_ITEM_HOOKSHOT;
        case ACTOR_MAGIC_FIRE: return LEVELED_ITEM_DINS_FIRE; // Din's Fire ring deals contact damage
        default: return -1;
    }
}

// Only enemies/bosses grant item EXP, so contact with breakables, switches, signs, etc. earns nothing.
static s32 Leveled_IsExpBearingEnemy(Actor* target) {
    return target != NULL && (target->category == ACTORCAT_ENEMY || target->category == ACTORCAT_BOSS);
}

// Per-weapon damage multiplier reached at the item's level cap (the ".DamageCap" CVar default). Items not
// listed here don't scale damage (deku nut, boomerang, hookshot, and all passive equipment).
static f32 Leveled_DefaultDamageCap(s32 item) {
    switch (item) {
        case LEVELED_ITEM_KOKIRI_SWORD:   return 4.0f;
        case LEVELED_ITEM_MASTER_SWORD:   return 4.0f;
        case LEVELED_ITEM_BIGGORON_SWORD: return 5.0f;
        case LEVELED_ITEM_DEKU_STICK:     return 5.0f;
        case LEVELED_ITEM_HAMMER:         return 3.0f;
        case LEVELED_ITEM_SLINGSHOT:      return 4.0f;
        case LEVELED_ITEM_BOW:            return 3.0f;
        case LEVELED_ITEM_FIRE_ARROW:     return 3.0f;
        case LEVELED_ITEM_ICE_ARROW:      return 3.0f;
        case LEVELED_ITEM_LIGHT_ARROW:    return 3.0f;
        case LEVELED_ITEM_BOMB:           return 2.5f;
        case LEVELED_ITEM_BOMBCHU:        return 2.5f;
        case LEVELED_ITEM_DINS_FIRE:      return 3.0f;
        default:                          return 1.0f;
    }
}

f32 Leveled_GetWeaponDamageMult(Actor* source, Player* player) {
    if (!CVarGetInteger("gLeveled.Master", 1) || player == NULL) {
        return 1.0f;
    }
    s32 item = Leveled_MapAttackerToItem(source, player);
    if (item < 0) {
        return 1.0f;
    }
    return Leveled_GetItemEffectMult(item, ".DamageCap", Leveled_DefaultDamageCap(item));
}

// Scales a magic cost by the item's ".MagicCap" multiplier (1.0x at level 1 down to the configured floor,
// default 0.5x, at the level cap). Never drops below 1. Returns baseCost unchanged for unmapped items.
static s16 Leveled_ApplyMagicCost(s32 item, s16 baseCost) {
    s32 cost = (s32)(baseCost * Leveled_GetItemEffectMult(item, ".MagicCap", 0.5f) + 0.5f);
    return (s16)CLAMP_MIN(cost, 1);
}

s16 Leveled_ModifyMagicArrowCost(s32 magicArrowType, s16 baseCost) {
    switch (magicArrowType) { // 0=fire, 1=ice, 2=light (arrowType - ARROW_FIRE)
        case 0:  return Leveled_ApplyMagicCost(LEVELED_ITEM_FIRE_ARROW, baseCost);
        case 1:  return Leveled_ApplyMagicCost(LEVELED_ITEM_ICE_ARROW, baseCost);
        case 2:  return Leveled_ApplyMagicCost(LEVELED_ITEM_LIGHT_ARROW, baseCost);
        default: return baseCost;
    }
}

s16 Leveled_ModifyMagicSpellCost(s32 magicSpell, s16 baseCost) {
    switch (magicSpell) { // index into sMagicSpellCosts (actionParam - PLAYER_IA_MAGIC_SPELL_15)
        case 3:  return Leveled_ApplyMagicCost(LEVELED_ITEM_FARORES_WIND, baseCost);
        case 4:  return Leveled_ApplyMagicCost(LEVELED_ITEM_NAYRUS_LOVE, baseCost);
        case 5:  return Leveled_ApplyMagicCost(LEVELED_ITEM_DINS_FIRE, baseCost);
        default: return baseCost;
    }
}

// Stun items (these earn EXP on contact but scale stun duration rather than damage).
static s32 Leveled_IsStunItem(s32 item) {
    return item == LEVELED_ITEM_DEKU_NUT || item == LEVELED_ITEM_BOOMERANG || item == LEVELED_ITEM_HOOKSHOT;
}

// Attribution cache: the collision hook records which stun item last struck which enemy, and
// Actor_SetColorFilter consumes it one-shot to lengthen the resulting stun. Actor_SetColorFilter has no
// PlayState, so matching is by actor pointer + one-shot consume (the enemy stuns ~1 frame after the hit).
static const Actor* sStunTarget = NULL;
static s32 sStunItem = -1;

void Leveled_AwardWeaponExpOnHit(Actor* target, Actor* source, PlayState* play) {
    // One award per enemy per frame so multi-sphere melee swings don't multiply the EXP.
    static const Actor* sLastTarget = NULL;
    static u32 sLastFrame = 0xFFFFFFFF;

    if (play == NULL || !CVarGetInteger("gLeveled.Master", 1) || !Leveled_IsExpBearingEnemy(target)) {
        return;
    }
    if (sLastTarget == target && sLastFrame == play->gameplayFrames) {
        return;
    }
    Player* player = GET_PLAYER(play);
    s32 item = Leveled_MapAttackerToItem(source, player);
    if (item < 0) {
        return;
    }
    sLastTarget = target;
    sLastFrame = play->gameplayFrames;
    // Remember stun-item contact so the stun it triggers can be lengthened (consumed in ScaleStunDuration).
    if (Leveled_IsStunItem(item)) {
        sStunTarget = target;
        sStunItem = item;
    }
    Leveled_GainItemExp(item, Leveled_ItemExpShare(item, (u16)target->exp, 50.0f));
}

s16 Leveled_ScaleStunDuration(Actor* actor, s16 duration) {
    if (!CVarGetInteger("gLeveled.Master", 1) || actor == NULL || actor != sStunTarget || sStunItem < 0) {
        return duration;
    }
    s32 item = sStunItem;
    sStunTarget = NULL; // one-shot: a given hit lengthens only the stun it directly causes
    sStunItem = -1;
    f32 scaled = (f32)duration * Leveled_GetItemEffectMult(item, ".DurationCap", 3.0f);
    return (s16)CLAMP(scaled, 1.0f, 255.0f); // colorFilterTimer is u8
}

s32 Leveled_DekuStickSurvives(void) {
    if (!CVarGetInteger("gLeveled.Master", 1) || !CVarGetInteger("gLeveled.Item.EnableCItems", 1)) {
        return 0;
    }
    // Chance to avoid breaking ramps from 0 at level 1 to the configured cap (% ) at the level cap.
    f32 maxChance = CVarGetFloat("gLeveled.Item.DekuStick.DurabilityCap", 80.0f) / 100.0f;
    return Rand_ZeroOne() < maxChance * Leveled_GetItemLevelFraction(LEVELED_ITEM_DEKU_STICK);
}

s16 Leveled_GetLensDrainInterval(s16 baseInterval) {
    // Longer interval between magic ticks = slower drain. 1.0x at level 1 up to the configured cap.
    f32 scaled = (f32)baseInterval * Leveled_GetItemEffectMult(LEVELED_ITEM_LENS, ".DrainCap", 2.0f);
    return (s16)CLAMP(scaled, 1.0f, 0x7FFF);
}

void Leveled_AwardOnEnemyDefeated(Player* player, u16 enemyExp) {
    static const s32 tunicItem[PLAYER_TUNIC_MAX] = {
        LEVELED_ITEM_KOKIRI_TUNIC, LEVELED_ITEM_GORON_TUNIC, LEVELED_ITEM_ZORA_TUNIC,
    };
    // Indexed by PlayerBoots; the indoor/underwater/child variants credit their "real" counterpart.
    static const s32 bootsItem[PLAYER_BOOTS_MAX] = {
        LEVELED_ITEM_KOKIRI_BOOTS, LEVELED_ITEM_IRON_BOOTS, LEVELED_ITEM_HOVER_BOOTS,
        LEVELED_ITEM_KOKIRI_BOOTS, LEVELED_ITEM_IRON_BOOTS, LEVELED_ITEM_KOKIRI_BOOTS,
    };

    if (!CVarGetInteger("gLeveled.Master", 1) || enemyExp == 0) {
        return;
    }
    if (player != NULL) {
        s8 tunic = player->currentTunic;
        if (tunic >= 0 && tunic < PLAYER_TUNIC_MAX) {
            Leveled_GainItemExp(tunicItem[tunic], Leveled_ItemExpShare(tunicItem[tunic], enemyExp, 10.0f));
        }
        s8 boots = player->currentBoots;
        if (boots >= 0 && boots < PLAYER_BOOTS_MAX) {
            Leveled_GainItemExp(bootsItem[boots], Leveled_ItemExpShare(bootsItem[boots], enemyExp, 10.0f));
        }
    }
    // Nayru's Love only counts if it is shielding Link at the moment the enemy dies.
    if (gSaveContext.nayrusLoveTimer != 0) {
        Leveled_GainItemExp(LEVELED_ITEM_NAYRUS_LOVE,
                            Leveled_ItemExpShare(LEVELED_ITEM_NAYRUS_LOVE, enemyExp, 10.0f));
    }
}

void Leveled_AwardShieldBlockExp(Player* player, Actor* attacker) {
    static const s32 shieldItem[PLAYER_SHIELD_MAX] = {
        -1, LEVELED_ITEM_DEKU_SHIELD, LEVELED_ITEM_HYLIAN_SHIELD, LEVELED_ITEM_MIRROR_SHIELD,
    };

    if (!CVarGetInteger("gLeveled.Master", 1) || player == NULL || attacker == NULL) {
        return;
    }
    s8 shield = player->currentShield;
    if (shield <= PLAYER_SHIELD_NONE || shield >= PLAYER_SHIELD_MAX) {
        return;
    }
    s32 item = shieldItem[shield];
    // Shields earn from the EXP of whatever attacked them (5% default), but only enemy attacks count.
    if (item < 0 || !Leveled_IsExpBearingEnemy(attacker)) {
        return;
    }
    Leveled_GainItemExp(item, Leveled_ItemExpShare(item, (u16)attacker->exp, 5.0f));
}

void Leveled_AwardStunExp(Actor* target, s32 item) {
    // Stun items only earn from an enemy that was not already stunned (no farming a frozen target).
    if (!CVarGetInteger("gLeveled.Master", 1) || !Leveled_IsExpBearingEnemy(target)) {
        return;
    }
    Leveled_GainItemExp(item, Leveled_ItemExpShare(item, (u16)target->exp, 50.0f));
}

void Leveled_AwardStaticExp(s32 item, const char* expSuffix, u16 defaultExp) {
    if (!CVarGetInteger("gLeveled.Master", 1)) {
        return;
    }
    char buf[96];
    snprintf(buf, sizeof(buf), "%s%s", Leveled_GetItemCvarPrefix(item), expSuffix);
    Leveled_GainItemExp(item, (u16)CVarGetInteger(buf, defaultExp));
}