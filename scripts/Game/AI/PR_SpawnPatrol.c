/*
Author: PapaReap. Inspired from Rabid Squirrel's code from scripts SCR_RS_AISpawner, SCR_RS_HunterKiller

ToDo:
Scale groups
Min Units in groupp
default player count for scaling
Add spawn in random area within radius
Add random spawn area drop down list
Add random group
Maybe make vehicle respawn, with group?
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

//	protected int m_iRespawnTimer;// = 0; not used atm
//	protected int m_iAIMovementType;// = 1; not used atm

	//------------------------------------------------------------------------------------------------
	/*!
	SPAWN GROUP AND HAND OUT WAYPOINTS

	PRSpawnPatrol(
		int spawnSide,
		int groupType,
		vector spawnPosition,
		array<bool> boolArray,
		array<array<string>> stringArray,
		array<int> intArray,
		float perceptionFactor,
		IEntity persistentObject
	)
	*/
	void PRSpawnPatrol(
		int spawnSide,
		int groupType,
		//vector triggerPosition,
		IEntity trigger,
		array<bool> boolArray,
		array<array<string>> stringArray,
		array<int> intArray,
		float perceptionFactor,
		IEntity persistentObject
	)
	{
		string spawnLocationObject;

		SetSpawnSide(spawnSide);
		SetGroupType(groupType);
		//SetTriggerPosition(triggerPosition);
		SetTrigger(trigger);
		//vector spawnPosition = triggerPosition;
		vector spawnPosition = trigger.GetOrigin();
		SetSpawnPosition(spawnPosition);
		vector spawnAngle = trigger.GetAngles();
		SetSpawnAngle(spawnAngle);

		SetPerceptionFactorB(perceptionFactor);

		bool cycleWaypoints = boolArray.Get(0);
		SetCycleWaypoints(cycleWaypoints);
		bool debugLogs = boolArray.Get(1);
		SetDebugLogs(debugLogs);
		bool useRandomRespawnTimer = boolArray.Get(2);
		SetRandomRespawnTimer(useRandomRespawnTimer);
		bool randomGroupSize = boolArray.Get(3);
		SetRandomGroupSize(randomGroupSize);
		bool keepGroupActive = boolArray.Get(4);
		SetKeepGroupActive(keepGroupActive);
		bool logGroupLocation = boolArray.Get(5);
		SetLogGroupLocation(logGroupLocation);
		bool suspendIfNoPlayers = boolArray.Get(6);
		SetSuspendIfNoPlayers(suspendIfNoPlayers);
		bool spawnVehicle = boolArray.Get(7);
		SetSpawnVehicle(spawnVehicle);
		bool teleportAfterSpawn = boolArray.Get(8);
		SetTeleportAfterSpawn(teleportAfterSpawn);
		bool neutralizePersistentObjectIfGroupIsDead = boolArray.Get(9);
		SetNeutralizePersistentObjectIfGroupIsDead(neutralizePersistentObjectIfGroupIsDead);
		
		//resetGroupIfIdle
		bool resetGroupIfIdle = boolArray.Get(10);
		SetResetGroupIfIdle(resetGroupIfIdle);
		// string arrays
		array<string> spawnPositionArray = stringArray.Get(0);
		SetSpawnPositionArray(spawnPositionArray);
		
		//make this a method
		array<IEntity> spawnArray = {};
		if (spawnPositionArray.Count() > 0)
		{
			int spawnPositionCount = spawnPositionArray.Count();
			int _i = 0;
			while (spawnPositionCount > _i)
			{
				array<IEntity> spawnpointArray = {};
				IEntity spHolder = GetGame().GetWorld().FindEntityByName(spawnPositionArray.Get(_i));
				
				GetAllChildren(spHolder, spawnpointArray);
				foreach (IEntity x : spawnpointArray)
				{
					spawnArray.Insert(x);
				}

				_i++;
			}

			array<IEntity> filteredSpawnArray = {};
			if (spawnArray.Count() > 0)
			{
				foreach (IEntity x : spawnArray)
				{
					IEntity player = GetClosestPlayerEntity(x, 500);
					if (!player)
					{
						filteredSpawnArray.Insert(x);
					} else
						Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) Spawn position too close to player for spawn! Checking other positions. : %1", x), LogLevel.WARNING);
				}
			}

			if (filteredSpawnArray.Count() > 0)
			{
				int randomIndex = filteredSpawnArray.GetRandomIndex();
				IEntity spawnObject = filteredSpawnArray.Get(randomIndex);
				spawnPosition = spawnObject.GetOrigin();
				SetSpawnPosition(spawnPosition);
				spawnAngle = spawnObject.GetAngles();
				SetSpawnAngle(spawnAngle);
			}
			else
			{
				//--- Execute the AI spawning using a delayed call if no positions are good. Will check again in 120 sec
				Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) No suitable locations to spawn, trying again in 120 seconds."), LogLevel.WARNING);
				GetGame().GetCallqueue().CallLater(
					PRSpawnPatrol,
					120000, //delay, 2 minutes
					false,
					GetSpawnSide(),//spawnSide,
					groupType,
					//triggerPosition,
					trigger,
					boolArray,
					stringArray,
					intArray,
					perceptionFactor,
					persistentObject
				);
				
				return;
			}
		}
		
		array<string> teleportPosition = stringArray.Get(1);
		SetTeleportPosition(teleportPosition);
		array<string> waypointCollection = stringArray.Get(2);
		SetStringWaypointCollection(waypointCollection);
		array<string> prefabArray = stringArray.Get(3);
		SetPrefabArray(prefabArray);
		array<string> groupIDArray = stringArray.Get(4);
		SetGroupIDArray(groupIDArray);

		if (groupIDArray.Count() > 0)
		{
			string groupID = groupIDArray.Get(0);
			SetGroupID(groupID);
		}
		else
			SetGroupID("");
		
		if (GetDebugLogs())
			Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) GroupID: %1, spawnPosition: %2, spawnAngle: %3", GetGroupID(), spawnPosition, spawnAngle), LogLevel.NORMAL);
		
		int rerunCounter = intArray.Get(0);
		SetRerunCounter(rerunCounter);
		int respawnTimerMin = intArray.Get(1);
		SetRespawnTimerMin(respawnTimerMin);
		int respawnTimerMax = intArray.Get(2);
		SetRespawnTimerMax(respawnTimerMax);
		int respawnCount = intArray.Get(3);
		SetRespawnCount(respawnCount);
		int teleportSortOrder = intArray.Get(4);
		SetTeleportSortOrder(teleportSortOrder);
		int collectionSortOrder = intArray.Get(5);
		SetCollectionSortOrder(collectionSortOrder);
		int waypointSortOrder = intArray.Get(6);
		SetWaypointSortOrder(waypointSortOrder);
		int spawnCollections = intArray.Get(7);
		SetSpawnCollections(spawnCollections);
		int skill = intArray.Get(8);
		SetAISkillB(skill);
		int combatType = intArray.Get(9);
		SetCombatTypeB(combatType);
		int groupFormation = intArray.Get(10);
		SetGroupFormationB(groupFormation);
		int minUnitsInGroup = intArray.Get(11);
		SetMinUnitsInGroup(minUnitsInGroup);
		int maxUnitsInGroup = intArray.Get(12);
		SetMaxUnitsInGroup(maxUnitsInGroup);
		int logUpdateInterval = intArray.Get(13);
		SetLogUpdateInterval(logUpdateInterval);

		// maybe make a custom group count
		string m_SpawnGroup = GetGroupToSpawn(/*spawnSide, */groupType);
		if (GetDebugLogs())
			Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) GetGroupID(): %1, m_SpawnGroup: %1", GetGroupID(), m_SpawnGroup), LogLevel.WARNING);

		SetPersistentObject(persistentObject);

		//--- Generate the resource
		Resource resource = GenerateAndValidateResource(m_SpawnGroup);

		if (!resource)
		{
			Print(("[PR_SpawnPatrol] (PRSpawnPatrol) Unable to load resource for the spawn group: " + m_SpawnGroup), LogLevel.ERROR);
			return;
		}

		if (GetDebugLogs())
			Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) resource: %1", resource), LogLevel.WARNING);

		//--- Generate spawn parameters and spawn the group
		string specificPrefabName;
		IEntity patrolVehicle;
		if (spawnVehicle && prefabArray.Count() > 0)
		{
			specificPrefabName = prefabArray.Get(0);

			Resource resourceVeh = GenerateAndValidateResource(specificPrefabName);
			patrolVehicle = SpawnPrefab(resourceVeh, spawnPosition);
			SetPatrolVehicle(patrolVehicle);
			patrolVehicle.SetAngles(GetSpawnAngle());
			SetFirstRun(true);
			if (GetDebugLogs())
				Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) GetGroupID(): %1, patrolVehicle: %2", GetGroupID(), patrolVehicle), LogLevel.WARNING);
			
		/*	DamageManagerComponent damageComponent = DamageManagerComponent.Cast(patrolVehicle.FindComponent(DamageManagerComponent));
			// Destroyed?
			if (damageComponent && damageComponent.GetState() == EDamageState.DESTROYED)
			{
				//Deploy(SCR_EMobileAssemblyStatus.DESTROYED);
				
				//return;
			}*/
		}

		m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPosition)));

		if (!m_Group)
		{
			Print("[PR_SpawnPatrol] (PRSpawnPatrol) Unable to spawn group!", LogLevel.ERROR);
			return;
		}

		SetOldVector(m_Group.GetOrigin());
		
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

			if (GetDebugLogs())
				Print(string.Format("[PRSpawnPatrol] (PRSpawnPatrol) unitsToSpawn: %1", unitsToSpawn), LogLevel.NORMAL);

			if (prefabUnitCount >= unitsToSpawn)
			{
				removeUnitCount = prefabUnitCount - unitsToSpawn;
			}
			else
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

		//--- Create waypoints for the group
		SetGroupWaypoints(m_Group);
		
		//--- For group information, needs a sleep it seems
		GetGame().GetCallqueue().CallLater(GetGroupAgents, 5000, false, m_Group, removeUnitCount);
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aStringWaypointCollection
	protected array<string> m_aStringWaypointCollection;
	void SetStringWaypointCollection(array<string> stringWaypointCollection)
	{
		m_aStringWaypointCollection = stringWaypointCollection;
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SetStringWaypointCollection) m_aStringWaypointCollection : %1", m_aStringWaypointCollection), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aStringWaypointCollection
	protected array<string> GetStringWaypointCollection()
	{
		return m_aStringWaypointCollection;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aPrefabArray
	protected array<string> m_aPrefabArray;
	void SetPrefabArray(array<string> prefabArray)
	{
		m_aPrefabArray = prefabArray;
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SetPrefabArray) m_aPrefabArray : %1", m_aPrefabArray), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aPrefabArray
	protected array<string> GetPrefabArray()
	{
		return m_aPrefabArray;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aGroupIDArray
	protected array<string> m_aGroupIDArray;
	void SetGroupIDArray(array<string> groupIDArray)
	{
		m_aGroupIDArray = groupIDArray;
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SetGroupIDArray) m_aGroupIDArray : %1", m_aGroupIDArray), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aGroupIDArray
	protected array<string> GetGroupIDArray()
	{
		return m_aGroupIDArray;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_sGroupID
	protected string m_sGroupID;
	void SetGroupID(string groupID)
	{
		m_sGroupID = groupID;
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SetGroupID) groupID: (%1)", m_sGroupID), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_sGroupID
	protected string GetGroupID()
	{
		return m_sGroupID;
	}

	void SetGroupWaypoints(SCR_AIGroup groupWP)
	{
		bool cycleWaypoints = GetCycleWaypoints();
		//bool spawnVehicle = GetSpawnVehicle(); // don't think i'll use this, unless I want to respawn? maybe!
		bool neutralizePersistentObjectIfGroupIsDead = GetNeutralizePersistentObjectIfGroupIsDead();

		array<string> waypointCollection = GetStringWaypointCollection();
		
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SetGroupWaypoints) waypointCollection : %1", waypointCollection), LogLevel.NORMAL);

		int rerunCounter = GetRerunCounter();
		int collectionSortOrder = GetCollectionSortOrder();
		int waypointSortOrder = GetWaypointSortOrder();
		int respawnCount = GetRespawnCount();
		int spawnCollections = GetSpawnCollections();

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
		
		if (GetDebugLogs())
			Print(("[PR_SpawnPatrol] (SetGroupWaypoints) waypointCollectionSort: " + waypointCollection + "  collectionSortOrder: " + collectionSortOrder + "  waypointSortOrder: " + waypointSortOrder), LogLevel.NORMAL);

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
				if (GetDebugLogs())
				{
					Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
					Print(("PR count: " + count), LogLevel.NORMAL);
				}
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
					if (GetDebugLogs())
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
				if (GetDebugLogs())
				{
					Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
					Print(("PR count: " + count), LogLevel.NORMAL);
				}
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
					if (GetDebugLogs())
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
				if (GetDebugLogs())
				{
					Print(("PR float 1 _i: " + _i), LogLevel.NORMAL);
					Print(("PR count: " + count), LogLevel.NORMAL);
				}
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
					if (GetDebugLogs())
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

					GetAllChildren(wpHolder, waypointArray);

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

							if (GetDebugLogs())
								Print(("[PR_SpawnPatrol] (SetGroupWaypoints) randomIndex Index: " + randomIndex + " Name: " + name), LogLevel.NORMAL);
						}
						namesArray = namesArrayRandom;
						if (GetDebugLogs())
							Print(("[PR_SpawnPatrol] (SetGroupWaypoints) Randomized namesArray: " + namesArray), LogLevel.NORMAL);
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

			groupWP.AddWaypointToGroup(waypoint);
			cycleWaypointArray.Insert(waypoint);
			if (GetDebugLogs())
				Print(("[PR_SpawnPatrol] (SetGroupWaypoints) cycleWaypointArray Index: " + index + " Name: " + name), LogLevel.NORMAL);
		}

		//--- Set waypoints to cycle if enabled
		if (cycleWaypointArray.IsEmpty() && GetDebugLogs())
			Print("[PR_SpawnPatrol] (SetGroupWaypoints) Cycle waypoints were empty", LogLevel.NORMAL);

		if (!cycleWaypointArray.IsEmpty() && cycleWaypoints)
		{
			cycleWaypointArray.Insert(cycleWaypointArray[0]);
			AddCycleWaypoint(groupWP, cycleWaypointArray.Get(0).GetOrigin(), cycleWaypointArray, rerunCounter);
			if (GetDebugLogs())
				Print(("[PR_SpawnPatrol] (SetGroupWaypoints) CycleWaypoints: " + cycleWaypointArray), LogLevel.NORMAL);
		}

		//--- Stuff to do after group death
		if (respawnCount != 0)
		{
			Print(string.Format("[PR_SpawnPatrol] (SetGroupWaypoints) Setting Group respawn parameters. respawnCount: %1", respawnCount), LogLevel.WARNING);
			SetWaypointCollection(waypointCollection);
			SetRespawnCount(respawnCount);
			groupWP.GetOnEmpty().Insert(AfterGroupIsEmpty);
		}
		else
		{
			if (GetDebugLogs())
				Print(("[PR_SpawnPatrol] (SetGroupWaypoints) neutralizePersistentObjectIfGroupIsDead: " + neutralizePersistentObjectIfGroupIsDead), LogLevel.WARNING);
			if (neutralizePersistentObjectIfGroupIsDead)
				groupWP.GetOnEmpty().Insert(AfterGroupIsEmpty);
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Spawn Prefab
	//protected void SpawnPrefab(Resource resource, vector spawnPos)
	protected IEntity SpawnPrefab(Resource resource, vector spawnPos)
	{
		//StaticModelEntity spawnedPrefab = StaticModelEntity.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPos)));
		IEntity spawnedPrefab = GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPos));
		if (GetDebugLogs())
			Print(string.Format("[PR_SpawnPatrol] (SpawnPrefab) spawnedPrefab: %1", spawnedPrefab), LogLevel.WARNING);
		return spawnedPrefab;
	}

	//------------------------------------------------------------------------------------------------
	//! Teleports units to position after spawning, useful for placing over water, in buildings, etc...
	void TeleportObject(string whatToMove, string whereToMove)
	{
		IEntity objectToTeleport = GetGame().GetWorld().FindEntityByName(whatToMove);
		IEntity objectToTeleportTo = GetGame().GetWorld().FindEntityByName(whereToMove);
		vector position = objectToTeleportTo.GetOrigin();
		if (GetDebugLogs())
			Print(("[PR_SpawnPatrol] (TeleportObject) objectToTeleport: " + objectToTeleport + " objectToTeleportTo: " + objectToTeleportTo + " position: " + position), LogLevel.NORMAL);
		objectToTeleport.SetOrigin(position)
	}

	//------------------------------------------------------------------------------------------------
	//! Gets groupGA agents
	void GetGroupAgents(SCR_AIGroup groupGA, int removeUnitCount)
	{
		if (!groupGA)
			return;

		//--- Create waypoints for the group
	//	SetGroupWaypoints(groupGA);

		bool keepGroupActive = GetKeepGroupActive();

		// teleport after spawn
		bool teleportAfterSpawn = GetTeleportAfterSpawn();
		array<string> teleportPosition = GetTeleportPosition();

		int teleportSortOrder = GetTeleportSortOrder();
		string whereToMove = "";

		array<AIAgent> agents = {};
		groupGA.GetAgents(agents);
		if (GetDebugLogs())
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
			if (GetDebugLogs())
				Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) agents.Count(): %1", agents.Count()), LogLevel.NORMAL);
		}

		IEntity patrolVehicle;
		if (GetSpawnVehicle())
		{
			patrolVehicle = GetPatrolVehicle();
			array<string> groupVehicles = {};
			string vehicle = patrolVehicle.ToString();
			groupVehicles.Insert(vehicle);
			groupGA.SetGroupVehicles(groupVehicles);

			BaseCompartmentManagerComponent slotCompMan = BaseCompartmentManagerComponent.Cast(patrolVehicle.FindComponent(BaseCompartmentManagerComponent));
			array<BaseCompartmentSlot> vehicleCompartments = {};
			int spaces = slotCompMan.GetCompartments(vehicleCompartments);
			for (int j = 0; (j < agents.Count() && spaces > j); j++)
			{
				AIAgent member = agents[j];
				if (member)
				{
					SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(member.GetControlledEntity());
					CompartmentAccessComponent CAComp = CompartmentAccessComponent.Cast(character.FindComponent(CompartmentAccessComponent));
					if (CAComp && character && !character.IsInVehicle())
					{
						BaseCompartmentSlot slot = vehicleCompartments[j];
						if (GetDebugLogs())
							Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) slot: %1", slot), LogLevel.NORMAL);
						if (slot)
							CAComp.GetInVehicle(patrolVehicle, slot, true, -1, ECloseDoorAfterActions.INVALID, false);
					}
				}
			}
		}

		AIWaypoint currentWaypoint = groupGA.GetCurrentWaypoint(); // works
		if (GetDebugLogs())
			Print(("[PR_SpawnPatrol] (GetGroupAgents) currentWaypoint: " + currentWaypoint), LogLevel.NORMAL);

		int groupSize = groupGA.GetAgentsCount();
		if (GetDebugLogs())
			Print(("[PR_SpawnPatrol] (GetGroupAgents) groupSize: " + groupSize), LogLevel.NORMAL);

		// Behaviors
		int skill = GetAISkillB();
		int combatType = GetCombatTypeB();
		int groupFormation = GetGroupFormationB();
		float perceptionFactor = GetPerceptionFactorB();
		string m_sPath = "$EnfusionPersistenceFramework:Scripts/Game/EPF_PersistenceComponent.c";
		
	//	if (FileIO.FileExists(m_sPath))
	//	{
	//		bool m_bEPF_ModExist = false;
			//EPF_PersistenceComponent persistence = EPF_PersistenceComponent.Cast(entity.FindComponent(EPF_PersistenceComponent));
	//	}

		if (teleportAfterSpawn)
		{
			whereToMove = teleportPosition.Get(0);
			if (GetDebugLogs())
				Print(("[PR_SpawnPatrol] (GetGroupAgents) whereToMove: " + whereToMove), LogLevel.NORMAL);

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
					if (GetDebugLogs())
						Print(("[PR_SpawnPatrolTrigger] (GetGroupAgents) agentName: " + agentName), LogLevel.NORMAL);
				}
			}

			SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(agentEntity.FindComponent(SCR_AICombatComponent));
			EAISkill aiSkill;
			EAICombatType aiCombatType;
			if (combatComponent)
			{
				combatComponent.SetAISkill(skill);
				combatComponent.SetCombatType(combatType);
				combatComponent.SetPerceptionFactor(perceptionFactor);

				aiSkill = combatComponent.GetAISkill();
				aiCombatType = combatComponent.GetCombatType();
				if (GetDebugLogs())
				{
					Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) aiSkill: %1", aiSkill), LogLevel.WARNING);
					Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) aiCombatType: %1", aiCombatType), LogLevel.WARNING);
				}
			}

			SCR_AIInfoComponent infoComponent;
			infoComponent = SCR_AIInfoComponent.Cast(agentEntity.FindComponent(SCR_AIInfoComponent));
		}

		// Set formation
		AIFormationComponent formComp = AIFormationComponent.Cast(groupGA.FindComponent(AIFormationComponent));
		if (!formComp)
		{
			Print(string.Format("[PR_SpawnPatrol] (GetGroupAgents) ScenarioFramework Action: AI Formation Component not found for Action %1.", this), LogLevel.ERROR);
			return;
		}
		formComp.SetFormation(SCR_Enum.GetEnumName(SCR_EAIGroupFormation, groupFormation));

		array<int> behaviorArray = {skill, combatType, groupFormation};

		if (keepGroupActive || GetLogGroupLocation())
			GetGame().GetCallqueue().CallLater(KeepGroupActive, 60000, true, groupGA, agents);
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iKGACounter
	protected int m_iKGACounter = 0;
	void SetKGACounter(int counter)
	{
		m_iKGACounter = counter;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iKGACounter;
	int GetKGACounter()
	{
		return m_iKGACounter;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_iLogUpdateCounter
	protected int m_iLogUpdateCounter = 0;
	void SetLogUpdateCounter(int logUpdateCounter)
	{
		m_iLogUpdateCounter = logUpdateCounter;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iLogUpdateCounter;
	int GetLogUpdateCounter()
	{
		return m_iLogUpdateCounter;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_iGroupIdleCounter
	protected int m_iGroupIdleCounter = 0;
	void SetGroupIdleCounter(int groupIdleCounter)
	{
		m_iGroupIdleCounter = groupIdleCounter;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iGroupIdleCounter;
	int GetGroupIdleCounter()
	{
		return m_iGroupIdleCounter;
	}

	//------------------------------------------------------------------------------------------------
	//! Keeps group active, even when no near player in area
	void KeepGroupActive(SCR_AIGroup kgaGroup, array<AIAgent> agents)
	{
		if (!kgaGroup || !agents || agents.Count() == 0)
		{
			GetGame().GetCallqueue().Remove(KeepGroupActive);
			return;
		}

		AIAgent leader = kgaGroup.GetLeaderAgent();
		if (!leader)
			return;

		int lod = leader.GetLOD();
		
		string groupID = GetGroupID();
		if (groupID == "")
			groupID = "No ID Given";
		
		int playerCount = GetGame().GetPlayerManager().GetPlayerCount();

		// maybe get idle check and not moving check, maybe store waypoints to restore after player returns? check cycle waypoints
		// maybe kill and cleanup non moving if not supposed to be, check for nearby players first
		if (GetLogGroupLocation() && playerCount > 0)
		{
			int logUpdateCounter = GetLogUpdateCounter();
			SetLogUpdateCounter(logUpdateCounter + 1);
Print(string.Format("[PR_SpawnPatrol] (KeepGroupActive) GetLogUpdateCounter(): %1  GetLogUpdateInterval(): %2", GetLogUpdateCounter(), GetLogUpdateInterval()), LogLevel.ERROR);
			if (GetLogUpdateCounter() >= GetLogUpdateInterval())
			{
				if (lod < 10)
				{
					vector oldPos = GetOldVector();
					vector pos = kgaGroup.GetOrigin();
					float distance = vector.DistanceXZ(oldPos, pos);
					
					AIWaypoint currentWaypoint = kgaGroup.GetCurrentWaypoint();
					
					if (currentWaypoint)
					{
						float wpDistance = vector.DistanceXZ(currentWaypoint.GetOrigin(), kgaGroup.GetOrigin());
						array<AIWaypoint> gWaypoints = {};

						int waypointCount = kgaGroup.GetWaypoints(gWaypoints);
						if (GetDebugLogs())
						{
							//Print(string.Format("[PR_SpawnPatrol] (KeepGroupActive) groupID: %1, distance to: %2, current waypoint: %3", groupID, wpDistance, currentWaypoint), LogLevel.WARNING);
							if (gWaypoints.Count() > 0)
							{
								int gridX, gridZ;
								SCR_MapEntity.GetGridPos(pos, gridX: gridX, gridZ: gridZ);
								
								string wpName = currentWaypoint.GetName();
								vector wpPos = currentWaypoint.GetOrigin();
								int wpGridX, wpGridZ;
								SCR_MapEntity.GetGridPos(wpPos, gridX: wpGridX, gridZ: wpGridZ);
								
								string contName = "Unknown";
								EntityPrefabData prefabData = currentWaypoint.GetPrefabData();
								if (prefabData)
								{
									BaseContainer cont = prefabData.GetPrefab();
									contName = cont.GetName();
								}
								
								Print(string.Format("[PR_SpawnPatrol] (KeepGroupActive) groupID: (%1)  Grid:(0%2,0%3)  Distance traveled:(%4)  Waypoint count:(%5)  Current waypoint:(%6) Grid:(0%7,0%8)   %9",
								groupID, gridX, gridZ, distance, gWaypoints.Count(), wpName, wpGridX, wpGridZ, contName), LogLevel.WARNING);
							}
						}
					} else
						Print(string.Format("[PR_SpawnPatrol] (KeepGroupActive) groupID: (%1), No Current Waypoint found!", groupID), LogLevel.ERROR);
					
					SetOldVector(pos);
				} else
					Print(string.Format("[PR_SpawnPatrol] (KeepGroupActive) groupID: (%1) is inactive.", groupID), LogLevel.NORMAL);
				
				SetLogUpdateCounter(0);
			}
		}

		if (!GetKeepGroupActive())
		{
			return;
		}
		
		bool suspendIfNoPlayers = GetSuspendIfNoPlayers();
		bool firstRun = false;

		if (playerCount == 0 && suspendIfNoPlayers)
		{
			if (GetKGACounter() >= 10)
			{
				Print(string.Format("[PR_SpawnPatrol]  (KeepGroupActive) playerCount: %1  Suspending group: (%2)", playerCount, groupID), LogLevel.WARNING);
				m_iKGACounter = 0;
				SetKGACounter(m_iKGACounter);
			}
			SetKGACounter(m_iKGACounter++);

			if (lod < 10)
			{
				foreach (int index, AIAgent agent : agents)
				{
					if (agent)
					{
						agent.SetLOD(10);
						agent.AllowMaxLOD();
					}
				}
				Print(string.Format("[PR_SpawnPatrol]  (KeepGroupActive) groupID: (%1)  Adjusting LOD to AllowMaxLOD()", groupID), LogLevel.WARNING);
			}
		}
		else
		{
			if ((lod == 10 && !GetSpawnVehicle()) || (GetSpawnVehicle() && lod < 10 && GetFirstRun()))
			{
				foreach (int index, AIAgent agent : agents)
				{
					if (agent)
					{
						agent.SetLOD(9);
						agent.PreventMaxLOD();
					}
				}
				SetFirstRun(false);
				Print(string.Format("[PR_SpawnPatrol]  (KeepGroupActive) groupID: (%1)  Adjusting LOD to PreventMaxLOD()", groupID), LogLevel.WARNING);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bFirstRun
	protected bool m_bFirstRun;
	void SetFirstRun(bool firstRun)
	{
		m_bFirstRun = firstRun;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bFirstRun;
	bool GetFirstRun()
	{
		return m_bFirstRun;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finds closest player from another entity
	protected IEntity GetClosestPlayerEntity(IEntity entityFrom, int distance)
	{
		if (!entityFrom)
			return null;

		array<int> playerIDs = {};
		GetGame().GetPlayerManager().GetPlayers(playerIDs);

		IEntity closestEntity;
		IEntity entityToBeChecked;

		foreach (int playerID : playerIDs)
		{
			entityToBeChecked = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
			if (!entityToBeChecked)
				continue;

			float actualDistance = vector.DistanceXZ(entityFrom.GetOrigin(), entityToBeChecked.GetOrigin());
			if (GetDebugLogs())
				Print(string.Format("[PR_SpawnPatrol] (GetClosestPlayerEntity) actualDistance: %1", actualDistance), LogLevel.WARNING);
			if (actualDistance < distance)
			{
				closestEntity = entityToBeChecked;
				distance = actualDistance;
			}
		}

		if (!closestEntity)
			return null;

		return closestEntity;
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
		string groupID = GetGroupID();
		if (groupID == "")
			groupID = "No ID Given";
		
		Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) : groupID: (%1)  Starting new group session...", groupID), LogLevel.WARNING);
		//Print(string.Format("[PR_SpawnPatrol] AfterGroupIsEmpty : Starting... RplSession.Mode(): %1", RplSession.Mode()), LogLevel.WARNING);
		//if (!Replication.IsRunning())
		if (!Replication.IsRunning() && (RplSession.Mode() == RplMode.Dedicated)) // this needs testing on server
		{
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) : groupID: (%1)  Ending session...", groupID), LogLevel.WARNING);
			return;
		}

		//bool neutralizePersistentObjectIfGroupIsDead = GetNeutralizePersistentObjectIfGroupIsDead();
		//IEntity persistentObject = GetPersistentObject();
		int respawnCount = GetRespawnCount();

		if (GetNeutralizePersistentObjectIfGroupIsDead())
		{
			if (GetPersistentObject())
			{
				Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) : persistentObject: %1", GetPersistentObject().GetName()), LogLevel.WARNING);
				KillUnit(GetPersistentObject());
				SCR_EntityHelper.DeleteEntityAndChildren(GetPersistentObject());
				if (respawnCount == 0)
					return;
			}
		}

		if (respawnCount != 0)
			respawnCount--;
		SetRespawnCount(respawnCount);

		//int spawnSide = GetSpawnSide();
		//int groupType = GetGroupType();
		//IEntity trigger = GetTrigger();
		//vector triggerPosition = GetTriggerPosition();
		//bool cycleWaypoints = GetCycleWaypoints();
		//bool useRandomRespawnTimer = GetRandomRespawnTimer();
		//bool randomGroupSize = GetRandomGroupSize();
		//int minUnitsInGroup = GetMinUnitsInGroup();
		//int maxUnitsInGroup = GetMaxUnitsInGroup();
		//int getLogUpdateInterval = GetLogUpdateInterval();
		//bool keepGroupActive = GetKeepGroupActive();
		//bool logGroupLocation = GetLogGroupLocation();
		//bool suspendIfNoPlayers = GetSuspendIfNoPlayers();
		//bool spawnVehicle = GetSpawnVehicle();
		//bool teleportAfterSpawn = GetTeleportAfterSpawn();
		//bool resetGroupIfIdle = GetResetGroupIfIdle();
		array<bool> boolArray = {
			GetCycleWaypoints(),//cycleWaypoints,							// 0
			GetDebugLogs(),							// 1
			GetRandomRespawnTimer(),//useRandomRespawnTimer,					// 2
			GetRandomGroupSize(),//randomGroupSize,						// 3
			GetKeepGroupActive(),//keepGroupActive,						// 4
			GetLogGroupLocation(),//logGroupLocation,						// 5
			GetSuspendIfNoPlayers(),//suspendIfNoPlayers,						// 6
			GetSpawnVehicle(),//spawnVehicle, 							// 7
			GetTeleportAfterSpawn(),//teleportAfterSpawn,						// 8
			GetNeutralizePersistentObjectIfGroupIsDead(),//neutralizePersistentObjectIfGroupIsDead,// 9
			GetResetGroupIfIdle()//resetGroupIfIdle						// 10
		};

		//array<string> spawnPositionArray = GetSpawnPositionArray();
		//array<string> teleportPosition = GetTeleportPosition();
		//array<string> waypointCollection = GetWaypointCollection();
		//array<string> prefabArray = GetPrefabArray();
		//array<string> groupIDArray = GetGroupIDArray();
		array<array<string>> stringArray = {
			GetSpawnPositionArray(),//spawnPositionArray,	// 0
			GetTeleportPosition(),//teleportPosition,	// 1
			GetWaypointCollection(),//waypointCollection,	// 2
			GetPrefabArray(),//prefabArray,		// 3
			GetGroupIDArray()//groupIDArray		// 4
		};
		//int rerunCounter = GetRerunCounter();
		//int respawnTimerMin = GetRespawnTimerMin();
		//int respawnTimerMax = GetRespawnTimerMax();
		//int teleportSortOrder = GetTeleportSortOrder();
		//int collectionSortOrder = GetCollectionSortOrder();
		//int waypointSortOrder = GetWaypointSortOrder();

		//int skill = GetAISkillB();
		//int combatType = GetCombatTypeB();
		//int groupFormation = GetGroupFormationB();
		//float perceptionFactor = GetPerceptionFactorB();

		//int spawnCollections = GetSpawnCollections();
		array<int> intArray = {
			GetRerunCounter(),//rerunCounter,		// 0
			GetRespawnTimerMin(),//respawnTimerMin,	// 1
			GetRespawnTimerMax(),//respawnTimerMax,	// 2
			GetRespawnCount(),//respawnCount,		// 3
			GetTeleportSortOrder(),//teleportSortOrder,	// 4
			GetCollectionSortOrder(),//collectionSortOrder,// 5
			GetWaypointSortOrder(),//waypointSortOrder,	// 6
			GetSpawnCollections(),//spawnCollections,	// 7
			GetAISkillB(),//skill,				// 8
			GetCombatTypeB(),//combatType,			// 9
			GetGroupFormationB(),//groupFormation,		// 10
			GetMinUnitsInGroup(),//minUnitsInGroup,	// 11
			GetMaxUnitsInGroup(),//maxUnitsInGroup,	// 12
			GetLogUpdateInterval()//getLogUpdateInterval// 13
		};

		int delay = GetRespawnTimerMin() * 1000;
		if (GetRandomRespawnTimer())
		{
			delay = Math.RandomInt(GetRespawnTimerMin() * 1000, GetRespawnTimerMax() * 1000);
		}

		if (GetDebugLogs())
		{
			/*Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) rerunCounter: " + rerunCounter, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnTimerMin: " + respawnTimerMin, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnTimerMax: " + respawnTimerMax, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) respawnCount: " + respawnCount, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) teleportPosition: " + teleportPosition, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) waypointCollection: " + waypointCollection, LogLevel.NORMAL);
			Print("[PR_SpawnPatrol] (AfterGroupIsEmpty) delay: " + delay, LogLevel.NORMAL);*/
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) spawnSide: %1", GetSpawnSide()), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) groupType: %1", GetGroupType()), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) trigger: %1", GetTrigger()), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) boolArray: %1", boolArray), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) stringArray: %1", stringArray), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) intArray: %1", intArray), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) perceptionFactor: %1", GetPerceptionFactorB()), LogLevel.NORMAL);
			Print(string.Format("[PR_SpawnPatrol] (AfterGroupIsEmpty) persistentObject: %1", GetPersistentObject()), LogLevel.NORMAL);
		}

		//--- Execute the AI spawning using a delayed call
		GetGame().GetCallqueue().CallLater(
			PRSpawnPatrol,
			delay,
			false,
			GetSpawnSide(),//spawnSide,
			GetGroupType(),//groupType,
			GetTrigger(),//trigger,
			boolArray,
			stringArray,
			intArray,
			GetPerceptionFactorB(),//perceptionFactor,
			GetPersistentObject()//persistentObject
		);
	}

	//! SETTERS/GETTERS

	//------------------------------------------------------------------------------------------------
	//! sets m_bSpawnVehicle
	protected bool m_bSpawnVehicle;
	void SetSpawnVehicle(bool spawnVehicle)
	{
		m_bSpawnVehicle = spawnVehicle;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bSpawnVehicle;
	bool GetSpawnVehicle()
	{
		return m_bSpawnVehicle;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_PatrolVehicle
	protected IEntity m_PatrolVehicle;
	void SetPatrolVehicle(IEntity patrolVehicle)
	{
		m_PatrolVehicle = patrolVehicle;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_PatrolVehicle;
	IEntity GetPatrolVehicle()
	{
		return m_PatrolVehicle;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_vOldVector
	protected vector m_vOldVector;
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
	protected int m_iAISkill;
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
	protected int m_iAICombatType;
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
	//! sets m_iAIGroupFormation
	protected int m_iAIGroupFormation;
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
	protected float m_fPerceptionFactor;
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
	protected IEntity m_PersistentObject;
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
	protected int m_iSpawnSide;
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
	protected int m_iGroupType;
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
/*
	//------------------------------------------------------------------------------------------------
	//! sets m_vTriggerPosition
	protected vector m_vTriggerPosition;
	void SetTriggerPosition(vector triggerPosition)
	{
		m_vTriggerPosition = triggerPosition;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_vTriggerPosition
	vector GetTriggerPosition()
	{
		return m_vTriggerPosition;
	}
*/	
	//------------------------------------------------------------------------------------------------
	//! sets m_iTrigger
	protected IEntity m_iTrigger;
	void SetTrigger(IEntity trigger)
	{
		m_iTrigger = trigger;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iTrigger
	IEntity GetTrigger()
	{
		return m_iTrigger;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_aSpawnPositionArray
	protected array<string> m_aSpawnPositionArray;
	void SetSpawnPositionArray(array<string> spawnPositionArray)
	{
		m_aSpawnPositionArray = spawnPositionArray;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_aSpawnPositionArray
	array<string> GetSpawnPositionArray()
	{
		return m_aSpawnPositionArray;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_vSpawnPosition
	protected vector m_vSpawnPosition;
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
	//! sets m_vSpawnAngle
	protected vector m_vSpawnAngle;
	void SetSpawnAngle(vector spawnAngle)
	{
		m_vSpawnAngle = spawnAngle;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_vSpawnAngle
	vector GetSpawnAngle()
	{
		return m_vSpawnAngle;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_bCycleWaypoints
	protected bool m_bCycleWaypoints;
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
	protected bool m_bDebugLogs;
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
	protected bool m_bNeutralizePersistentObjectIfGroupIsDead;
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
	protected bool m_bRandomGroupSize;
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
	protected int m_iMinUnitsInGroup;
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
	protected int m_iMaxUnitsInGroup;
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
	//! sets m_iLogUpdateInterval
	protected int m_iLogUpdateInterval;
	void SetLogUpdateInterval(int logUpdateInterval)
	{
		m_iLogUpdateInterval = logUpdateInterval;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_iLogUpdateInterval
	int GetLogUpdateInterval()
	{
		return m_iLogUpdateInterval;
	}

	//------------------------------------------------------------------------------------------------
	//! sets m_bKeepGroupActive
	void SetKeepGroupActive(bool keepGroupActive)
	{
		m_bKeepGroupActive = keepGroupActive;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bKeepGroupActive
	protected bool m_bKeepGroupActive;
	bool GetKeepGroupActive()
	{
		return m_bKeepGroupActive;
	}

	//SetLogGroupLocation(logGroupLocation);
	//------------------------------------------------------------------------------------------------
	//! sets m_bLogGroupLocation
	void SetLogGroupLocation(bool logGroupLocation)
	{
		m_bLogGroupLocation = logGroupLocation;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bLogGroupLocation
	protected bool m_bLogGroupLocation;
	bool GetLogGroupLocation()
	{
		return m_bLogGroupLocation;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_bSuspendIfNoPlayers
	protected bool m_bSuspendIfNoPlayers;
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
	//! sets m_bResetGroupIfIdle
	protected bool m_bResetGroupIfIdle;
	void SetResetGroupIfIdle(bool resetGroupIfIdle)
	{
		m_bResetGroupIfIdle = resetGroupIfIdle;
	}

	//------------------------------------------------------------------------------------------------
	//! returns m_bResetGroupIfIdle
	bool GetResetGroupIfIdle()
	{
		return m_bResetGroupIfIdle;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets m_bTeleportAfterSpawn
	protected bool m_bTeleportAfterSpawn;
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
	protected bool m_bUseRandomRespawnTimer;
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
	protected array<string> m_aTeleportPosition;
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
	protected array<string> m_aWaypointCollection;
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
	protected int m_iRerunCounter;
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
	protected int m_iRespawnTimerMin;
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
	protected int m_iRespawnTimerMax;
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
	protected int m_iRespawnCount;
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
	protected int m_iTeleportSortOrder;
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
	protected int m_iCollectionSortOrder;
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
	protected int m_iWaypointSortOrder;
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
	protected int m_iSpawnCollections;
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
	void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren)
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
	void AddCycleWaypoint(SCR_AIGroup groupCW, vector waypointPosition, array<AIWaypoint> cycleWaypointArray, int rerunCounter)
	{
		Resource resource = Resource.Load("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et");
		AIWaypointCycle waypoint = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(waypointPosition)));
		waypoint.SetWaypoints(cycleWaypointArray);
		waypoint.SetRerunCounter(rerunCounter);
		groupCW.AddWaypoint(waypoint);
		if (GetDebugLogs())
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
	protected string GetGroupToSpawn(/*int spawnSide, */int groupType)
	{
		/*
		USSR
		{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et
		FIA
		{EE92725E9B949C3D}Prefabs/Groups/INDFOR/Group_FIA_PlatoonHQ.et
		{2E9C920C3ACA2C6F}Prefabs/Groups/INDFOR/Group_FIA_ReconTeam.et
		*/
		if (GetSpawnSide() == 0) // US
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
			if (GetSpawnSide() == 1) // USSR
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
				if (GetSpawnSide() == 2) // FIA
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
					if (GetSpawnSide() == 3) // CIV
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
