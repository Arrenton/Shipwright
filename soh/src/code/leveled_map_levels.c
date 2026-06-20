#include "global.h"
#include "leveled_actor_level_table.h"

// Per-area level table. The CVar default IS the pre-determined level, so an unset CVar reproduces
// vanilla behavior and the menu slider (defaulting to the same value) shows the current level at a
// glance. Dual-age overworld scenes have a separate child and adult entry.
const LeveledAreaEntry gLeveledAreaTable[] = {
    // Dungeons
    { "Deku Tree",                "gLeveled.AreaOverride.DekuTree",         2,  0, { SCENE_DEKU_TREE, SCENE_DEKU_TREE_BOSS, -1, -1, -1, -1 } },
    { "Dodongo's Cavern",         "gLeveled.AreaOverride.DodongosCavern",   10, 0, { SCENE_DODONGOS_CAVERN, SCENE_DODONGOS_CAVERN_BOSS, -1, -1, -1, -1 } },
    { "Jabu-Jabu's Belly",        "gLeveled.AreaOverride.JabuJabu",         16, 0, { SCENE_JABU_JABU, SCENE_JABU_JABU_BOSS, -1, -1, -1, -1 } },
    { "Forest Temple",            "gLeveled.AreaOverride.ForestTemple",     24, 0, { SCENE_FOREST_TEMPLE, SCENE_FOREST_TEMPLE_BOSS, -1, -1, -1, -1 } },
    { "Fire Temple",              "gLeveled.AreaOverride.FireTemple",       30, 0, { SCENE_FIRE_TEMPLE, SCENE_FIRE_TEMPLE_BOSS, -1, -1, -1, -1 } },
    { "Water Temple",             "gLeveled.AreaOverride.WaterTemple",      35, 0, { SCENE_WATER_TEMPLE, SCENE_WATER_TEMPLE_BOSS, -1, -1, -1, -1 } },
    { "Spirit Temple",            "gLeveled.AreaOverride.SpiritTemple",     45, 0, { SCENE_SPIRIT_TEMPLE, SCENE_SPIRIT_TEMPLE_BOSS, -1, -1, -1, -1 } },
    { "Shadow Temple",            "gLeveled.AreaOverride.ShadowTemple",     40, 0, { SCENE_SHADOW_TEMPLE, SCENE_SHADOW_TEMPLE_BOSS, -1, -1, -1, -1 } },
    { "Bottom of the Well",       "gLeveled.AreaOverride.BottomOfTheWell",  38, 0, { SCENE_BOTTOM_OF_THE_WELL, -1, -1, -1, -1, -1 } },
    { "Ice Cavern",               "gLeveled.AreaOverride.IceCavern",        32, 0, { SCENE_ICE_CAVERN, -1, -1, -1, -1, -1 } },
    { "Gerudo Training Ground",   "gLeveled.AreaOverride.GerudoTraining",   43, 0, { SCENE_GERUDO_TRAINING_GROUND, -1, -1, -1, -1, -1 } },
    { "Thieves' Hideout",         "gLeveled.AreaOverride.ThievesHideout",   42, 0, { SCENE_THIEVES_HIDEOUT, -1, -1, -1, -1, -1 } },
    { "Ganon's Castle",           "gLeveled.AreaOverride.GanonsCastle",     52, 0, { SCENE_INSIDE_GANONS_CASTLE, SCENE_GANONS_TOWER, SCENE_GANONDORF_BOSS, SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR, SCENE_GANON_BOSS, -1 } },
    { "Ganon's Castle (Collapse)","gLeveled.AreaOverride.GanonsCastleCollapse", 53, 0, { SCENE_GANONS_TOWER_COLLAPSE_INTERIOR, SCENE_INSIDE_GANONS_CASTLE_COLLAPSE, -1, -1, -1, -1 } },
    // Towns / graves / single-age areas
    { "Market Ruins",             "gLeveled.AreaOverride.MarketRuins",      23, 0, { SCENE_MARKET_RUINS, -1, -1, -1, -1, -1 } },
    { "Grave (ReDead)",           "gLeveled.AreaOverride.RedeadGrave",      10, 0, { SCENE_REDEAD_GRAVE, -1, -1, -1, -1, -1 } },
    { "Royal Family's Tomb",      "gLeveled.AreaOverride.RoyalTomb",        8,  0, { SCENE_ROYAL_FAMILYS_TOMB, -1, -1, -1, -1, -1 } },
    { "Windmill / Dampe's Grave", "gLeveled.AreaOverride.Windmill",         22, 0, { SCENE_WINDMILL_AND_DAMPES_GRAVE, -1, -1, -1, -1, -1 } },
    { "House of Skulltula",       "gLeveled.AreaOverride.SkulltulaHouse",   99, 0, { SCENE_HOUSE_OF_SKULLTULA, -1, -1, -1, -1, -1 } },
    { "Lon Lon Ranch",            "gLeveled.AreaOverride.LonLonRanch",      6,  0, { SCENE_LON_LON_RANCH, -1, -1, -1, -1, -1 } },
    { "Death Mountain Crater",    "gLeveled.AreaOverride.DMCrater",         11, 0, { SCENE_DEATH_MOUNTAIN_CRATER, -1, -1, -1, -1, -1 } },
    { "Desert Colossus",          "gLeveled.AreaOverride.DesertColossus",   42, 0, { SCENE_DESERT_COLOSSUS, -1, -1, -1, -1, -1 } },
    { "Gerudo's Fortress",        "gLeveled.AreaOverride.GerudosFortress",  40, 0, { SCENE_GERUDOS_FORTRESS, -1, -1, -1, -1, -1 } },
    { "Outside Ganon's Castle",   "gLeveled.AreaOverride.OutsideGanon",     25, 0, { SCENE_OUTSIDE_GANONS_CASTLE, -1, -1, -1, -1, -1 } },
    // Overworld (child / adult)
    { "Hyrule Field (Child)",     "gLeveled.AreaOverride.HyruleFieldChild", 5,  1, { SCENE_HYRULE_FIELD, -1, -1, -1, -1, -1 } },
    { "Hyrule Field (Adult)",     "gLeveled.AreaOverride.HyruleFieldAdult", 20, 2, { SCENE_HYRULE_FIELD, -1, -1, -1, -1, -1 } },
    { "Graveyard (Child)",        "gLeveled.AreaOverride.GraveyardChild",   7,  1, { SCENE_GRAVEYARD, -1, -1, -1, -1, -1 } },
    { "Graveyard (Adult)",        "gLeveled.AreaOverride.GraveyardAdult",   20, 2, { SCENE_GRAVEYARD, -1, -1, -1, -1, -1 } },
    { "Zora's River (Child)",     "gLeveled.AreaOverride.ZorasRiverChild",  8,  1, { SCENE_ZORAS_RIVER, -1, -1, -1, -1, -1 } },
    { "Zora's River (Adult)",     "gLeveled.AreaOverride.ZorasRiverAdult",  24, 2, { SCENE_ZORAS_RIVER, -1, -1, -1, -1, -1 } },
    { "Kokiri Forest (Child)",    "gLeveled.AreaOverride.KokiriChild",      2,  1, { SCENE_KOKIRI_FOREST, -1, -1, -1, -1, -1 } },
    { "Kokiri Forest (Adult)",    "gLeveled.AreaOverride.KokiriAdult",      21, 2, { SCENE_KOKIRI_FOREST, -1, -1, -1, -1, -1 } },
    { "Sacred Forest Meadow (Child)","gLeveled.AreaOverride.SfmChild",      9,  1, { SCENE_SACRED_FOREST_MEADOW, -1, -1, -1, -1, -1 } },
    { "Sacred Forest Meadow (Adult)","gLeveled.AreaOverride.SfmAdult",      23, 2, { SCENE_SACRED_FOREST_MEADOW, -1, -1, -1, -1, -1 } },
    { "Lake Hylia (Child)",       "gLeveled.AreaOverride.LakeHyliaChild",   7,  1, { SCENE_LAKE_HYLIA, -1, -1, -1, -1, -1 } },
    { "Lake Hylia (Adult)",       "gLeveled.AreaOverride.LakeHyliaAdult",   22, 2, { SCENE_LAKE_HYLIA, -1, -1, -1, -1, -1 } },
    { "Zora's Fountain (Child)",  "gLeveled.AreaOverride.ZorasFountainChild", 13, 1, { SCENE_ZORAS_FOUNTAIN, -1, -1, -1, -1, -1 } },
    { "Zora's Fountain (Adult)",  "gLeveled.AreaOverride.ZorasFountainAdult", 30, 2, { SCENE_ZORAS_FOUNTAIN, -1, -1, -1, -1, -1 } },
    { "Gerudo Valley (Child)",    "gLeveled.AreaOverride.GerudoValleyChild", 10, 1, { SCENE_GERUDO_VALLEY, -1, -1, -1, -1, -1 } },
    { "Gerudo Valley (Adult)",    "gLeveled.AreaOverride.GerudoValleyAdult", 28, 2, { SCENE_GERUDO_VALLEY, -1, -1, -1, -1, -1 } },
    { "Lost Woods (Child)",       "gLeveled.AreaOverride.LostWoodsChild",   7,  1, { SCENE_LOST_WOODS, -1, -1, -1, -1, -1 } },
    { "Lost Woods (Adult)",       "gLeveled.AreaOverride.LostWoodsAdult",   22, 2, { SCENE_LOST_WOODS, -1, -1, -1, -1, -1 } },
    { "Death Mountain Trail (Child)","gLeveled.AreaOverride.DMTrailChild",  8,  1, { SCENE_DEATH_MOUNTAIN_TRAIL, -1, -1, -1, -1, -1 } },
    { "Death Mountain Trail (Adult)","gLeveled.AreaOverride.DMTrailAdult",  24, 2, { SCENE_DEATH_MOUNTAIN_TRAIL, -1, -1, -1, -1, -1 } },
};
const s32 gLeveledAreaTableCount = ARRAY_COUNT(gLeveledAreaTable);

