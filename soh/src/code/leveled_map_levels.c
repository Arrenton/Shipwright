#include "global.h"

s8 Leveled_GetSceneLevel(s16 sceneId) {
	switch (sceneId) {
	case SCENE_YDAN:
	case SCENE_YDAN_BOSS:
		return 2; // Deku Tree

	case SCENE_DDAN:
	case SCENE_DDAN_BOSS:
		return 10; // Dodongo's

	case SCENE_BDAN:
	case SCENE_BDAN_BOSS:
		return 16; // Jabu Jabu

	case SCENE_BMORI1:
	case SCENE_MORIBOSSROOM:
		return 24; // Forest Temple

	case SCENE_HIDAN:
	case SCENE_FIRE_BS:
		return 30; // Fire Temple

	case SCENE_MIZUSIN:
	case SCENE_MIZUSIN_BS:
		return 35; // Water Temple

	case SCENE_JYASINZOU:
	case SCENE_JYASINBOSS:
		return 45; // Spirit Temple

	case SCENE_HAKADAN:
	case SCENE_HAKADAN_BS:
		return 40; // Shadow Temple

	case SCENE_HAKADANCH: 
		return 38; // Bottom of the Well

	case SCENE_ICE_DOUKUTO: 
		return 32; // Ice Cavern

	case SCENE_GANON:
	case SCENE_GANON_BOSS:
	case SCENE_GANON_FINAL:
	case SCENE_GANON_DEMO:
		return 50; // Ganon's Tower

	case SCENE_MEN: 
		return 43; // Gerudo Training Grounds

	case SCENE_GERUDOWAY: 
		return 42; // Thieve's Hideout

	case SCENE_GANONTIKA: 
		return 52; // Inside Ganon's Castle

	case SCENE_GANON_SONOGO: 
		// Tower Collapse
	case SCENE_GANONTIKA_SONOGO: 
		return 53; // Castle Collapse

	case SCENE_MARKET_RUINS:
		return 23;

	case SCENE_HAKAANA: 
		return 10; // Redead Grave

	case SCENE_HAKAANA_OUKE: 
		return 8; // Royal Family's Tomb

	case SCENE_HAKASITARELAY: 
		return 22; // Dampe's Grave

	case SCENE_KINSUTA: 
		return 99; // House of Skulltula

	case SCENE_SPOT00: 
		if (LINK_IS_CHILD)
			return 5;
		else // Hyrule Field
			return 20;

	case SCENE_SPOT02: 
		if (LINK_IS_CHILD)
			return 7;
		else // Graveyard
			return 20;

	case SCENE_SPOT03:
		if (LINK_IS_CHILD)
			return 8;
		else // Zora's River
			return 24;

	case SCENE_SPOT04:
		if (LINK_IS_CHILD)
			return 3;
		else // Kokiri Forest
			return 21;

	case SCENE_SPOT05:
		if (LINK_IS_CHILD)
			return 9;
		else // Sacred Forest Meadow
			return 23;
		
	case SCENE_SPOT06:
		if (LINK_IS_CHILD)
			return 7;
		else // Lake Hylia
			return 22;
		
	case SCENE_SPOT08:
		if (LINK_IS_CHILD)
			return 13;
		else // Zora's Fountain
			return 30;
		
	case SCENE_SPOT09:
		if (LINK_IS_CHILD)
			return 10;
		else // Gerudo Valley
			return 28;
		
	case SCENE_SPOT10:
		if (LINK_IS_CHILD)
			return 7;
		else // Lost Woods
			return 22;
		
	case SCENE_SPOT11: 
		return 42; // Desert Colossus

	case SCENE_SPOT12: 
		return 40; // Gerudo's Fortress

	case SCENE_SPOT16:
		if (LINK_IS_CHILD)
			return 8; // Death Mountain Trail
		else
			return 24;

	case SCENE_SPOT17: 
		return 11; // Death Mountain Crater

	case SCENE_SPOT20: 
		return 6;// Lon Lon Ranch

	case SCENE_GANON_TOU: 
		return 25; // Outside Ganon's Castle
	

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