#include "global.h"

s8 Leveled_GetSceneLevel(s16 sceneId) {
	switch (sceneId) {
	case SCENE_DEKU_TREE:
	case SCENE_DEKU_TREE_BOSS:
		return 2; // Deku Tree

	case SCENE_DODONGOS_CAVERN:
        case SCENE_DODONGOS_CAVERN_BOSS:
		return 10; // Dodongo's

	case SCENE_JABU_JABU:
	case SCENE_JABU_JABU_BOSS:
		return 16; // Jabu Jabu

	case SCENE_FOREST_TEMPLE:
	case SCENE_FOREST_TEMPLE_BOSS:
		return 24; // Forest Temple

	case SCENE_FIRE_TEMPLE:
	case SCENE_FIRE_TEMPLE_BOSS:
		return 30; // Fire Temple

	case SCENE_WATER_TEMPLE:
	case SCENE_WATER_TEMPLE_BOSS:
		return 35; // Water Temple

	case SCENE_SPIRIT_TEMPLE:
	case SCENE_SPIRIT_TEMPLE_BOSS:
		return 45; // Spirit Temple

	case SCENE_SHADOW_TEMPLE:
	case SCENE_SHADOW_TEMPLE_BOSS:
		return 40; // Shadow Temple

	case SCENE_BOTTOM_OF_THE_WELL: 
		return 38; // Bottom of the Well

	case SCENE_ICE_CAVERN: 
		return 32; // Ice Cavern

	case SCENE_GANONS_TOWER:
    case SCENE_GANONDORF_BOSS:
	case SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR:
	case SCENE_GANON_BOSS:
		return 50;

	case SCENE_GERUDO_TRAINING_GROUND: 
		return 43;

	case SCENE_THIEVES_HIDEOUT: 
		return 42;

	case SCENE_INSIDE_GANONS_CASTLE: 
		return 52;

	case SCENE_GANONS_TOWER_COLLAPSE_INTERIOR:
    case SCENE_INSIDE_GANONS_CASTLE_COLLAPSE: 
		return 53;

	case SCENE_MARKET_RUINS:
		return 23;

	case SCENE_REDEAD_GRAVE: 
		return 10;

	case SCENE_ROYAL_FAMILYS_TOMB: 
		return 8;

	case SCENE_WINDMILL_AND_DAMPES_GRAVE: 
		return 22;

	case SCENE_HOUSE_OF_SKULLTULA: 
		return 99;

	case SCENE_HYRULE_FIELD: 
		if (LINK_IS_CHILD)
			return 5;
		else
			return 20;

	case SCENE_GRAVEYARD: 
		if (LINK_IS_CHILD)
			return 7;
		else
			return 20;

	case SCENE_ZORAS_RIVER:
		if (LINK_IS_CHILD)
			return 8;
		else
			return 24;

	case SCENE_KOKIRI_FOREST:
		if (LINK_IS_CHILD)
			return 2;
		else
			return 21;

	case SCENE_SACRED_FOREST_MEADOW:
		if (LINK_IS_CHILD)
			return 9;
		else
			return 23;
		
	case SCENE_LAKE_HYLIA:
		if (LINK_IS_CHILD)
			return 7;
		else
			return 22;
		
	case SCENE_ZORAS_FOUNTAIN:
		if (LINK_IS_CHILD)
			return 13;
		else
			return 30;
		
	case SCENE_GERUDO_VALLEY:
		if (LINK_IS_CHILD)
			return 10;
		else
			return 28;
		
	case SCENE_LOST_WOODS:
		if (LINK_IS_CHILD)
			return 7;
		else
			return 22;
		
	case SCENE_DESERT_COLOSSUS: 
		return 42;

	case SCENE_GERUDOS_FORTRESS: 
		return 40;

	case SCENE_DEATH_MOUNTAIN_TRAIL:
		if (LINK_IS_CHILD)
			return 8;
		else
			return 24;

	case SCENE_DEATH_MOUNTAIN_CRATER: 
		return 11;

	case SCENE_LON_LON_RANCH: 
		return 6;

	case SCENE_OUTSIDE_GANONS_CASTLE: 
		return 25;
	

	// Debug only scenes
	case SCENE_TEST01: 
		// Test Map
	case SCENE_BESITU: 
		// Test Room
	case SCENE_DEPTH_TEST: 
		// Depth Test
	case SCENE_SYOTES: 
		// Stalfos Mini-Boss
	case SCENE_SYOTES2: 
		// Stalfos Boss
	case SCENE_SUTARU: 
		// Dark Link
	case SCENE_HAIRAL_NIWA2: 
		// Castle Maze (Broken)
	case SCENE_SASATEST: 
		// SRD Room
	case SCENE_TESTROOM: 
		// Chest Room
		return 45;

	default:
		return -1;
	}
	return -1;
}