s16 Leveled_GetSceneLevel(s16 sceneId) {
    // Ganon's Castle optionally jumps to level 120 with all 100 tokens. Takes priority over overrides.
    if ((sceneId == SCENE_INSIDE_GANONS_CASTLE || sceneId == SCENE_GANONS_TOWER ||
         sceneId == SCENE_GANONDORF_BOSS || sceneId == SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR ||
         sceneId == SCENE_GANON_BOSS) &&
        CVarGetInteger("gLeveled.Difficulty.Enemy.MaxLevelInGanonCastle", 0) == 1 &&
        gSaveContext.inventory.gsTokens == 100) {
        return 120;
    }

    u8 isChild = LINK_IS_CHILD;
    for (s32 i = 0; i < gLeveledAreaTableCount; i++) {
        const LeveledAreaEntry* a = &gLeveledAreaTable[i];
        if (a->age == 1 && !isChild) {
            continue; // child-only entry, but Link is an adult
        }
        if (a->age == 2 && isChild) {
            continue; // adult-only entry, but Link is a child
        }
        for (s32 j = 0; j < 6 && a->sceneIds[j] >= 0; j++) {
            if (a->sceneIds[j] == sceneId) {
                return (s16)CVarGetInteger(a->cvar, a->defaultLevel);
            }
        }
    }
    return -1; // not a leveled area
}
