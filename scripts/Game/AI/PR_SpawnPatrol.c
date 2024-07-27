/*
Author: PapaReap. Inspired from Rabid Squirrel's code from scripts SCR_RS_AISpawner, SCR_RS_HunterKiller

ToDo:
Scale groups
Min Units in groupp
default player count for scaling
Add spawn in random area within radius
Add random spawn area drop down list
Get waypoint formation and other types note: this should be put in the waypoint
Add waypoint spawn types note: this should be put in the waypoint
Add random group

*/

class PR_SpawnPatrol
{
	void PR_SpawnPatrol()
	{}

	//------------------------------------------------------------------------------------------------
	//! destructor
	void ~PR_SpawnPatrol()
	{}

	SCR_AIGroup m_Group;
	protected bool m_bHoldFire = false;
	protected float m_fPerceptionFactor;// = 1;

	protected int m_iSpawnSide;
	protected int m_iGroupType;
	protected vector m_vOldVector;
	protected vector m_vSpawnPosition;
	protected array<string> m_aTeleportPosition;
	protected array<string> m_aWaypointCollection;
	protected bool m_bDebugLogs;
	protected bool m_bCycleWaypoints;
	protected bool m_bRandomGroupSize;
	protected bool m_bKeepGroupActive;
	protected bool m_bSuspendIfNoPlayers;
	protected bool m_bTeleportAfterSpawn;
	protected bool m_bNeutralizePersistentObjectIfGroupIsDead;
	protected int m_iRerunCounter;// = 0;
	protected int m_iRespawnTimer;// = 0;
	protected int m_iRespawnCount;// = -1;
	protected int m_iTeleportSortOrder;// = 0;
	protected int m_iCollectionSortOrder;// = 0;
	protected int m_iWaypointSortOrder;// = 0;
	protected int m_iSpawnCollections;// = 0;
	protected bool m_bUseRandomRespawnTimer;
	protected int m_iRespawnTimerMin;// = 0;
	protected int m_iRespawnTimerMax;// = 0;

	protected int m_iAISkill;// = 50;
	protected int m_iAICombatType;// = 1;
	protected int m_iAIGroupFormation;// = 0;
	protected int m_iAIMovementType;// = 1;
	
	protected int m_iMinUnitsInGroup;// = 1;
	protected int m_iMaxUnitsInGroup;// = -1;
	
	protected IEntity m_PersistentObject;

