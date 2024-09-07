/*
Author: PapaReap
*/

[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class PR_AIPatrolTriggerClass : PR_CoreTriggerClass
{
	// prefab properties here
}

enum PR_EGroupUS
{
	"MUST CHOOSE GROUP!"			= -1,
	"Man: AR (1)"					= 0, //{C153572E4A1724B4}Prefabs/Groups/BLUFOR/Group_US_AR_M.et
	"Man: AT (1)"					= 1, //{7C8305A9350C38AA}Prefabs/Groups/BLUFOR/Group_US_AT_M.et
	"Man: GL (1)" 					= 2, //{1B692CF8F695C2FA}Prefabs/Groups/BLUFOR/Group_US_GL_M.et
	"Man: MG (1)"					= 3, //{543E9CAF67ED912D}Prefabs/Groups/BLUFOR/Group_US_MG_M.et
	"Man: Medic (1)" 				= 4, //{EB297CF916CF1277}Prefabs/Groups/BLUFOR/Group_US_Medic_M.et
	"Man: Officer (1)" 				= 5, //{1671C06D39508596}Prefabs/Groups/BLUFOR/Group_US_Officer_M.et
	"Man: Randomized (1)" 			= 6, //{5F0B6659A5594AB1}Prefabs/Groups/BLUFOR/Group_US_Randomized_M.et
	"Man: Rifleman (1)"				= 7, //{1C6439B48BCA6D77}Prefabs/Groups/BLUFOR/Group_US_Rifleman_M.et
	"Man: RTO (1)" 					= 8, //{C246071DBF6AD7D9}Prefabs/Groups/BLUFOR/Group_US_RTO_M.et
	"Man: Sapper (1)"				= 9, //{ABE8F3539944B6BD}Prefabs/Groups/BLUFOR/Group_US_Sapper_M.et
	"Man: Sniper (1)" 				= 10, //{DF2F568D5A2A8F13}Prefabs/Groups/BLUFOR/Group_US_Sniper_M.et
	"Man: Spotter (1)" 				= 11, //{7A32D63CD61C9C60}Prefabs/Groups/BLUFOR/Group_US_Spotter_M.et
	"Man: SL (1)"					= 12, //{64D6EFA39E5190DE}Prefabs/Groups/BLUFOR/Group_US_SL_M.et
	"Man: Unarmed (1)" 				= 13, //{71379F19E18ED274}Prefabs/Groups/BLUFOR/Group_US_Unarmed_M.et
	"Man SF: Grenadier (1)" 		= 14, //{4ACCB999D9E24889}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Grenadier_M.et
	"Man SF: LMG (1)" 				= 15, //{B9339A61F368AB3C}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_LMG_M.et
	"Man SF: Medic (1)" 			= 16, //{3CF7506E3F5873B4}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Medic_M.et
	"Man SF: Officer (1)"			= 17, //{AB87CDAB4444D3E1}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Officer_M.et
	"Man SF: Rifleman (1)" 			= 18, //{2F71D82DE8A1E379}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Rifleman_M.et
	"Man SF: RTO (1)" 				= 19, //{BF9204E1603997ED}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_RTO_M.et
	"Man SF: Sapper (1)" 			= 20, //{B124BF29FDE612DA}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Sapper_M.et
	"Man SF: Sharpshooter (1)"	 	= 21, //{7579F08D3CFFE330}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Sharpshooter_M.et
	"Man SF: SL (1)"	 			= 22, //{E29113E030AE676B}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_SL_M.et
	"Group: Ammo Team (4)"			= 23, //{F72EF3429D8C8DF5}Prefabs/Groups/BLUFOR/Group_US_AmmoTeam.et
	"Group: AT Team (4)" 			= 24, //{FAEA8B9E1252F56E}Prefabs/Groups/BLUFOR/Group_US_Team_LAT.et
	"Group: Fire Team (4)" 			= 25, //{84E5BBAB25EA23E5}Prefabs/Groups/BLUFOR/Group_US_FireTeam.et
	"Group: Light Fire Team (4)" 	= 26, //{FCF7F5DC4F83955C}Prefabs/Groups/BLUFOR/Group_US_LightFireTeam.et
	"Group: GL Team (2)" 			= 27, //{DE747BC9217D383C}Prefabs/Groups/BLUFOR/Group_US_Team_GL.et
	"Group: MG Team (2)" 			= 28, //{958039B857396B7B}Prefabs/Groups/BLUFOR/Group_US_MachineGunTeam.et
	"Group: Medical Section (2)" 	= 29, //{EF62027CC75A7459}Prefabs/Groups/BLUFOR/Group_US_MedicalSection.et
	"Group: Platoon HQ (4)" 		= 30, //{B7AB5D3F8A7ADAE4}Prefabs/Groups/BLUFOR/Group_US_PlatoonHQ.et
	"Group: Recon Team (2)" 		= 31, //{F65B7BB712F46FEE}Prefabs/Groups/BLUFOR/Group_US_ReconTeam.et
	"Group: Rifle Squad (9)"		= 32, //{DDF3799FA1387848}Prefabs/Groups/BLUFOR/Group_US_RifleSquad.et
	"Group: Sapper Team (2)" 		= 33, //{9624D2B39397E148}Prefabs/Groups/BLUFOR/Group_US_SapperTeam.et
	"Group: Sentry Team (2)" 		= 34, //{3BF36BDEEB33AEC9}Prefabs/Groups/BLUFOR/Group_US_SentryTeam.et
	"Group: Sniper Team (2)"		= 35, //{D807C7047E818488}Prefabs/Groups/BLUFOR/Group_US_SniperTeam.et
	"Group: Suppress Team (4)"		= 36, //{81B6DBF2B88545F5}Prefabs/Groups/BLUFOR/Group_US_Team_Suppress.et
	"Group SF: Sentry Team (2)" 	= 37, //{35681BE27C302FF5}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_GreenBeret_SentryTeam.et
	"Group SF: Squad (6)" 			= 38, //{D0886786634E55AE}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_GreenBeret_Squad.et
}

enum PR_EGroupUSSR
{
	"MUST CHOOSE GROUP!"			= -1,
	"Man: AR (1)"					= 0,
	"Man: AT (1)"					= 1,
	"Man: GL (1)" 					= 2,
	"Man: MG (1)"					= 3,
	"Man: Medic (1)" 				= 4,
	"Man: Officer (1)" 				= 5,
	"Man: Randomized (1)" 			= 6,
	"Man: Rifleman (1)"				= 7,
	"Man: RTO (1)" 					= 8,
	"Man: Sapper (1)"				= 9,
	"Man: Scout (1)" 				= 10,
	"Man: Sharpshooter (1)" 		= 11,
	"Man: SL (1)"					= 12,
	"Man: Unarmed (1)" 				= 13,
	"Man SF: Grenadier (1)" 		= 14,
	"Man SF: LMG (1)" 				= 15,
	"Man SF: Medic (1)" 			= 16,
	"Man SF: Officer (1)"			= 17,
	"Man SF: Rifleman (1)" 			= 18,
	"Man SF: RTO (1)" 				= 19,
	"Man SF: Sapper (1)" 			= 20,
	"Man SF: Sharpshooter (1)"	 	= 21,
	"Man SF: SL (1)"	 			= 22,
	"Group: Ammo Team (4)"			= 23,
	"Group: AT Team (4)" 			= 24,
	"Group: Fire Team (4)" 			= 25,
	"Group: Light Fire Team (4)" 	= 26,
	"Group: GL Team (2)" 			= 27,
	"Group: Maneuver Group (2)" 	= 28,
	"Group: MG Team (2)" 			= 29,
	"Group: Medical Section (2)" 	= 30,
	"Group: Platoon HQ (5)" 		= 31,
	"Group: Rifle Squad (6)"		= 32,
	"Group: Sapper Team (2)" 		= 33,
	"Group: Sentry Team (2)" 		= 34,
	"Group: Suppress Team (4)"		= 35,
	"Group SF: Sentry Team (2)" 	= 36,
	"Group SF: Squad (6)" 			= 37,
}

enum PR_EGroupFIA
{
	"MUST CHOOSE GROUP!"			= -1,
	"Man: AT (1)"					= 0,
	"Man: Machine Gun (1)"			= 1,
	"Man: Medic (1)" 				= 2,
	"Man: Randomized (1)" 			= 3,
	"Man: Rifleman (1)"				= 4,
	"Man: RTO (1)" 					= 5,
	"Man: Sapper (1)"				= 6,
	"Man: Sharpshooter (1)" 		= 7,
	"Man: Squad Leader (1)"			= 8,
	"Man: Unarmed (1)" 				= 9,
	"Man SF: Grenadier (1)" 		= 10,
	"Man SF: Medic (1)" 			= 11,
	"Man SF: Rifleman (1)" 			= 12,
	"Man SF: Scout (1)" 			= 13,
	"Group: Ammo Team (4)"			= 14,
	"Group: AT Team (4)" 			= 15,
	"Group: Fire Team (5)" 			= 16,
	"Group: Light Fire Team (4)" 	= 17,
	"Group: Machine Gun Team (2)" 	= 18,
	"Group: Medical Section (2)" 	= 19,
	"Group: Platoon HQ (3)" 		= 20,
	"Group: Recon Team (2)" 		= 21,
	"Group: Rifle Squad (7)"		= 22,
	"Group: Sapper Team (2)" 		= 23,
	"Group: Sentry Team (2)" 		= 24,
	"Group: Sharpshooter Team (2)"	= 25,
	"Group: Suppress Team (4)"		= 26,
}

enum PR_EGroupCiv
{
	"MUST CHOOSE GROUP!"						= -1,
	"Man: Businessman: Randomized (1)"			= 0,
	"Man: ConstructionWorker: Randomized (1)"	= 1,
	"Man: Dockworker: Randomized (1)"			= 2,
	"Man: GenericCivilian: Randomized (1)"		= 3,
	"Group: Businessmen (3)"					= 4,
	"Group: ConstructionWorkers (5)"			= 5,
	"Group: Dockworkers (6)"					= 6,
	"Group: Generic Civilians Cotton Shirt (6)"	= 7,
	"Group: Generic Civilians Denim Jacket (2)"	= 8,
	"Group: Generic Civilians Turtleneck (3)"	= 9,
}

enum PR_ETeleportSortOrder
{
	"ASCENDING" = 0,
	"DESCENDING" = 1,
	"RAMDOM DIRECTION" = 2,
	"RAMDOM MIX" = 3,
	"RANDOM ONE" = 4,
}

enum PR_ECollectionSortOrder
{
	"ASCENDING" = 0,
	"DESCENDING" = 1,
	"RAMDOM DIRECTION" = 2,
	"RAMDOM MIX" = 3,
}

enum PR_EWaypointSortOrder
{
	"ASCENDING" = 0,
	"DESCENDING" = 1,
	"RAMDOM DIRECTION" = 2,
	"RAMDOM MIX" = 3,
}

enum PR_ECollectionSpawn
{
	"ALL" = 0,
	"RANDOM ONE" = 1,
	"RAMDOM 25%" = 2,
	"RAMDOM 50%" = 3,
	"RAMDOM 75%" = 4,
}

//! Main Script
class PR_AIPatrolTrigger : PR_CoreTrigger
{
	//! PR Task Spawner: EPF Persistence - Use Enfusion Persistent Framework
	[Attribute("false", UIWidgets.CheckBox,"Neutralize Persistent Object when all group members are dead.  ", category: "PR Core: EPF Persistence")]
	protected bool m_bNeutralizePersistentObjectIfGroupIsDead;

	[Attribute(desc: "Individual Groups to spawn.  ", category: "PR Spawn Group Details")]
	ref array<ref PR_Group_US> m_aSideUS;

	[Attribute(desc: "Individual Groups to spawn.  ", category: "PR Spawn Group Details")]
	ref array<ref PR_Group_USSR> m_aSideUSSR;

	[Attribute(desc: "Individual Groups to spawn.  ", category: "PR Spawn Group Details")]
	ref array<ref PR_Group_FIA> m_aSideFIA;

	[Attribute(desc: "Individual Groups to spawn.  ", category: "PR Spawn Group Details")]
	ref array<ref PR_Group_Civilian> m_aSideCIV;

	private ref PR_SpawnPatrol m_PR_SpawnPatrol;
	private int groupSide;
	private int groupType;

	//! Group Details
	//private string spawnPosition = "";
	private ref array<string> spawnPosition = {};
	private bool randomGroupSize = 0;
	private int minUnitsInGroup = 1;
	private int maxUnitsInGroup = -1;
	private bool keepGroupActive = 0;
	private bool logGroupLocation = 0;
	private int logUpdateInterval = 5;
	private bool suspendIfNoPlayers = 1;
	private bool resetGroupIfIdle = 0;
	private bool spawnVehicle = 0;
	private ref array<string> groupIDArray = {};
	private string groupID;
	private ref array<string> prefabArray = {};
	private string specificPrefabName = "";
	//! Waypoints
	private bool cycleWaypoints = 0;
	private int rerunCounter = -1;
	private ref array<string> waypointCollection = {};
	private PR_ECollectionSortOrder collectionSortOrder = 0;
	private PR_EWaypointSortOrder waypointSortOrder = 0;
	private PR_ECollectionSpawn spawnCollections = 0;
	//! Behaviours
	private SCR_EAIGroupFormation groupFormation = 0;
	private EAISkill aISkill = 50;
	private EAICombatType aICombatType = 1;
	//private bool m_bHoldFire = 0;
	private float perceptionFactor = 1;
	//! Teleport Group
	private bool teleportAfterSpawn = 0;
	private ref array<string> teleportPosition = {};
	private PR_ETeleportSortOrder teleportSortOrder = 0;
	//! Respawn Group
	private int respawnCount = 0;
	private int respawnTimerMin = 0;
	private bool useRandomRespawnTimer = 0;
	private int respawnTimerMax = 0;
	private int _i = 0;
	protected void ResetTypes()
	{
		//! Group Details
		//spawnPosition = "";
		spawnPosition = {};
		randomGroupSize = 0;
		minUnitsInGroup = 1;
		maxUnitsInGroup = -1;
		keepGroupActive = 0;
		logGroupLocation = 0;
		logUpdateInterval = 5;
		suspendIfNoPlayers = 1;
		resetGroupIfIdle = 0;
		spawnVehicle = 0;
		groupIDArray = {};
		prefabArray = {};
		specificPrefabName = "";
		//! Waypoints
		cycleWaypoints = 0;
		rerunCounter = -1;
		waypointCollection = {};
		collectionSortOrder = 0;
		waypointSortOrder = 0;
		spawnCollections = 0;
		//! Behaviours
		groupFormation = 0;
		aISkill = 50;
		aICombatType = 1;
	//	m_bHoldFire = 0;
		perceptionFactor = 1;
		//! Teleport Group
		teleportAfterSpawn = 0;
		teleportPosition = {};
		teleportSortOrder = 0;
		//! Respawn Group
		respawnCount = 0;
		respawnTimerMin = 0;
		useRandomRespawnTimer = 0;
		respawnTimerMax = 0;
	}

	//------------------------------------------------------------------------------------------------
	//! EOnInit
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);

		if (m_bIsTestingMode)
			SpawnPatrolInit();
	}

	override protected event void OnActivate(IEntity ent)
	{
		super.OnActivate(ent);

		if (!Replication.IsServer())
			return;

		if (m_bIsTriggerActivated)
			return;

		m_bIsTriggerActivated = true;

		SpawnPatrolInit();

		Deactivate();
	}

	//------------------------------------------------------------------------------------------------
	//! Gather spawn information
	protected void SpawnPatrolInit()
	{
		m_sLogMode = "(OnActivate)";

		if (m_aSideUS.Count() > 0)
		{
			foreach (PR_Group_US detailsUS : m_aSideUS)
			{
				if (!detailsUS.m_bEnableGroup)
					continue;

				groupType = detailsUS.m_GroupType;
				if (groupType == -1)
					continue;

				groupSide = 0;
				ResetTypes();
				
				groupID = detailsUS.m_sGroupID;
				if (groupID)
					groupIDArray.Insert(groupID);
				
				GetGroupDetails(detailsUS.m_aGroupDetails);
				GetGroupWaypoints(detailsUS.m_aGroupWaypoints);
				GetGroupBehaviours(detailsUS.m_aGroupBehaviours);
				GetTeleportGroup(detailsUS.m_aTeleportGroup);
				GetRespawnGroup(detailsUS.m_aRespawnGroup);
				GetDebugLogs("(PR_Group_US)");
				if (!m_bIsTestingMode)
					CallSpawnPatrol();
			}
		}

		if (m_aSideUSSR.Count() > 0)
		{
			foreach (PR_Group_USSR detailsUSSR : m_aSideUSSR)
			{
				if (!detailsUSSR.m_bEnableGroup)
					continue;

				groupType = detailsUSSR.m_GroupType;
				if (groupType == -1)
					continue;

				groupSide = 1;
				ResetTypes();

				groupID = detailsUSSR.m_sGroupID;
				if (groupID)
					groupIDArray.Insert(groupID);	
				
				GetGroupDetails(detailsUSSR.m_aGroupDetails);
				GetGroupWaypoints(detailsUSSR.m_aGroupWaypoints);
				GetGroupBehaviours(detailsUSSR.m_aGroupBehaviours);
				GetTeleportGroup(detailsUSSR.m_aTeleportGroup);
				GetRespawnGroup(detailsUSSR.m_aRespawnGroup);
				GetDebugLogs("(PR_Group_USSR)");
				if (!m_bIsTestingMode)
					CallSpawnPatrol();
			}

		}

		if (m_aSideFIA.Count() > 0)
		{
			foreach (PR_Group_FIA detailsFIA : m_aSideFIA)
			{
				if (!detailsFIA.m_bEnableGroup)
					continue;

				groupType = detailsFIA.m_GroupType;
				if (groupType == -1)
					continue;
				
				groupSide = 2;
				ResetTypes();

				groupID = detailsFIA.m_sGroupID;
				if (groupID)
					groupIDArray.Insert(groupID);
				
				GetGroupDetails(detailsFIA.m_aGroupDetails);
				GetGroupWaypoints(detailsFIA.m_aGroupWaypoints);
				GetGroupBehaviours(detailsFIA.m_aGroupBehaviours);
				GetTeleportGroup(detailsFIA.m_aTeleportGroup);
				GetRespawnGroup(detailsFIA.m_aRespawnGroup);
				GetDebugLogs("(PR_Group_FIA)");
				if (!m_bIsTestingMode)
					CallSpawnPatrol();
			}
		}

		if (m_aSideCIV.Count() > 0)
		{
			foreach (PR_Group_Civilian detailsCIV : m_aSideCIV)
			{
				if (!detailsCIV.m_bEnableGroup)
					continue;

				groupType = detailsCIV.m_GroupType;
				if (groupType == -1)
					continue;
				
				groupSide = 3;
				ResetTypes();

				groupID = detailsCIV.m_sGroupID;
				if (groupID)
					groupIDArray.Insert(groupID);
				
				GetGroupDetails(detailsCIV.m_aGroupDetails);
				GetGroupWaypoints(detailsCIV.m_aGroupWaypoints);
				GetGroupBehaviours(detailsCIV.m_aGroupBehaviours);
				GetTeleportGroup(detailsCIV.m_aTeleportGroup);
				GetRespawnGroup(detailsCIV.m_aRespawnGroup);
				GetDebugLogs("(PR_Group_Civilian)");
				if (!m_bIsTestingMode)
					CallSpawnPatrol();
			}
		}
		
		if (groupIDArray.Count() > 0)
			Print(string.Format("[PR_AIPatrolTrigger] (SpawnPatrolInit) %1 : groupID: %2", m_sLogMode, groupIDArray.Get(0)), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void CallSpawnPatrol()
	{
		m_sLogMode = "(OnActivate)";

		//--- Persistence
		PersistenceCleanup();

		int delay = m_iDelayTimerMin * 1000;

		if (m_bUseRandomDelayTimer)
			delay = Math.RandomInt(m_iDelayTimerMin * 1000, m_iDelayTimerMax * 1000);

		if (m_bDebugLogs)
		{
			Print(("[PR_AIPatrolTrigger] (CallSpawnPatrol) " + m_sLogMode + " : Trigger Name: " + m_sTriggerName), LogLevel.NORMAL);
			Print((
			"[PR_AIPatrolTrigger] (CallSpawnPatrol) " + m_sLogMode + " : Info: Group side: " + groupSide +
			", Spawn group type: " + groupType +
			", Group ID: " + groupID +
			", Spawn position: " + spawnPosition +
			", Delay: " + delay
			), LogLevel.NORMAL);
		}
		
		//--- Call to spawner script on trigger activation
		m_PR_SpawnPatrol = new PR_SpawnPatrol();
		
		//IEntity spawnPos = GetGame().GetWorld().FindEntityByName(spawnPosition);
	//	vector location = m_Trigger.GetOrigin(); //trying to set directions
	//	Print(string.Format("[PR_AIPatrolTrigger] %1 : location: %2", m_sLogMode, location), LogLevel.NORMAL);
	//	Print(string.Format("[PR_AIPatrolTrigger] %1 : m_Trigger.GetOrigin(): %2", m_sLogMode, m_Trigger.GetOrigin()), LogLevel.NORMAL);
		
		//if (!spawnPos)
		IEntity trigger = m_World.FindEntityByID(this.GetID());

		if (useRandomRespawnTimer)
		{
			if (respawnTimerMax <= respawnTimerMin)
				respawnTimerMax = respawnTimerMin + 1;
		};

		protected array<array<string>> m_sStringArray = {
			spawnPosition,			// 0
			teleportPosition,		// 1
			waypointCollection,		// 2
			prefabArray,			// 3
			groupIDArray,			// 4
		};

		if (teleportPosition.Count() == 0)
			teleportPosition.Insert(trigger.GetName());

		protected array<bool> m_aBoolArray = {
			cycleWaypoints,			// 0
			m_bDebugLogs,			// 1
			useRandomRespawnTimer,	// 2
			randomGroupSize,		// 3
			keepGroupActive,		// 4
			logGroupLocation,		// 5
			suspendIfNoPlayers,		// 6
			spawnVehicle,			// 7
			teleportAfterSpawn,		// 8
			m_bNeutralizePersistentObjectIfGroupIsDead, // 9
			resetGroupIfIdle		// 10
		};
		protected array<int> m_aIntArray = {
			rerunCounter,			// 0
			respawnTimerMin,		// 1
			respawnTimerMax,		// 2
			respawnCount,			// 3
			teleportSortOrder,		// 4
			collectionSortOrder,	// 5
			waypointSortOrder,		// 6
			spawnCollections,		// 7
			aISkill,				// 8
			aICombatType,			// 9
			groupFormation,			// 10
			minUnitsInGroup,		// 11
			maxUnitsInGroup,		// 12
			logUpdateInterval		// 13
		};

		//--- Execute the AI spawning using a delayed call
		GetGame().GetCallqueue().CallLater(
			m_PR_SpawnPatrol.PRSpawnPatrol,
			delay,
			false,
			groupSide,
			groupType,
			//trigger.GetOrigin(),
			trigger,
			m_aBoolArray,
			m_sStringArray,
			m_aIntArray,
			perceptionFactor,
			m_PersistentObject
		);
	}

	protected void GetDebugLogs(string logs)
	{
		Print(string.Format("[PR_AIPatrolTrigger] %1 : logs: %2", m_sLogMode, logs), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : groupSide: %2", m_sLogMode, groupSide), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : spawnPosition: %2", m_sLogMode, spawnPosition), LogLevel.NORMAL);

		Print(string.Format("[PR_AIPatrolTrigger] %1 : randomGroupSize: %2", m_sLogMode, randomGroupSize), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : minUnitsInGroup: %2", m_sLogMode, minUnitsInGroup), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : maxUnitsInGroup: %2", m_sLogMode, maxUnitsInGroup), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : keepGroupActive: %2", m_sLogMode, keepGroupActive), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : logGroupLocation: %2", m_sLogMode, logGroupLocation), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : logUpdateInterval: %2", m_sLogMode, logUpdateInterval), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : suspendIfNoPlayers: %2", m_sLogMode, suspendIfNoPlayers), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : resetGroupIfIdle: %2", m_sLogMode, resetGroupIfIdle), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : spawnVehicle: %2", m_sLogMode, spawnVehicle), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : prefabArray: %2", m_sLogMode, prefabArray), LogLevel.NORMAL);

		Print(string.Format("[PR_AIPatrolTrigger] %1 : cycleWaypoints: %2", m_sLogMode, cycleWaypoints), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : rerunCounter: %2", m_sLogMode, rerunCounter), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : waypointCollection: %2", m_sLogMode, waypointCollection), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : collectionSortOrder: %2", m_sLogMode, collectionSortOrder), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : waypointSortOrder: %2", m_sLogMode, waypointSortOrder), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : spawnCollections: %2", m_sLogMode, spawnCollections), LogLevel.NORMAL);

		Print(string.Format("[PR_AIPatrolTrigger] %1 : groupFormation: %2", m_sLogMode, groupFormation), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : aISkill: %2", m_sLogMode, aISkill), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : aICombatType: %2", m_sLogMode, aICombatType), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : perceptionFactor: %2", m_sLogMode, perceptionFactor), LogLevel.NORMAL);

		Print(string.Format("[PR_AIPatrolTrigger] %1 : teleportAfterSpawn: %2", m_sLogMode, teleportAfterSpawn), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : teleportPosition: %2", m_sLogMode, teleportPosition), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : teleportSortOrder: %2", m_sLogMode, teleportSortOrder), LogLevel.NORMAL);

		Print(string.Format("[PR_AIPatrolTrigger] %1 : respawnCount: %2", m_sLogMode, respawnCount), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : respawnTimerMin: %2", m_sLogMode, respawnTimerMin), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : useRandomRespawnTimer: %2", m_sLogMode, useRandomRespawnTimer), LogLevel.NORMAL);
		Print(string.Format("[PR_AIPatrolTrigger] %1 : respawnTimerMax: %2", m_sLogMode, respawnTimerMax), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void GetGroupDetails(array<ref PR_GroupDetails> groupDetails)
	{
		if (groupDetails)
		{
			foreach (PR_GroupDetails detailsInfo : groupDetails)
			{
				spawnPosition = detailsInfo.m_aSpawnPosition; // //m_sSpawnPosition
				randomGroupSize = detailsInfo.m_bRandomGroupSize;
				minUnitsInGroup = detailsInfo.m_iMinUnitsInGroup;
				maxUnitsInGroup = detailsInfo.m_iMaxUnitsInGroup;
				keepGroupActive = detailsInfo.m_bKeepGroupActive;
				logGroupLocation = detailsInfo.m_bLogGroupLocation;
				logUpdateInterval = detailsInfo.m_iLogUpdateInterval;
				suspendIfNoPlayers = detailsInfo.m_bSuspendIfNoPlayers;
				resetGroupIfIdle = detailsInfo.m_bResetGroupIfIdle;
				spawnVehicle = detailsInfo.m_bSpawnVehicle;
				if (spawnVehicle)
				{
					specificPrefabName = detailsInfo.m_aSpecificPrefabName;
					prefabArray.Insert(specificPrefabName);
				}
				
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void GetGroupWaypoints(array<ref PR_GroupWaypoints> groupWaypoints)
	{
		if (groupWaypoints)
		{
			foreach (PR_GroupWaypoints waypointInfo : groupWaypoints)
			{
				cycleWaypoints = waypointInfo.m_bCycleWaypoints;
				rerunCounter = waypointInfo.m_iRerunCounter;
				waypointCollection = waypointInfo.m_aWaypointCollection;
				collectionSortOrder = waypointInfo.m_CollectionSortOrder;
				waypointSortOrder = waypointInfo.m_WaypointSortOrder;
				spawnCollections = waypointInfo.m_SpawnCollections;
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void GetGroupBehaviours(array<ref PR_GroupBehaviours> groupBehaviours)
	{
		if (groupBehaviours)
		{
			foreach (PR_GroupBehaviours behavior : groupBehaviours)
			{
				groupFormation = behavior.m_eAIGroupFormation;
				aISkill = behavior.m_eAISkill;
				aICombatType = behavior.m_eAICombatType;
				perceptionFactor = behavior.m_fPerceptionFactor;
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void GetTeleportGroup(array<ref PR_TeleportGroup> teleportGroup)
	{
		if (teleportGroup)
		{
			foreach (PR_TeleportGroup teleportInfo : teleportGroup)
			{
				teleportAfterSpawn = teleportInfo.m_bTeleportAfterSpawn;
				teleportPosition = teleportInfo.m_aTeleportPosition;
				teleportSortOrder = teleportInfo.m_TeleportSortOrder;
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void GetRespawnGroup(array<ref PR_RespawnGroup> respawnGroup)
	{
		if (respawnGroup)
		{
			foreach (PR_RespawnGroup respawnInfo : respawnGroup)
			{
				respawnCount = respawnInfo.m_iRespawnCount;
				respawnTimerMin = respawnInfo.m_iRespawnTimerMin;
				useRandomRespawnTimer = respawnInfo.m_bUseRandomRespawnTimer;
				respawnTimerMax = respawnInfo.m_iRespawnTimerMax;
			}
		}
	}
}

//! US Groups
[BaseContainerProps()]
class PR_Group_US
{
	[Attribute("true", UIWidgets.CheckBox,"Disabling group will keep it in place without having to reenter all info. The group will not be spawned or used. ", category: "PR Spawn Patrol: Group")]
	bool m_bEnableGroup;

	[Attribute("-1", UIWidgets.ComboBox, "Group to spawn, unit count inside ( ).  ", enums: ParamEnumArray.FromEnum(PR_EGroupUS), category: "PR Spawn Patrol: Group")]
	PR_EGroupUS m_GroupType;
	
	//! PR SPAWN PATROL: GROUP - ID Name to give spawned group. Use for identification, log tracking, etc...
	[Attribute(desc: "ID Name to give spawned group. Use for identification, log tracking, etc...  ", category: "PR Spawn Patrol: Group")]
	string m_sGroupID;

	[Attribute(desc: "Group details. Includes: Set spawn location, keep active, suspend if no players. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupDetails> m_aGroupDetails;

	[Attribute(desc: "Group waypoint information. Includes: cycle, rerun counter, waypoint collection, sorting. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupWaypoints> m_aGroupWaypoints;

	[Attribute(desc: "Group behaviour traits. Includes: Formation, skill, combat type & perception factor. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupBehaviours> m_aGroupBehaviours;

	[Attribute(desc: "Teleport group to position after spawn? Useful for hard to place areas, like over water, towers, etc... Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_TeleportGroup> m_aTeleportGroup;

	[Attribute(desc: "Respawn group if all units are dead. Includes: Respawn count, timer, randome timer. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_RespawnGroup> m_aRespawnGroup;
}

//! USSR Groups
[BaseContainerProps()]
class PR_Group_USSR
{
	[Attribute("true", UIWidgets.CheckBox,"Disabling group will keep it in place without having to reenter all info. The group will not be spawned or used. ", category: "PR Spawn Patrol: Group")]
	bool m_bEnableGroup;

	[Attribute("-1", UIWidgets.ComboBox, "Group to spawn, unit count inside ( ).  ", enums: ParamEnumArray.FromEnum(PR_EGroupUSSR), category: "PR Spawn Patrol: Group")]
	PR_EGroupUSSR m_GroupType;

	//! PR SPAWN PATROL: GROUP - ID Name to give spawned group. Use for identification, log tracking, etc...
	[Attribute(desc: "ID Name to give spawned group. Use for identification, log tracking, etc...  ", category: "PR Spawn Patrol: Group")]
	string m_sGroupID;
	
	[Attribute(desc: "Group details. Includes: Set spawn location, keep active, suspend if no players. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupDetails> m_aGroupDetails;

	[Attribute(desc: "Group waypoint information. Includes: cycle, rerun counter, waypoint collection, sorting. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupWaypoints> m_aGroupWaypoints;

	[Attribute(desc: "Group behaviour traits. Includes: Formation, skill, combat type & perception factor. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupBehaviours> m_aGroupBehaviours;

	[Attribute(desc: "Teleport group to position after spawn? Useful for hard to place areas, like over water, towers, etc... Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_TeleportGroup> m_aTeleportGroup;

	[Attribute(desc: "Respawn group if all units are dead. Includes: Respawn count, timer, randome timer. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_RespawnGroup> m_aRespawnGroup;
}

//! FIA Groups
[BaseContainerProps()]
class PR_Group_FIA
{
	[Attribute("true", UIWidgets.CheckBox,"Disabling group will keep it in place without having to reenter all info. The group will not be spawned or used. ", category: "PR Spawn Patrol: Group")]
	bool m_bEnableGroup;

	[Attribute("-1", UIWidgets.ComboBox, "Group to spawn, unit count inside ( ).  ", enums: ParamEnumArray.FromEnum(PR_EGroupFIA), category: "PR Spawn Patrol: Group")]
	PR_EGroupFIA m_GroupType;

	//! PR SPAWN PATROL: GROUP - ID Name to give spawned group. Use for identification, log tracking, etc...
	[Attribute(desc: "ID Name to give spawned group. Use for identification, log tracking, etc...  ", category: "PR Spawn Patrol: Group")]
	string m_sGroupID;
	
	[Attribute(desc: "Group details. Includes: Set spawn location, keep active, suspend if no players. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupDetails> m_aGroupDetails;

	[Attribute(desc: "Group waypoint information. Includes: cycle, rerun counter, waypoint collection, sorting. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupWaypoints> m_aGroupWaypoints;

	[Attribute(desc: "Group behaviour traits. Includes: Formation, skill, combat type & perception factor. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupBehaviours> m_aGroupBehaviours;

	[Attribute(desc: "Teleport group to position after spawn? Useful for hard to place areas, like over water, towers, etc... Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_TeleportGroup> m_aTeleportGroup;

	[Attribute(desc: "Respawn group if all units are dead. Includes: Respawn count, timer, randome timer. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_RespawnGroup> m_aRespawnGroup;
}

//! Civilian Groups
[BaseContainerProps()]
class PR_Group_Civilian
{
	[Attribute("true", UIWidgets.CheckBox,"Disabling group will keep it in place without having to reenter all info. The group will not be spawned or used. ", category: "PR Spawn Patrol: Group")]
	bool m_bEnableGroup;

	[Attribute("-1", UIWidgets.ComboBox, "Group to spawn, unit count inside ( ).  ", enums: ParamEnumArray.FromEnum(PR_EGroupCiv), category: "PR Spawn Patrol: Group")]
	PR_EGroupCiv m_GroupType;

	//! PR SPAWN PATROL: GROUP - ID Name to give spawned group. Use for identification, log tracking, etc...
	[Attribute(desc: "ID Name to give spawned group. Use for identification, log tracking, etc...  ", category: "PR Spawn Patrol: Group")]
	string m_sGroupID;
	
	[Attribute(desc: "Group details. Includes: Set spawn location, keep active, suspend if no players. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupDetails> m_aGroupDetails;

	[Attribute(desc: "Group waypoint information. Includes: cycle, rerun counter, waypoint collection, sorting. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupWaypoints> m_aGroupWaypoints;

	[Attribute(desc: "Group behaviour traits. Includes: Formation, skill, combat type & perception factor. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_GroupBehaviours> m_aGroupBehaviours;

	[Attribute(desc: "Teleport group to position after spawn? Useful for hard to place areas, like over water, towers, etc... Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_TeleportGroup> m_aTeleportGroup;

	[Attribute(desc: "Respawn group if all units are dead. Includes: Respawn count, timer, randome timer. Use Only One! ", category: "PR Spawn Patrol: Group")]
	ref array<ref PR_RespawnGroup> m_aRespawnGroup;
}

//! Group Details
[BaseContainerProps()]
class PR_GroupDetails
{
	//! PR SPAWN PATROL: GROUP - Group spawn position: Object name to spawn on. If none given group will spawn on this triggers position.
	[Attribute(desc: "Object name to spawn on. If none given, group will spawn on this triggers position. Can be a collection of positions, will pick random one.  ", category: "PR Spawn Patrol: Group")]
	//string m_sSpawnPosition;
	ref array<string> m_aSpawnPosition;

	[Attribute(defvalue: "false", desc: "Randomize group size based on a random min and max values below.  ", category: "PR Spawn Patrol: Group")]
	bool m_bRandomGroupSize;

	[Attribute(defvalue: "1", desc: "Minimum amount of AIs in the group.  ", category: "PR Spawn Patrol: Group")]
	int m_iMinUnitsInGroup;

	[Attribute(defvalue: "-1", desc: "Maximum amount of AIs in the group. -1 has no effect, will use min value. If value is higher than actual group size, units will be added to group until max amount reached.  ", category: "PR Spawn Patrol: Group")]
	int m_iMaxUnitsInGroup;
	
	//! PR SPAWN PATROL: GROUP - Use random respawn timer: Use random respawn timer. Uses a min value from above and max value below.
	[Attribute("false", UIWidgets.CheckBox,"Group will continue movement even outside of player range, their simulation will not be stopped. By default groups cache at around 1km. NOTE: Use with caution, too many units might degrade server performance.  ", category: "PR Spawn Patrol: Group")]
	bool m_bKeepGroupActive;

	//! PR SPAWN PATROL: GROUP - Suspend the groups movement if no active players in mission.
	[Attribute("true", UIWidgets.CheckBox,"Suspend the groups movement if no active players in mission.  ", category: "PR Spawn Patrol: Group")]
	bool m_bSuspendIfNoPlayers;

	//! PR SPAWN PATROL: GROUP - Removes existing group and spawns new group if idle too long.
	[Attribute("false", UIWidgets.CheckBox,"Reset group if idle too long (no movement) or can't complete given waypoints.  ", category: "PR Spawn Patrol: Group")]
	bool m_bResetGroupIfIdle;
	
	[Attribute(defvalue: "0", desc: "Amount of time group is idle (no movement) before being reset. In minutes.  ", category: "PR Spawn Patrol: Group")]
	int m_iResetGroupTimer;
		
	//! PR SPAWN PATROL: GROUP - Write logs of group location, for debugging purposes.
	[Attribute("false", UIWidgets.CheckBox,"Write logs of group location, for debugging purposes.  ", category: "PR Spawn Patrol: Group")]
	bool m_bLogGroupLocation;

	[Attribute(defvalue: "5", desc: "Interval to update logs, in minutes.  ", category: "PR Spawn Patrol: Group")]
	int m_iLogUpdateInterval;
	
	//! PR SPAWN PATROL: GROUP - Spawn a vehicle to use for patrol.
	[Attribute("false", UIWidgets.CheckBox,"Spawn a vehicle to use for patrol.  ", category: "PR Spawn Patrol: Group")]
	bool m_bSpawnVehicle;
	
	//--- PR Spawn Prefab Details: Prefab type to spawn
	[Attribute("{19C7E8B968404775}Prefabs/Vehicles/Wheeled/UAZ469/AI/UAZ469_ai.et", params: "et", desc: "If SPECIFIC_PREFAB_NAME is selected, fill the class name here.", category: "PR Spawn Prefab Details")]
	ResourceName m_aSpecificPrefabName;
}

//! Group Waypoints
[BaseContainerProps()]
class PR_GroupWaypoints
{
	//! PR SPAWN PATROL: WAYPOINT - Cycle Waypoint: Enable waypoint cycle? Check box to cycle through all given waypoints.
	[Attribute("false", UIWidgets.CheckBox,"Enable waypoint cycle? Check box to cycle through all given waypoints.  ", category: "PR Spawn Patrol: Waypoint")]
	bool m_bCycleWaypoints;

	//! PR SPAWN PATROL: WAYPOINT - Cycle Rerun Counter: Decides how many times the cycled waypoint will run for the specific group. -1 means infinate.
	[Attribute("-1", desc: "Decides how many times the cycled waypoint will run for the specific group. -1 means infinate.  ", category: "PR Spawn Patrol: Waypoint")]
	int m_iRerunCounter;

	//! PR SPAWN PATROL: WAYPOINT - Waypoint Set: Name of object holding a set of waypoints or just the name of the waypoint itself.
	[Attribute("", desc: "Name of object holding a set of waypoints or just the name of the waypoint itself.  ", category: "PR Spawn Patrol: Waypoint")]
	ref array<string> m_aWaypointCollection;

	//! PR SPAWN PATROL: WAYPOINT - Collection Sort Order: What order will the collections be given to the group from 'Waypoint Collections'.
	[Attribute("0", UIWidgets.ComboBox, "What order will the collections be given to the group from 'Waypoint Collections'.  ", enums: ParamEnumArray.FromEnum(PR_ECollectionSortOrder), category: "PR Spawn Patrol: Waypoint")]
	PR_ECollectionSortOrder m_CollectionSortOrder;

	//! PR SPAWN PATROL: WAYPOINT - Waypoint Sort Order: What order will the waypoints be given to the group from within each individual 'Waypoint Collections'.
	[Attribute("0", UIWidgets.ComboBox, "What order will the waypoints be given to the group from within each individual 'Waypoint Collections'.  ", enums: ParamEnumArray.FromEnum(PR_EWaypointSortOrder), category: "PR Spawn Patrol: Waypoint")]
	PR_EWaypointSortOrder m_WaypointSortOrder;

	//! PR SPAWN PATROL: WAYPOINT - Spawn Collections: How many collections to use from 'Waypoint Collections'. If % is used, it will round down. Always a min of 1 collection.
	[Attribute("0", UIWidgets.ComboBox, "How many collections to use from 'Waypoint Collections'. If % is used, it will round down. Always a min of 1 collection.  ", enums: ParamEnumArray.FromEnum(PR_ECollectionSpawn), category: "PR Spawn Patrol: Waypoint")]
	PR_ECollectionSpawn m_SpawnCollections;
}

//! Group Behaviors
[BaseContainerProps()]
class PR_GroupBehaviours
{
	//! PR SPAWN PATROL: BEHAVIOR - Group Formation: AI Group formation
	[Attribute(defvalue: SCR_EAIGroupFormation.Wedge.ToString(), UIWidgets.ComboBox, "AI Group formation", "", ParamEnumArray.FromEnum(SCR_EAIGroupFormation), category: "PR Spawn Patrol: Behavior")]
	SCR_EAIGroupFormation m_eAIGroupFormation; // done

	//! PR SPAWN PATROL: BEHAVIOR - AISkill: AI skill in combat
	[Attribute(defvalue: EAISkill.REGULAR.ToString(), UIWidgets.ComboBox, "AI skill in combat", "", ParamEnumArray.FromEnum(EAISkill), category: "PR Spawn Patrol: Behavior")]
	EAISkill m_eAISkill; // done

	//! PR SPAWN PATROL: BEHAVIOR - Combat Type: AI combat type
	[Attribute(defvalue: EAICombatType.NORMAL.ToString(), UIWidgets.ComboBox, "AI combat type", "", ParamEnumArray.FromEnum(EAICombatType), category: "PR Spawn Patrol: Behavior")]
	EAICombatType m_eAICombatType; // done

	//! PR SPAWN PATROL: BEHAVIOR - Hold Fire: If AI in the group should hold fire
//	[Attribute(defvalue: "0", desc: "If AI in the group should hold fire", category: "PR Spawn Patrol: Behavior")]
//	bool m_bHoldFire = 0; // done

	//! PR SPAWN PATROL: BEHAVIOR - Perception Factor: Sets perception ability. Affects speed at which perception detects targets. Bigger value means proportionally faster detection.
	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Sets perception ability. Affects speed at which perception detects targets. Bigger value means proportionally faster detection.", params: "0 100 0.001", category: "PR Spawn Patrol: Behavior")]
	float m_fPerceptionFactor;
}

//! Teleport Group
[BaseContainerProps()]
class PR_TeleportGroup
{
	//! PR SPAWN PATROL: TELEPORT TO POSITION
	[Attribute("false", UIWidgets.CheckBox,"Teleport group to position after spawn? Useful for hard to place areas, like over water, towers, etc...  ", category: "PR Spawn Patrol: SPECIAL: Teleport To Position")]
	bool m_bTeleportAfterSpawn;

	//! PR SPAWN PATROL: TELEPORT TO POSITION
	[Attribute("", desc: "Object name to teleport to. If more than one is used, a random spot will be used. If none given, group will teleport to 'Spawn Position' from above.  ", category: "PR Spawn Patrol: SPECIAL: Teleport To Position")]
	ref array<string> m_aTeleportPosition;

	//! PR SPAWN PATROL: WAYPOINT - Collection Sort Order: What order will the collections be given to the group from 'Waypoint Collections'.
	[Attribute("0", UIWidgets.ComboBox, "What order will the teleport positions be given to the group from 'Teleport Position'.  ", enums: ParamEnumArray.FromEnum(PR_ETeleportSortOrder), category: "PR Spawn Patrol: SPECIAL: Teleport To Position")]
	PR_ETeleportSortOrder m_TeleportSortOrder;
}

//! Respawn Group
[BaseContainerProps()]
class PR_RespawnGroup
{
	//! PR SPAWN PATROL: GROUP - Group respawn: How many times will the group respawn. (0 = no respawn, -1 = infinite respawn)
	[Attribute("0", UIWidgets.EditBox, "How many times will the group respawn. (0 = no respawn, -1 = infinite respawn)", "-1 inf 1", category: "PR Spawn Patrol: Group")]
	int m_iRespawnCount;

	//! PR SPAWN PATROL: GROUP - Respawn timer min value: How much time before the group will respawn. Minimum value if used with random timer. (seconds)
	[Attribute("0", UIWidgets.EditBox, "How much time before the group will respawn. Minimum value if used with random timer. (seconds)  ", "0 inf 1", category: "PR Spawn Patrol: Group")]
	int m_iRespawnTimerMin;

	//! PR SPAWN PATROL: GROUP - Use random respawn timer: Uses a min value from above and max value below.
	[Attribute("false", UIWidgets.CheckBox,"Use random respawn timer. Uses a min value from above and max value below.  ", category: "PR Spawn Patrol: Group")]
	bool m_bUseRandomRespawnTimer;

	//! PR SPAWN PATROL: GROUP - Respawn timer max value: How much time before the group will respawn. Maximum value if used with random timer. (seconds)
	[Attribute("0", UIWidgets.EditBox, "How much time before the group will respawn. Maximum value if used with random timer. (seconds)  ", "0 inf 1", category: "PR Spawn Patrol: Group")]
	int m_iRespawnTimerMax;
}