	//------------------------------------------------------------------------------------------------
	/*!
	SPAWN GROUP AND HAND OUT WAYPOINTS

	PRSpawnPatrol(
		int spawnSide,
		int groupType,
		vector spawnPosition,
		array<bool> boolArray,
		array<array<string>> m_sStringArray,
		array<int> intArray,
		float perceptionFactor,
		IEntity persistentObject
	)
	*/
	void PRSpawnPatrol(
		int spawnSide,
		int groupType,
		vector spawnPosition,
		array<bool> boolArray,
		array<array<string>> m_sStringArray,
		array<int> intArray,
		float perceptionFactor,
		IEntity persistentObject
	)
	{
		bool debugLogs = boolArray.Get(1);
		bool useRandomRespawnTimer = boolArray.Get(2);
		bool holdFire = boolArray.Get(3);
		bool randomGroupSize = boolArray.Get(4);
		bool keepGroupActive = boolArray.Get(5);
		bool suspendIfNoPlayers = boolArray.Get(6);
		bool teleportAfterSpawn = boolArray.Get(7);
		bool neutralizePersistentObjectIfGroupIsDead = boolArray.Get(8);

		array<string> teleportPosition = m_sStringArray.Get(0);
		array<string> waypointCollection = m_sStringArray.Get(1);

		int rerunCounter = intArray.Get(0);
		int respawnTimerMin = intArray.Get(1);
		int respawnTimerMax = intArray.Get(2);
		int respawnCount = intArray.Get(3);
		int teleportSortOrder = intArray.Get(4);
		int collectionSortOrder = intArray.Get(5);
		int waypointSortOrder = intArray.Get(6);
		int spawnCollections = intArray.Get(7);
		int skill = intArray.Get(8);
		int combatType = intArray.Get(9);
		int groupFormation = intArray.Get(10);
		int minUnitsInGroup = intArray.Get(11);
		int maxUnitsInGroup = intArray.Get(12);

		// maybe make a custom group count
		string m_SpawnGroup = GetGroupToSpawn(spawnSide, groupType);
		if (debugLogs)
			Print(string.Format("[PRSpawnPatrol] m_SpawnGroup: %1", m_SpawnGroup), LogLevel.WARNING);

		SetPersistentObject(persistentObject);
		
		//--- Generate the resource
		Resource resource = GenerateAndValidateResource(m_SpawnGroup);

		if (!resource)
		{
			Print(("[PR_SpawnPatrol] (PRSpawnPatrol) Unable to load resource for the spawn group: " + m_SpawnGroup), LogLevel.ERROR);
			return;
		}

		if (debugLogs)
			Print(string.Format("[PRSpawnPatrol] resource: %1", resource), LogLevel.WARNING);

		//--- Generate spawn parameters and spawn the group

		m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPosition)));

		if (!m_Group)
		{
			Print("[PR_SpawnPatrol] (PRSpawnPatrol) Unable to spawn group!", LogLevel.ERROR);
			return;
		}

		int removeUnitCount = 0;

		if (randomGroupSize)
		{
			int prefabUnitCount = m_Group.m_aUnitPrefabSlots.Count();
			int unitsToSpawn = prefabUnitCount;
			if (unitsToSpawn > minUnitsInGroup)
				unitsToSpawn = minUnitsInGroup;
	
			if (maxUnitsInGroup > minUnitsInGroup)
				unitsToSpawn = Math.RandomIntInclusive(minUnitsInGroup, maxUnitsInGroup);
	
			if (unitsToSpawn < 1)
				unitsToSpawn = 1;
			
			Print(string.Format("[PRSpawnPatrol] unitsToSpawn: %1", unitsToSpawn), LogLevel.NORMAL);
	
			if (prefabUnitCount >= unitsToSpawn)
			{
				removeUnitCount = prefabUnitCount - unitsToSpawn;
			} else
			{
				unitsToSpawn = unitsToSpawn - prefabUnitCount;
				while (unitsToSpawn > 0)
				{
					m_Group.SetMaxUnitsToSpawn(Math.Min(prefabUnitCount, unitsToSpawn));
					m_Group.SpawnUnits();
					unitsToSpawn = unitsToSpawn - prefabUnitCount;
				}
			}
		}

		//--- For group information, needs a sleep it seems
		GetGame().GetCallqueue().CallLater(GetGroupAgents, 5000, false, m_Group, removeUnitCount);

		//--- Create waypoints for the group
		array<AIWaypoint> cycleWaypointArray = {};
		array<string> waypointCollectionsArray = {};
		array<string> collectionsArrayRandom = {};

		bool waypointReverseSort = 0;
		bool exitLoop = false;

		if (waypointCollection.Count() > 0 && waypointCollection.Count() >= 2)
		{
			int collectionReverseSort = false;

			if (collectionSortOrder == 1) // decending
			{
				collectionReverseSort = true;
			} else if (collectionSortOrder == 2) // random order
			{
				collectionReverseSort = Math.RandomInt(0, 2);
			}
			waypointCollection.Sort(collectionReverseSort);

			if (collectionSortOrder == 3) // random mix
			{
				array<string> tempCollectionsArray = {};
				foreach (int index, string x : waypointCollection)
				{
					string name = waypointCollection.Get(index);
					tempCollectionsArray.Insert(name);
				}

				while (waypointCollection.Count() > 0)
				{
					waypointCollection.Remove(0);
				}

				while (tempCollectionsArray.Count() >= 1)
				{
					int randomIndex = tempCollectionsArray.GetRandomIndex();
					string name = tempCollectionsArray.Get(randomIndex);
					waypointCollection.Insert(name);
					tempCollectionsArray.Remove(randomIndex);
				}
			}
		}
		Print(("[PR_SpawnPatrol] (PRSpawnPatrol) waypointCollectionSort: " + waypointCollection + "  collectionSortOrder: " + collectionSortOrder + "  waypointSortOrder: " + waypointSortOrder), LogLevel.NORMAL);

		if (waypointCollection.Count() > 0)
		{
			if (spawnCollections == 0)
			{
				waypointCollectionsArray = waypointCollection;
			}
			else if (spawnCollections == 1)
			{
				//--- RANDOM ONE
				int randomIndex = waypointCollection.GetRandomIndex();
				string name = waypointCollection.Get(randomIndex);
				waypointCollectionsArray.Insert(name);
			}
			else if (spawnCollections == 2)
			{
				//RAMDOM 25%
				array<string> tempArray = waypointCollection;
				int count = waypointCollection.Count();
				float _i = count * 0.75;
				Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
				Print(("PR count: " + count), LogLevel.NORMAL);
				float _c = _i;

				while (count > _i && !exitLoop)
				{
					int randomIndex = tempArray.GetRandomIndex();
					string name = tempArray.Get(randomIndex);
					waypointCollectionsArray.Insert(name);
					tempArray.Remove(randomIndex);
					_i = _i + _c;
					if (_i + _c >= count)
					{
						exitLoop = true;
					}
					Print(("PR float 2 _i: " + _i), LogLevel.NORMAL);
				}
				if (waypointCollectionsArray.Count() == 0)
					waypointCollectionsArray = waypointCollection;
			}
			else if (spawnCollections == 3)
			{
				//RAMDOM 50%
				array<string> tempArray = waypointCollection;
				int count = waypointCollection.Count();
				float _i = count * 0.50;
				Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
				Print(("PR count: " + count), LogLevel.NORMAL);
				float _c = _i;
				while (count > _i && !exitLoop)
				{
					int randomIndex = tempArray.GetRandomIndex();
					string name = tempArray.Get(randomIndex);
					waypointCollectionsArray.Insert(name);
					tempArray.Remove(randomIndex);
					_i = _i + _c;
					if (_i + _c >= count)
					{
						exitLoop = true;
					}
					Print(("PR float 2 _i: " + _i), LogLevel.NORMAL);
				}
				if (waypointCollectionsArray.Count() == 0)
					waypointCollectionsArray = waypointCollection;
			}
			else if (spawnCollections == 4)
			{
				//RAMDOM 75%
				array<string> tempArray = waypointCollection;
				int count = waypointCollection.Count();
				float _i = count * 0.25;
				Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
				Print(("PR count: " + count), LogLevel.NORMAL);
				float _c = _i;
				while (count > _i && !exitLoop)
				{
					int randomIndex = tempArray.GetRandomIndex();
					string name = tempArray.Get(randomIndex);
					waypointCollectionsArray.Insert(name);
					tempArray.Remove(randomIndex);
					_i = _i + _c;
					if (_i + _c >= count)
					{
						exitLoop = true;
					}
					Print(("PR float 2 _i: " + _i), LogLevel.NORMAL);
				}
				if (waypointCollectionsArray.Count() == 0)
					waypointCollectionsArray = waypointCollection;
			}
		}

		//--- Sorting waypoint names within collections
		array<string> namesArray = {};

		if (waypointCollectionsArray)
		{
			if (waypointCollectionsArray.Count() > 0)
			{
				int collectionCount = waypointCollectionsArray.Count();

				int _i = 0;
				while (collectionCount > _i)
				{
					array<IEntity> waypointArray = {};
					IEntity wpHolder = GetGame().GetWorld().FindEntityByName(waypointCollectionsArray.Get(_i));

					GetAllChildren(wpHolder, waypointArray, debugLogs);

					//--- Sort waypoint names, either ascending or descending
					array<string> tempNamesArray = {};
					foreach (int index, IEntity x : waypointArray)
					{
						string name = waypointArray.Get(index).GetName();
						tempNamesArray.Insert(name);
					}

					if (waypointSortOrder == 1)
						waypointReverseSort = 1;

					if (waypointSortOrder == 2 && tempNamesArray.Count() >= 1)
						waypointReverseSort = Math.RandomInt(0, 1);

					tempNamesArray.Sort(waypointReverseSort);

					foreach (int index, string x : tempNamesArray)
					{
						namesArray.Insert(x);
					}

					//--- Randomize, mix waypoint names
					if (waypointSortOrder == 3 && namesArray.Count() >= 1)
					{
						array<string> namesArrayRandom = {};
						while (namesArray.Count() >= 1)
						{
							int randomIndex = namesArray.GetRandomIndex();
							string name = namesArray.Get(randomIndex);
							namesArrayRandom.Insert(name);
							namesArray.Remove(randomIndex);

							if (debugLogs)
								Print(("[PR_SpawnPatrol] (PRSpawnPatrol) randomIndex Index: " + randomIndex + " Name: " + name), LogLevel.NORMAL);
						}
						namesArray = namesArrayRandom;
						if (debugLogs)
							Print(("[PR_SpawnPatrol] (PRSpawnPatrol) Randomized namesArray: " + namesArray), LogLevel.NORMAL);
					}

					_i++;
				}
			}
		}
		//--- Sorting waypoint names complete

		//--- Add Waypoints to the group
		foreach (int index, string x : namesArray)
		{
			string name = namesArray.Get(index);
			AIWaypoint waypoint = AIWaypoint.Cast(GetGame().GetWorld().FindEntityByName(name));

			m_Group.AddWaypointToGroup(waypoint);
			cycleWaypointArray.Insert(waypoint);
			if (debugLogs)
				Print(("[PR_SpawnPatrol] (PRSpawnPatrol) cycleWaypointArray Index: " + index + " Name: " + name), LogLevel.NORMAL);
		}

		//--- Set waypoints to cycle if enabled
		bool cycleWaypoints = boolArray.Get(0);

		if (cycleWaypointArray.IsEmpty())
			Print("[PR_SpawnPatrol] (PRSpawnPatrol) Cycle waypoints were empty", LogLevel.NORMAL);

		if (!cycleWaypointArray.IsEmpty() && cycleWaypoints)
		{
			cycleWaypointArray.Insert(cycleWaypointArray[0]);
			AddCycleWaypoint(m_Group, cycleWaypointArray.Get(0).GetOrigin(), cycleWaypointArray, rerunCounter, debugLogs);
			if (debugLogs)
				Print(("[PR_SpawnPatrol] (PRSpawnPatrol) CycleWaypoints: " + cycleWaypointArray), LogLevel.NORMAL);
		}

		SetKeepGroupActive(keepGroupActive);
		SetSuspendIfNoPlayers(suspendIfNoPlayers);
		// Set Behaviors
		SetAISkillB(skill);
		SetCombatTypeB(combatType);
		SetHoldFireB(holdFire);
		SetPerceptionFactorB(perceptionFactor);
		SetGroupFormationB(groupFormation);
		SetTeleportAfterSpawn(teleportAfterSpawn);
		SetTeleportPosition(teleportPosition);
		SetTeleportSortOrder(teleportSortOrder);
		SetDebugLogs(debugLogs);
		SetNeutralizePersistentObjectIfGroupIsDead(neutralizePersistentObjectIfGroupIsDead);

		//--- Stuff to do after group death
		if (respawnCount != 0)
		{
			SetSpawnSide(spawnSide);
			SetGroupType(groupType);
			SetSpawnPosition(spawnPosition);
			SetRandomGroupSize(randomGroupSize);
			SetMinUnitsInGroup(minUnitsInGroup);
			SetMaxUnitsInGroup(maxUnitsInGroup);
			SetCycleWaypoints(cycleWaypoints);
			SetRandomRespawnTimer(useRandomRespawnTimer);
			SetWaypointCollection(waypointCollection);
			SetRerunCounter(rerunCounter);
			SetRespawnTimerMin(respawnTimerMin);
			SetRespawnTimerMax(respawnTimerMax);
			SetRespawnCount(respawnCount);
			SetCollectionSortOrder(collectionSortOrder);
			SetWaypointSortOrder(waypointSortOrder);
			SetSpawnCollections(spawnCollections);
			m_Group.GetOnEmpty().Insert(AfterGroupIsEmpty);
		}
		else
		{
			Print(("[PR_SpawnPatrol] neutralizePersistentObjectIfGroupIsDead: " + neutralizePersistentObjectIfGroupIsDead), LogLevel.WARNING);
			if (neutralizePersistentObjectIfGroupIsDead)
				m_Group.GetOnEmpty().Insert(AfterGroupIsEmpty);
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Teleports units to position after spawning, useful for placing over water, in buildings, etc...
	void TeleportObject(string whatToMove, string whereToMove)
	{
		IEntity objectToTeleport = GetGame().GetWorld().FindEntityByName(whatToMove);
		IEntity objectToTeleportTo = GetGame().GetWorld().FindEntityByName(whereToMove);
		vector position = objectToTeleportTo.GetOrigin();
		Print(("[PR_SpawnPatrol] (TeleportObject) objectToTeleport: " + objectToTeleport + " objectToTeleportTo: " + objectToTeleportTo + " position: " + position), LogLevel.NORMAL);
		objectToTeleport.SetOrigin(position)
	}

	//------------------------------------------------------------------------------------------------
	//! Gets groupGA agents
	void GetGroupAgents(SCR_AIGroup groupGA, int removeUnitCount)
	{
		if (!groupGA)
			return;

		bool keepGroupActive = GetKeepGroupActive();
		
		// teleport after spawn
		bool teleportAfterSpawn = GetTeleportAfterSpawn();
		array<string> teleportPosition = GetTeleportPosition();

		int teleportSortOrder = GetTeleportSortOrder();
		string whereToMove = "";

		array<AIAgent> agents = {};
		groupGA.GetAgents(agents);
		Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) removeUnitCount: %1", removeUnitCount), LogLevel.NORMAL);
		if (removeUnitCount > 0 && agents.Count() > removeUnitCount)
		{
			int i = 0;
			IEntity agentEntityToRemove;

			while (i < removeUnitCount && agents.Count() > 1)
			{
				int randomIndex = agents.GetRandomIndex();
				agentEntityToRemove = agents.Get(randomIndex).GetControlledEntity();
				if (agentEntityToRemove)
				{
					SCR_EntityHelper.DeleteEntityAndChildren(agentEntityToRemove);
					agents.Remove(randomIndex);
				}
				i++;
			}
			
			agents.Clear();
			groupGA.GetAgents(agents);
			Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) agents.Count(): %1", agents.Count()), LogLevel.NORMAL);
		}

		AIWaypoint currentWaypoint = groupGA.GetCurrentWaypoint(); // works
		Print(("[PR_SpawnPatrol] (GetGroupAgents) currentWaypoint: " + currentWaypoint), LogLevel.NORMAL);
		int groupSize = groupGA.GetAgentsCount();
		Print(("[PR_SpawnPatrol] (GetGroupAgents) groupSize: " + groupSize), LogLevel.NORMAL);

		// Behaviors
		int skill = GetAISkillB();
		int combatType = GetCombatTypeB();
		bool holdFire = GetHoldFireB();
		int groupFormation = GetGroupFormationB();
		float perceptionFactor = GetPerceptionFactorB();
		string m_sPath = "$EnfusionPersistenceFramework:Scripts/Game/EPF_PersistenceComponent.c";
		bool m_bEPF_ModExist = false;

		if (teleportAfterSpawn)
		{
			whereToMove = teleportPosition.Get(0);
			Print(("[PR_SpawnPatrol] (GetGroupAgents) whereToMove: " + whereToMove), LogLevel.NORMAL);
			//teleportReverseSort
			if (teleportPosition.Count() > 0 && teleportPosition.Count() >= 2)
			{
				int teleportReverseSort = false;

				if (teleportSortOrder == 1) // decending
				{
					teleportReverseSort = true;
				} else if (teleportSortOrder == 2) // random order
				{
					teleportReverseSort = Math.RandomInt(0, 2);
				}
				teleportPosition.Sort(teleportReverseSort);

				// Match teleport array count to agents count to allow for enough positions
				if (teleportPosition.Count() < groupGA.GetAgentsCount())
				{
					int _i = 0;
					while (teleportPosition.Count() < groupGA.GetAgentsCount())
					{
						string name = teleportPosition.Get(_i);
						teleportPosition.Insert(name);
						_i++;
					}
				}

				if (teleportSortOrder == 3) // random mix
				{
					array<string> tempTeleportArray = {};
					foreach (int index, string x : teleportPosition)
					{
						string name = teleportPosition.Get(index);
						tempTeleportArray.Insert(name);
					}

					while (teleportPosition.Count() > 0)
					{
						teleportPosition.Remove(0);
					}

					while (tempTeleportArray.Count() >= 1)
					{
						int randomIndex = tempTeleportArray.GetRandomIndex();
						string name = tempTeleportArray.Get(randomIndex);
						teleportPosition.Insert(name);
						tempTeleportArray.Remove(randomIndex);
					}
				}

				if (teleportSortOrder == 4)
				{
					int randomIndex = teleportPosition.GetRandomIndex();
					whereToMove = teleportPosition.Get(randomIndex);
				}
			}
		}

		foreach (int index, AIAgent agent : agents)
		{
			IEntity agentEntity = agent.GetControlledEntity();

			if (!agentEntity)
				continue;

			string agentName = agentEntity.GetName();
			if (!agentName)
			{
				if (teleportAfterSpawn)
				{
					if ((teleportPosition.Count() > 1) && (!teleportSortOrder == 4))
					{
						whereToMove = teleportPosition.Get(index);
					}
					int iRanNum = Math.RandomInt(1, 10000);
					agentName = "PR_Agent_" + iRanNum;
					agentEntity.SetName(agentName);
					GetGame().GetCallqueue().CallLater(TeleportObject, 2000, false, agentName, whereToMove);
					Print(("[PR_SpawnPatrolTrigger] (OnActivate) agentName: " + agentName), LogLevel.NORMAL);
				}
			}

			SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(agentEntity.FindComponent(SCR_AICombatComponent));
			EAISkill aiSkill;
			EAICombatType aiCombatType;
			if (combatComponent)
			{
				combatComponent.SetAISkill(skill);
				combatComponent.SetCombatType(combatType);
				combatComponent.SetHoldFire(holdFire);
				combatComponent.SetPerceptionFactor(perceptionFactor);
				
				aiSkill = combatComponent.GetAISkill();
				aiCombatType = combatComponent.GetCombatType();
				Print(string.Format("[PR_SpawnPatrol]  (Behaviors AI) aiSkill: %1", aiSkill), LogLevel.WARNING);
				Print(string.Format("[PR_SpawnPatrol]  (Behaviors AI) aiCombatType: %1", aiCombatType), LogLevel.WARNING);
			}

			SCR_AIInfoComponent infoComponent;
			infoComponent = SCR_AIInfoComponent.Cast(agentEntity.FindComponent(SCR_AIInfoComponent));
		}

		// Set formation
		AIFormationComponent formComp = AIFormationComponent.Cast(groupGA.FindComponent(AIFormationComponent));
		if (!formComp)
		{
			Print(string.Format("ScenarioFramework Action: AI Formation Component not found for Action %1.", this), LogLevel.ERROR);
			return;
		}
		formComp.SetFormation(SCR_Enum.GetEnumName(SCR_EAIGroupFormation, groupFormation));

		array<int> behaviorArray = {skill, combatType, groupFormation};

		if (keepGroupActive)
			GetGame().GetCallqueue().CallLater(KeepGroupActive, 750, true, groupGA, agents);
	}

	int m_vKGACounter = 0;
	//------------------------------------------------------------------------------------------------
	//! returns m_vOldVector;
	int GetKGACounter()
	{
		return m_vKGACounter;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_vOldVector
	void SetKGACounter(int counter)
	{
		m_vKGACounter = counter;
	}

	//------------------------------------------------------------------------------------------------
	//! Keeps group active, even when no near player in area
	void KeepGroupActive(SCR_AIGroup kgaGroup, array<AIAgent> agents)
	{
		bool suspendIfNoPlayers = GetSuspendIfNoPlayers();
		int playerCount = GetGame().GetPlayerManager().GetPlayerCount();
		if (playerCount == 0 && suspendIfNoPlayers)
		{
			if (GetKGACounter() >= 60)
			{
				Print(("[PR_SpawnPatrol] (KeepActive) playerCount: " + playerCount), LogLevel.NORMAL);
				m_vKGACounter = 0;
				SetKGACounter(m_vKGACounter);
			}
			SetKGACounter(m_vKGACounter++);
			return;
		}

		if (!kgaGroup || !agents || agents.Count() == 0)
		{
			GetGame().GetCallqueue().Remove(KeepGroupActive);
			return;
		}

		AIAgent leader = kgaGroup.GetLeaderAgent();
		if (!leader)
			return;
		//Print(("[PR_SpawnPatrol] (KeepActive) leader.GetLOD(): " + leader.GetLOD()), LogLevel.NORMAL);
		if (leader.GetLOD() == 10)
		{
			foreach (int index, AIAgent agent : agents)
			{
				if (agent)
					agent.SetLOD(9);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Kills entity
	protected void KillUnit(IEntity entity)
	{
		SCR_DamageManagerComponent damageMananager = SCR_DamageManagerComponent.Cast(entity.FindComponent(SCR_DamageManagerComponent));
		if (damageMananager)
			damageMananager.Kill(Instigator.CreateInstigator(null));
	}
	
	//------------------------------------------------------------------------------------------------
	//! What to do after all groupA members are dead
	void AfterGroupIsEmpty(SCR_AIGroup groupA)
	{
		if (!Replication.IsRunning())
			return;

		bool neutralizePersistentObjectIfGroupIsDead = GetNeutralizePersistentObjectIfGroupIsDead();
		IEntity persistentObject = GetPersistentObject();
		int respawnCount = GetRespawnCount();
		
		if (neutralizePersistentObjectIfGroupIsDead)
		{
			if (persistentObject)
			{
				Print(string.Format("[PR_SpawnPatrol] AfterGroupIsEmpty : persistentObject: %1", persistentObject.GetName()), LogLevel.WARNING);
				KillUnit(persistentObject);
				SCR_EntityHelper.DeleteEntityAndChildren(persistentObject);
				if (respawnCount == 0)
					return;
			}
		}
		
		if (respawnCount != 0)
			respawnCount--;
		SetRespawnCount(respawnCount);
		
		int spawnSide = GetSpawnSide();
		int groupType = GetGroupType();
		vector spawnPosition = GetSpawnPosition();
		bool cycleWaypoints = GetCycleWaypoints();
		bool debugLogs = GetDebugLogs();
		bool useRandomRespawnTimer = GetRandomRespawnTimer();
		bool holdFire = GetHoldFireB();
		bool randomGroupSize = GetRandomGroupSize();
		int minUnitsInGroup = GetMinUnitsInGroup();
		int maxUnitsInGroup = GetMaxUnitsInGroup();
		bool keepGroupActive = GetKeepGroupActive();
		bool suspendIfNoPlayers = GetSuspendIfNoPlayers();
		bool teleportAfterSpawn = GetTeleportAfterSpawn();
		array<bool> boolArray = {cycleWaypoints, debugLogs, useRandomRespawnTimer, holdFire, randomGroupSize, keepGroupActive, suspendIfNoPlayers, teleportAfterSpawn, neutralizePersistentObjectIfGroupIsDead};
		array<string> teleportPosition = GetTeleportPosition();
		array<string> waypointCollection = GetWaypointCollection();
		array<array<string>> stringArray = {teleportPosition, waypointCollection};
		int rerunCounter = GetRerunCounter();
		int respawnTimerMin = GetRespawnTimerMin();
		int respawnTimerMax = GetRespawnTimerMax();
		int teleportSortOrder = GetTeleportSortOrder();
		int collectionSortOrder = GetCollectionSortOrder();
		int waypointSortOrder = GetWaypointSortOrder();

		int skill = GetAISkillB();
		int combatType = GetCombatTypeB();
		int groupFormation = GetGroupFormationB();
		float perceptionFactor = GetPerceptionFactorB();

		int spawnCollections = GetSpawnCollections();
		array<int> intArray = {
			rerunCounter, // 0
			respawnTimerMin, // 1
			respawnTimerMax, // 2
			respawnCount, // 3
			teleportSortOrder, // 4
			collectionSortOrder, // 5
			waypointSortOrder, // 6
			spawnCollections, // 7
			skill, // 8
			combatType, // 9
			groupFormation, // 10
			minUnitsInGroup, // 11
			maxUnitsInGroup // 12
		};

		int delay = respawnTimerMin * 1000;
		if (useRandomRespawnTimer)
		{
			delay = Math.RandomInt(respawnTimerMin * 1000, respawnTimerMax * 1000);
		}

		if (debugLogs)
		{
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) rerunCounter: " + rerunCounter, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnTimerMin: " + respawnTimerMin, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnTimerMax: " + respawnTimerMax, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnCount: " + respawnCount, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) spawnSide: " + spawnSide, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) groupType: " + groupType, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) spawnPosition: " + spawnPosition, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) boolArray: " + boolArray, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) teleportPosition: " + teleportPosition, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) waypointCollection: " + waypointCollection, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) intArray: " + intArray, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) delay: " + delay, LogLevel.NORMAL);
		}

		//--- Execute the AI spawning using a delayed call
		GetGame().GetCallqueue().CallLater(
			PRSpawnPatrol,
			delay,
			false,
			spawnSide,
			groupType,
			spawnPosition,
			boolArray,
			stringArray,
			intArray,
			perceptionFactor,
			persistentObject
		);
	}

	//! SETTERS/GETTERS

	//------------------------------------------------------------------------------------------------
	//! sets m_vOldVector
	void SetOldVector(vector distance)
	{
		m_vOldVector = distance;
	}
	
	//------------------------------------------------------------------------------------------------
	//! returns m_vOldVector;
	vector GetOldVector()
	{
		return m_vOldVector;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iAISkill
	void SetAISkillB(int skill)
	{
		m_iAISkill = skill;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iAISkill;
	int GetAISkillB()
	{
		return m_iAISkill;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iAICombatType
	void SetCombatTypeB(int combatType)
	{
		m_iAICombatType = combatType;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iAICombatType;
	int GetCombatTypeB()
	{
		return m_iAICombatType;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bHoldFire
	void SetHoldFireB(bool holdFire)
	{
		m_bHoldFire = holdFire;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bHoldFire;
	bool GetHoldFireB()
	{
		return m_bHoldFire;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iAIGroupFormation
	void SetGroupFormationB(int groupFormation)
	{
		m_iAIGroupFormation = groupFormation;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iAIGroupFormation;
	int GetGroupFormationB()
	{
		return m_iAIGroupFormation;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_fPerceptionFactor
	void SetPerceptionFactorB(float perceptionFactor)
	{
		m_fPerceptionFactor = perceptionFactor;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_fPerceptionFactor;
	float GetPerceptionFactorB()
	{
		return m_fPerceptionFactor;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_PersistentObject
	void SetPersistentObject(IEntity persistentObject)
	{
		m_PersistentObject = persistentObject;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_PersistentObject;
	IEntity GetPersistentObject()
	{
		return m_PersistentObject;
	}	
		
	//------------------------------------------------------------------------------------------------
	//! sets m_iSpawnSide
	void SetSpawnSide(int spawnSide)
	{
		m_iSpawnSide = spawnSide;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iSpawnSide;
	int GetSpawnSide()
	{
		return m_iSpawnSide;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iGroupType
	void SetGroupType(int groupType)
	{
		m_iGroupType = groupType;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iGroupType
	int GetGroupType()
	{
		return m_iGroupType;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_vSpawnPosition
	void SetSpawnPosition(vector spawnPosition)
	{
		m_vSpawnPosition = spawnPosition;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_vSpawnPosition
	vector GetSpawnPosition()
	{
		return m_vSpawnPosition;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bCycleWaypoints
	void SetCycleWaypoints(bool cycleWaypoints)
	{
		m_bCycleWaypoints = cycleWaypoints;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bCycleWaypoints
	bool GetCycleWaypoints()
	{
		return m_bCycleWaypoints;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bDebugLogs
	void SetDebugLogs(bool debugLogs)
	{
		m_bDebugLogs = debugLogs;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bDebugLogs
	bool GetDebugLogs()
	{
		return m_bDebugLogs;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bNeutralizePersistentObjectIfGroupIsDead
	void SetNeutralizePersistentObjectIfGroupIsDead(bool neutralizePersistentObjectIfGroupIsDead)
	{
		m_bNeutralizePersistentObjectIfGroupIsDead = neutralizePersistentObjectIfGroupIsDead;
	}
	
	//------------------------------------------------------------------------------------------------
	//! returns m_bNeutralizePersistentObjectIfGroupIsDead
	bool GetNeutralizePersistentObjectIfGroupIsDead()
	{
		return m_bNeutralizePersistentObjectIfGroupIsDead;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bRandomGroupSize
	void SetRandomGroupSize(bool randomGroupSize)
	{
		m_bRandomGroupSize = randomGroupSize;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bRandomGroupSize
	bool GetRandomGroupSize()
	{
		return m_bRandomGroupSize;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iMinUnitsInGroup
	void SetMinUnitsInGroup(int minUnitsInGroup)
	{
		m_iMinUnitsInGroup = minUnitsInGroup;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iMinUnitsInGroup
	int GetMinUnitsInGroup()
	{
		return m_iMinUnitsInGroup;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_iMaxUnitsInGroup
	void SetMaxUnitsInGroup(int maxUnitsInGroup)
	{
		m_iMaxUnitsInGroup = maxUnitsInGroup;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iMaxUnitsInGroup
	int GetMaxUnitsInGroup()
	{
		return m_iMaxUnitsInGroup;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_bKeepGroupActive
	void SetKeepGroupActive(bool keepGroupActive)
	{
		m_bKeepGroupActive = keepGroupActive;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bKeepGroupActive
	bool GetKeepGroupActive()
	{
		return m_bKeepGroupActive;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bSuspendIfNoPlayers
	void SetSuspendIfNoPlayers(bool suspendIfNoPlayers)
	{
		m_bSuspendIfNoPlayers = suspendIfNoPlayers;
	}
	
	//------------------------------------------------------------------------------------------------
	//! returns m_bSuspendIfNoPlayers
	bool GetSuspendIfNoPlayers()
	{
		return m_bSuspendIfNoPlayers;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bTeleportAfterSpawn
	void SetTeleportAfterSpawn(bool teleportAfterSpawn)
	{
		m_bTeleportAfterSpawn = teleportAfterSpawn;
	}
	
	//------------------------------------------------------------------------------------------------
	//! returns m_bTeleportAfterSpawn
	bool GetTeleportAfterSpawn()
	{
		return m_bTeleportAfterSpawn;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bUseRandomRespawnTimer
	void SetRandomRespawnTimer(bool useRandomRespawnTimer)
	{
		m_bUseRandomRespawnTimer = useRandomRespawnTimer;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bUseRandomRespawnTimer
	bool GetRandomRespawnTimer()
	{
		return m_bUseRandomRespawnTimer;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aTeleportPosition
	void SetTeleportPosition(array<string> teleportPosition)
	{
		m_aTeleportPosition = teleportPosition;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aTeleportPosition
	array<string> GetTeleportPosition()
	{
		return m_aTeleportPosition;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aWaypointCollection
	void SetWaypointCollection(array<string> waypointCollection)
	{
		m_aWaypointCollection = waypointCollection;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aWaypointCollection
	array<string> GetWaypointCollection()
	{
		return m_aWaypointCollection;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iRerunCounter
	void SetRerunCounter(int rerunCounter)
	{
		m_iRerunCounter = rerunCounter;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iRerunCounter
	int GetRerunCounter()
	{
		return m_iRerunCounter;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iRespawnTimerMin
	void SetRespawnTimerMin(int respawnTimerMin)
	{
		m_iRespawnTimerMin = respawnTimerMin;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iRespawnTimerMin
	int GetRespawnTimerMin()
	{
		return m_iRespawnTimerMin;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iRespawnTimerMax
	void SetRespawnTimerMax(int respawnTimerMax)
	{
		m_iRespawnTimerMax = respawnTimerMax;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iRespawnTimerMax
	int GetRespawnTimerMax()
	{
		return m_iRespawnTimerMax;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iRespawnCount
	void SetRespawnCount(int respawnCount)
	{
		m_iRespawnCount = respawnCount;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iRespawnCount
	int GetRespawnCount()
	{
		return m_iRespawnCount;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iTeleportSortOrder
	void SetTeleportSortOrder(int teleportSortOrder)
	{
		m_iTeleportSortOrder = teleportSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iTeleportSortOrder
	int GetTeleportSortOrder()
	{
		return m_iTeleportSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iCollectionSortOrder
	void SetCollectionSortOrder(int collectionSortOrder)
	{
		m_iCollectionSortOrder = collectionSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iCollectionSortOrder
	int GetCollectionSortOrder()
	{
		return m_iCollectionSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iWaypointSortOrder
	void SetWaypointSortOrder(int waypointSortOrder)
	{
		m_iWaypointSortOrder = waypointSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iWaypointSortOrder
	int GetWaypointSortOrder()
	{
		return m_iWaypointSortOrder;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iSpawnCollections
	void SetSpawnCollections(int spawnCollections)
	{
		m_iSpawnCollections = spawnCollections;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iSpawnCollections
	int GetSpawnCollections()
	{
		return m_iSpawnCollections;
	}

	//------------------------------------------------------------------------------------------------
	//! GET ALL CHILDREN FOR WAYPOINT COLLECTION
	void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren, bool debugLogs)
	{
		if (!parent)
			return;

		IEntity child = parent.GetChildren();

		if (!child)
			child = parent;

		int childCount = 0;
		while (child)
		{
			childCount = childCount+ 1;
			allChildren.InsertAt(child, 0);
			child = child.GetSibling();
		}
	}

	//------------------------------------------------------------------------------------------------
	//! CREATE GROUP CYCLE WAYPOINT	credit to Kingsley & Zelik <discord enfusion_scripting> for ideas
	void AddCycleWaypoint(SCR_AIGroup groupCW, vector waypointPosition, array<AIWaypoint> cycleWaypointArray, int rerunCounter, bool debugLogs)
	{
		Resource resource = Resource.Load("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et");
		AIWaypointCycle waypoint = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(waypointPosition)));
		waypoint.SetWaypoints(cycleWaypointArray);
		waypoint.SetRerunCounter(rerunCounter);
		groupCW.AddWaypoint(waypoint);
		if (debugLogs)
			Print(("[PR_SpawnPatrol] (AddCycleWaypoint) Added cycle waypoint to groupCW: " + waypoint + ", Waypoint Queue: " + cycleWaypointArray), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! GENERATE SPAWN PARAMETERS
	protected EntitySpawnParams GenerateSpawnParameters(vector position)
	{
		EntitySpawnParams params = EntitySpawnParams(); //--- Create a new set of spawn parameters
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = position; //--- Assign the position to those parameters
		return params; //--- Return this set of spawn parameters
	}

	//------------------------------------------------------------------------------------------------
	//! GENERATE AND VALIDATE RESOURCES
	protected Resource GenerateAndValidateResource(string resourceToLoad)
	{
		Resource resource = Resource.Load(resourceToLoad); //--- Load the resouce

		if (!resource.IsValid()) //--- Validate the prefab and show error if invalid
		{
			Print(("[PR_SpawnPatrol] (GenerateAndValidateResource) Resource is invalid: " + resourceToLoad), LogLevel.ERROR);
			return null;
		}

		return resource; //--- Return the resource
	}

	//------------------------------------------------------------------------------------------------
	//! GET TYPE OF GROUP TO SPAWN
	// TO DO: Add more single units and fill empty groups
	protected string GetGroupToSpawn(int spawnSide, int groupType)
	{
		/*
		USSR
		{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et
		FIA
		{EE92725E9B949C3D}Prefabs/Groups/INDFOR/Group_FIA_PlatoonHQ.et
		{2E9C920C3ACA2C6F}Prefabs/Groups/INDFOR/Group_FIA_ReconTeam.et
		*/
		if (spawnSide == 0) // US
		{
			switch (groupType)
			{
				case 0: // Man: AR (1)
				{
					return "{C153572E4A1724B4}Prefabs/Groups/BLUFOR/Group_US_AR_M.et"; // modded
				};
				case 1: // Man: AT (1)
				{
					return "{7C8305A9350C38AA}Prefabs/Groups/BLUFOR/Group_US_AT_M.et"; // modded
				};
				case 2: // Man: GL (1)
				{
					return "{1B692CF8F695C2FA}Prefabs/Groups/BLUFOR/Group_US_GL_M.et"; // modded
				};
				case 3: // Man: MG (1)
				{
					return "{543E9CAF67ED912D}Prefabs/Groups/BLUFOR/Group_US_MG_M.et"; // modded
				};
				case 4: // Man: Medic (1)
				{
					return "{EB297CF916CF1277}Prefabs/Groups/BLUFOR/Group_US_Medic_M.et"; // modded
				};
				case 5: // Man: Officer (1)
				{
					return "{1671C06D39508596}Prefabs/Groups/BLUFOR/Group_US_Officer_M.et"; // modded
				};
				case 6: // Man: Randomized (1)
				{
					return "{5F0B6659A5594AB1}Prefabs/Groups/BLUFOR/Group_US_Randomized_M.et"; // modded
				};
				case 7: // Man: Rifleman (1)
				{
					return "{1C6439B48BCA6D77}Prefabs/Groups/BLUFOR/Group_US_Rifleman_M.et"; // modded
				};
				case 8: // Man: RTO (1)
				{
					return "{C246071DBF6AD7D9}Prefabs/Groups/BLUFOR/Group_US_RTO_M.et"; // modded
				};
				case 9: // Man: Sapper (1)
				{
					return "{ABE8F3539944B6BD}Prefabs/Groups/BLUFOR/Group_US_Sapper_M.et"; // modded
				};
				case 10: // Man: Sniper (1)
				{
					return "{DF2F568D5A2A8F13}Prefabs/Groups/BLUFOR/Group_US_Sniper_M.et"; // modded
				};
				case 11: // Man: Spotter (1)
				{
					return "{7A32D63CD61C9C60}Prefabs/Groups/BLUFOR/Group_US_Spotter_M.et"; // modded
				};
				case 12: // Man: SL (1)
				{
					return "{64D6EFA39E5190DE}Prefabs/Groups/BLUFOR/Group_US_SL_M.et"; // modded
				};
				case 13: // Man: Unarmed (1)
				{
					return "{71379F19E18ED274}Prefabs/Groups/BLUFOR/Group_US_Unarmed_M.et"; // modded
				};
				case 14: // Man SF: Grenadier (1)
				{
					return "{4ACCB999D9E24889}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Grenadier_M.et"; // modded
				};
				case 15: // Man SF: LMG (1)
				{
					return "{B9339A61F368AB3C}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_LMG_M.et"; // modded
				};
				case 16: // Man SF: Medic (1)
				{
					return "{3CF7506E3F5873B4}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Medic_M.et"; // modded
				};
				case 17: // Man SF: Officer (1)
				{
					return "{AB87CDAB4444D3E1}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Officer_M.et"; // modded
				};
				case 18: // Man SF: Rifleman (1)
				{
					return "{2F71D82DE8A1E379}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Rifleman_M.et"; // modded
				};
				case 19: // Man SF: RTO (1)
				{
					return "{BF9204E1603997ED}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_RTO_M.et"; // modded
				};
				case 20: // Man SF: Sapper (1)
				{
					return "{B124BF29FDE612DA}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Sapper_M.et"; // modded
				};
				case 21: // Man SF: Sharpshooter (1)
				{
					return "{7579F08D3CFFE330}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_Sharpshooter_M.et"; // modded
				};
				case 22: // Man SF: SL (1)
				{
					return "{E29113E030AE676B}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_SF_SL_M.et"; // modded
				};
				case 23: // Group: Ammo Team (4)
				{
					return "{F72EF3429D8C8DF5}Prefabs/Groups/BLUFOR/Group_US_AmmoTeam.et";
				};
				case 24: // Group: AT Team (4)
				{
					return "{FAEA8B9E1252F56E}Prefabs/Groups/BLUFOR/Group_US_Team_LAT.et";
				};
				case 25: // Group: Fire Team (4)
				{
					return "{84E5BBAB25EA23E5}Prefabs/Groups/BLUFOR/Group_US_FireTeam.et";
				};
				case 26: // Group: Light Fire Team (4)
				{
					return "{FCF7F5DC4F83955C}Prefabs/Groups/BLUFOR/Group_US_LightFireTeam.et";
				};
				case 27: // Group: GL Team (2)
				{
					return "{DE747BC9217D383C}Prefabs/Groups/BLUFOR/Group_US_Team_GL.et";
				};
				case 28: // Group: MG Team (2)
				{
					return "{958039B857396B7B}Prefabs/Groups/BLUFOR/Group_US_MachineGunTeam.et";
				};
				case 29: // Group: Medical Section (2)
				{
					return "{EF62027CC75A7459}Prefabs/Groups/BLUFOR/Group_US_MedicalSection.et";
				};
				case 30: // Group: Platoon HQ (4)
				{
					return "{B7AB5D3F8A7ADAE4}Prefabs/Groups/BLUFOR/Group_US_PlatoonHQ.et";
				};
				case 31: // Group: Recon Team (2)
				{
					return "{F65B7BB712F46FEE}Prefabs/Groups/BLUFOR/Group_US_ReconTeam.et";
				};
				case 32: // Group: Rifle Squad (9)
				{
					return "{DDF3799FA1387848}Prefabs/Groups/BLUFOR/Group_US_RifleSquad.et";
				};
				case 33: // Group: Sapper Team (2)
				{
					return "{9624D2B39397E148}Prefabs/Groups/BLUFOR/Group_US_SapperTeam.et";
				};
				case 34: // Group: Sentry Team (2)
				{
					return "{3BF36BDEEB33AEC9}Prefabs/Groups/BLUFOR/Group_US_SentryTeam.et";
				};
				case 35: // Group: Sniper Team (2)
				{
					return "{D807C7047E818488}Prefabs/Groups/BLUFOR/Group_US_SniperTeam.et";
				};
				case 36: // Group: Suppress Team (4)
				{
					return "{81B6DBF2B88545F5}Prefabs/Groups/BLUFOR/Group_US_Team_Suppress.et";
				};
				case 37: // Group SF: Sentry Team (2)
				{
					return "{35681BE27C302FF5}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_GreenBeret_SentryTeam.et";
				};
				case 38: // Group SF: Squad (6)
				{
					return "{D0886786634E55AE}Prefabs/Groups/BLUFOR/GreenBerets/Group_US_GreenBeret_Squad.et";
				};
				default:
				{
					return "{84E5BBAB25EA23E5}Prefabs/Groups/BLUFOR/Group_US_FireTeam.et";
				};
			};
		}
		else
		{
			if (spawnSide == 1) // USSR
			{
				switch (groupType)
				{
					case 0: // Man: AR (1)
					{
						return "{FEDDF58C99D9F1EE}Prefabs/Groups/OPFOR/Group_USSR_AR_M.et"; // modded
					};
					case 1: // Man: AT (1)
					{
						return "{430DA70BE6C2EDF0}Prefabs/Groups/OPFOR/Group_USSR_AT_M.et"; // modded
					};
					case 2: // Man: GL (1)
					{
						return "{24E78E5A255B17A0}Prefabs/Groups/OPFOR/Group_USSR_GL_M.et"; // modded
					};
					case 3: // Man: MG (1)
					{
						return "{B50B7BF31273C2A6}Prefabs/Groups/OPFOR/Group_USSR_MG_M.et"; // modded
					};
					case 4: // Man: Medic (1)
					{
						return "{291DD10453C18378}Prefabs/Groups/OPFOR/Group_USSR_Medic_M.et"; // modded
					};
					case 5: // Man: Officer (1)
					{
						return "{ACCA51BC7499B346}Prefabs/Groups/OPFOR/Group_USSR_Officer_M.et"; // modded
					};
					case 6: // Man: Randomized (1)
					{
						return "{006A43D3DD08919D}Prefabs/Groups/OPFOR/Group_USSR_Randomized_M.et"; // modded
					};
					case 7: // Man: Rifleman (1)
					{
						return "{ECCF8B64C1BDF313}Prefabs/Groups/OPFOR/Group_USSR_Rifleman_M.et"; // modded
					};
					case 8: // Man: RTO (1)
					{
						return "{FAE7A43D28646D3C}Prefabs/Groups/OPFOR/Group_USSR_RTO_M.et"; // modded
					};
					case 9: // Man: Sapper (1)
					{
						return "{365B2A14535F8179}Prefabs/Groups/OPFOR/Group_USSR_Sapper_M.et"; // modded
					};
					case 10: // Man: Scout (1)
					{
						return "{A91F457171BCAD72}Prefabs/Groups/OPFOR/Group_USSR_Scout_M.et"; // modded
					};
					case 11: // Man: Sharpshooter (1)
					{
						return "{429C8FCA9031B8D7}Prefabs/Groups/OPFOR/Group_USSR_Sniper_M.et"; // modded change name Group_USSR_Sharpshooter_M
					};
					case 12: // Man: SL (1)
					{
						return "{5B584D014D9F4584}Prefabs/Groups/OPFOR/Group_USSR_SL_M.et"; // modded
					};
					case 13: // Man: Unarmed (1)
					{
						return "{CB8C0EC8AC47E4A4}Prefabs/Groups/OPFOR/Group_USSR_Unarmed_M.et"; // modded
					};
					case 14: // Man SF: Grenadier (1)
					{
						return "{53FB4DC3D5ACE553}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_GL_M.et"; // modded
					};
					case 15: // Man SF: LMG (1)
					{
						return "{F09892CAF174B826}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_LMG_M.et"; // modded
					};
					case 16: // Man SF: Medic (1)
					{
						return "{DF03D3F9DA973834}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_Medic_M.et"; // modded
					};
					case 17: // Man SF: Officer (1)
					{
						return "{31F7833E5140D159}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_Officer_M.et"; // modded
					};
					case 18: // Man SF: Rifleman (1)
					{
						return "{D87FDF5A4E211EC3}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_Rifleman_M.et"; // modded
					};
					case 19: // Man SF: RTO (1)
					{
						return "{F6390C4A622584F7}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_RTO_M.et"; // modded
					};
					case 20: // Man SF: Sapper (1)
					{
						return "{62331CDA7E0E9753}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_Sapper_M.et"; // modded	
					};
					case 21: // Man SF: Sharpshooter (1)
					{
						return "{20903BE0913029BD}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_Sharpshooter_M.et"; // modded
					};
					case 22: // Man SF: SL (1)
					{
						return "{00BDDC3D6649324B}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_SF_SL_M.et"; // modded
					};
					case 23: // Group: Ammo Team (4)
					{
						return "{C8622D0595B48437}Prefabs/Groups/OPFOR/Group_USSR_AmmoTeam.et";
					};
					case 24: // Group: AT Team (4)
					{
						return "{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et";
					};
					case 25: // Group: Fire Team (4)
					{
						return "{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et";
					};
					case 26: // Group: Light Fire Team (4)
					{
						return "{657590C1EC9E27D3}Prefabs/Groups/OPFOR/Group_USSR_LightFireTeam.et";
					};
					case 27: // Group: GL Team (2)
					{
						return "{43C7A28EEB660FF8}Prefabs/Groups/OPFOR/Group_USSR_Team_GL.et";
					};
					case 28: // Group: Maneuver Group (2) (2)
					{
						return "{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et";
					};
					case 29: // Group: MG Team (2)
					{
						return "{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et";
					};
					case 30: // Group: Medical Section (2)
					{
						return "{D815658156080328}Prefabs/Groups/OPFOR/Group_USSR_MedicalSection.et";
					};
					case 31: // Group: Platoon HQ (5)
					{
						return "{0D10CCEEC7B3EC34}Prefabs/Groups/OPFOR/Group_USSR_PlatoonHQ.et";
					};
					case 32: // Group: Rifle Squad (6)
					{
						return "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et";
					};
					case 33: // Group: Sapper Team (2)
					{
						return "{BB2E6C2CC2755E9B}Prefabs/Groups/OPFOR/Group_USSR_SapperTeam.et";
					};
					case 34: // Group: Sentry Team (2)
					{
						return "{CB58D90EA14430AD}Prefabs/Groups/OPFOR/Group_USSR_SentryTeam.et";
					};
					case 35: // Group: Suppress Team (4)
					{
						return "{1C0502B5729E7231}Prefabs/Groups/OPFOR/Group_USSR_Team_Suppress.et";
					};
					case 36: // Group SF: Sentry Team (2)
					{
						return "{4C44B4D8F2820F25}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_Spetsnaz_SentryTeam.et";
					};
					case 37: // Group SF: Squad (6)
					{
						return "{4D3BBEC1A955626A}Prefabs/Groups/OPFOR/Spetsnaz/Group_USSR_Spetsnaz_Squad.et";
					};
					case 11: // Sniper Team
					{
						return "{28AC75D434F61AEC}Prefabs/Groups/OPFOR/Group_USSR_SniperTeam.et"; // modded
					};
					default:
					{
						return "{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et";
					};
				};
			}
			else
			{
				if (spawnSide == 2) // FIA
				{
					switch (groupType)
					{
						case 0: // "Man: AT (1)" modded
						{
							return "{F70727A140D8E7FE}Prefabs/Groups/INDFOR/Group_FIA_AT_M.et";
						};
						case 1: // "Man: Machine Gun (1)" modded
						{
							return "{DFBABEA712394E79}Prefabs/Groups/INDFOR/Group_FIA_MG_M.et";
						};
						case 2: //  "Man: Medic (1)" modded
						{
							return "{C9BFE8740605FBB9}Prefabs/Groups/INDFOR/Group_FIA_Medic_M.et";
						};
						case 3: // "Man: Randomized (1)" modded
						{
							return "{F4F92FB2FE7EC979}Prefabs/Groups/INDFOR/Group_FIA_Randomized_M.et";
						};
						case 4: // "Man: Rifleman (1)" modded
						{
							return "{49E50F2EAA6EFF9A}Prefabs/Groups/INDFOR/Group_FIA_Rifleman_M.et";
						};
						case 5: // "Man: RTO (1)" modded
						{
							return "{E5E9041EF03C751F}Prefabs/Groups/INDFOR/Group_FIA_RTO_M.et";
						};
						case 6: // "Man: Sapper (1)" modded
						{
							return "{74E74C6B21BA1E45}Prefabs/Groups/INDFOR/Group_FIA_Sapper_M.et";
						};
						case 7: // "Man: Sharpshooter (1)" modded
						{
							return "{0020E9B5E2D427EB}Prefabs/Groups/INDFOR/Group_FIA_Sharpshooter_M.et";
						};
						case 8: // "Man: Squad Leader (1)" modded
						{
							return "{EF52CDABEB854F8A}Prefabs/Groups/INDFOR/Group_FIA_SL_M.et";
						};
						case 9: // "Man: Unarmed (1)" modded
						{
							return "{280EB078F06094AD}Prefabs/Groups/INDFOR/Group_FIA_Unarmed_M.et";
						};
						case 10: // "Man SF: Grenadier (1)" modded
						{
							return "{0F481F85ADD2EDE9}Prefabs/Groups/INDFOR/Group_FIA_SF_Grenadier_M.et";
						};
						case 11: // "Man SF: Medic (1)" modded
						{
							return "{609E7953D2C74180}Prefabs/Groups/INDFOR/Group_FIA_SF_Medic_M.et";
						};
						case 12: // "Man SF: Rifleman (1)" modded
						{
							return "{076A40B681E8950E}Prefabs/Groups/INDFOR/Group_FIA_SF_Rifleman_M.et";
						};
						case 13: // "Man SF: Scout (1)" modded
						{
							return "{E09CED26F0BA6F8A}Prefabs/Groups/INDFOR/Group_FIA_SF_Scout_M.et";
						};
						case 14: // "Group: Ammo Team (4)"
						{
							return "{C1E39427E43B1E79}Prefabs/Groups/INDFOR/Group_FIA_AmmoTeam.et";
						};
						case 15: // "Group: AT Team (4)"
						{
							return "{2CC26054775FBA2C}Prefabs/Groups/INDFOR/Group_FIA_Team_AT.et";
						};
						case 16: // "Group: Fire Team (5)"
						{
							return "{5BEA04939D148B1D}Prefabs/Groups/INDFOR/Group_FIA_FireTeam.et";
						};
						case 17: // "Group: Light Fire Team (4)"
						{
							return "{1BB20A4B3A53D0F5}Prefabs/Groups/INDFOR/Group_FIA_LightFireTeam.et";
						};
						case 18: // "Group: Machine Gun Team (2)"
						{
							return "{22F33D3EC8F281AB}Prefabs/Groups/INDFOR/Group_FIA_MachineGunTeam.et";
						};
						case 19: // "Group: Medical Section (2)"
						{
							return "{581106FA58919E89}Prefabs/Groups/INDFOR/Group_FIA_MedicalSection.et";
						};
						case 20: // "Group: Platoon HQ (3)"
						{
							return "{EE92725E9B949C3D}Prefabs/Groups/INDFOR/Group_FIA_PlatoonHQ.et";
						};
						case 21: // "Group: Recon Team (2)"
						{
							return "{2E9C920C3ACA2C6F}Prefabs/Groups/INDFOR/Group_FIA_ReconTeam.et";
						};
						case 22: // "Group: Rifle Squad (7)"
						{
							return "{CE41AF625D05D0F0}Prefabs/Groups/INDFOR/Group_FIA_RifleSquad.et";
						};
						case 23: // "Group: Sapper Team (2)"
						{
							return "{A0E9B5D6EA2072C4}Prefabs/Groups/INDFOR/Group_FIA_SapperTeam.et";
						};
						case 24: // "Group: Sentry Team (2)"
						{
							return "{6E725D44CA973C24}Prefabs/Groups/INDFOR/Group_FIA_SentryTeam.et";
						};
						case 25: // "Group: Sharpshooter Team (2)"
						{
							return "{6307F42403E9B8A4}Prefabs/Groups/INDFOR/Group_FIA_SharpshooterTeam.et";
						};
						case 26: // "Group: Suppress Team (4)"
						{
							return "{89CE31B88A5FA745}Prefabs/Groups/INDFOR/Group_FIA_Team_Suppress.et";
						};
						default:
						{
							return "{5BEA04939D148B1D}Prefabs/Groups/INDFOR/Group_FIA_FireTeam.et";
						};
					};
				}
				else
				{
					if (spawnSide == 3) // CIV
					{
						switch (groupType)
						{
							case 0: // "Businessman: Randomized (1)" modded
							{
								return "{55F9B4E227B13B6B}Prefabs/Groups/CIV/Group_CIV_Businessman_M.et"; // modded
							};
							case 1: // "ConstructionWorker: Randomized (1)" modded
							{
								return "{6FD0B67BDC42F1FD}Prefabs/Groups/CIV/Group_CIV_ConstructionWorker_M.et"; // modded
							};
							case 2: // "Dockworker: Randomized (1)" modded
							{
								return "{435A9C896D732A96}Prefabs/Groups/CIV/Group_CIV_Dockworker_M.et"; // modded
							};
							case 3: // "GenericCivilian: Randomized (1)" modded
							{
								return "{786CFDDDEDC05481}Prefabs/Groups/CIV/Group_CIV_GenericCivilian_M.et"; // modded
							};
							case 4: // Businessmen (3)
							{
								return "{FB2BD9B5C65E0487}Prefabs/Groups/CIV/Group_CIV_Businessmen.et"; // modded
							};
							case 5: // ConstructionWorkers (5)
							{
								return "{8A251FDFB29D763D}Prefabs/Groups/CIV/Group_CIV_ConstructionWorkers.et"; // modded
							};
							case 6: // Dockworkers (6)
							{
								return "{3A23D35EEB7B8171}Prefabs/Groups/CIV/Group_CIV_Dockworkers.et"; // modded
							};
							case 7: // Generic Civilians Cotton Shirt (6)
							{
								return "{083743756819C4EC}Prefabs/Groups/CIV/Group_CIV_GenericCivilians_CottonShirt.et"; // modded
							};
							case 8: // Generic Civilians Denim Jacket (2)
							{
								return "{0B09A60D42C795BF}Prefabs/Groups/CIV/Group_CIV_GenericCivilians_DenimJacket.et"; // modded
							};
							case 9: // Generic Civilians Turtleneck (3)
							{
								return "{A0AB3BE793EC3CBB}Prefabs/Groups/CIV/Group_CIV_GenericCivilians_Turtleneck.et"; // modded
							};
							default:
							{
								return "{8A251FDFB29D763D}Prefabs/Groups/CIV/Group_CIV_ConstructionWorkers.et";
							};
						}
					}
					return "<ERROR>";
				}
				return "<ERROR>";
			}
		}
		return "<ERROR>";
	}
}
