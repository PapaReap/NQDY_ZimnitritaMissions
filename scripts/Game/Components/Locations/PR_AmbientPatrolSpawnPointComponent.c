modded class SCR_AmbientPatrolSpawnPointComponent : ScriptComponent
{
	//--- PapaReap >>>
	
	static string hintSimulateGamePlayerCountForScaling1 = (
	"This is a simulated test for estimating actual group spawn count." + "<br />" +
	"Will output value into the 'Log Console' for evaluation." + "<br />" +
	"---------------------------------------------------------------------------------------------------" + "<br />" +
	"Input values into this box to simulate ingame player count." + "<br />" + "<br />" +
	"" + "<br />" +
	".  " + "<br />" + "<br />" +
	"" + "<br />" +
	""
	);
	
	[Attribute(defvalue: "8", desc: "Simulated test for estimating actual group spawn count. Will output value into the 'Log Console'.", category: "Group Scaling")]
	int m_iSimulateGamePlayerCountForScaling;

	[Attribute(defvalue: "0", desc: "Scale group sizes based on number of players.", category: "Group Scaling")]
	bool m_bBalanceOnPlayersCount;
	
	[Attribute(defvalue: "1", desc: "This overrides the mission header to get player count. Uses only 'Optimum Player Count For Scaling'", category: "Group Scaling")]
	bool m_bIgnoreMissionHeader;

	static string hintOptimumPlayerCountForScaling1 = (
	"Optimum player count used for scaling when no mission header is found.  " + "<br />" +
	"---------------------------------------------------------------------------------------------------" + "<br />" +
	"When the player count is at this value," + "<br />" +
	"the group will spawn at or near the 'High Unit Scale'.  " + "<br />" + "<br />" +
	"Less players than optimum and the group will spawn less members," + "<br />" +
	"down to the 'Min Units In Group' amount.  " + "<br />" + "<br />" +
	"More players than optimum and the group will spawn more members," + "<br />" +
	"up to the 'Max Units In Group' amount."
	);
	static string hintOptimumPlayerCountForScaling2 = (
	"<br />" +
	"---------------------------------------------------------------------------------------------------" + "<br />" +
	"Examples:" + "<br />" + "<br />" +
	"Optimum Player Count For Scaling: 8" + "<br />" +
	"Min Units In Group: 4" + "<br />" +
	"High Unit Scale: 12" + "<br />" +
	"Actual players in mission: 5" + "<br />"  +
	"Actual units spawned = 8"
	);
	static string hintOptimumPlayerCountForScaling3 = (
	"<br />" +
	"---------------------------------------------------------------------------------------------------" + "<br />" +
	"Optimum Player Count For Scaling: 8" + "<br />" +
	"Min Units In Group: 4" + "<br />" +
	"High Unit Scale: 12" + "<br />" +
	"Actual players in mission: 11" + "<br />"  +
	"Actual units spawned = 15"
	);
	static string hintOptimumPlayerCountForScaling = (hintOptimumPlayerCountForScaling1 + hintOptimumPlayerCountForScaling2 + hintOptimumPlayerCountForScaling3);
	[Attribute(defvalue: "8", desc: hintOptimumPlayerCountForScaling, category: "Group Scaling")]
	int m_iOptimumPlayerCountForScaling;

	[Attribute(defvalue: "1", desc: "Minimum amount of AIs in the group that will be spawned.  ", category: "Group Scaling")]
	int m_iMinUnitsInGroup;

	[Attribute(defvalue: "-1", desc: "This is the amount of units that will be spawned if actual player count equals 'Optimum Player Count For Scaling'.  ", category: "Group Scaling")]
	int m_iUnitScaleForOptimumPlayerCount;

	static string hintMaxUnitsInGroup = (
	"----------------------------------------------------------------------------------" + "<br />" +
	"Maximum amount of AIs in the group that can be spawned." + "<br />" +
	"-1 has no effect, group size will not be limited."
	);
	[Attribute(defvalue: "-1", desc: hintMaxUnitsInGroup, category: "Group Scaling")]
	int m_iMaxUnitsInGroup;

	static string hintRandomGroupSize = (
	"---------------------------------------------------------------------------------" + "<br />" +
	"Randomized group size based on a random value between" + "<br />" +
	"'Min Units In Group' and 'Max Units In Group' from above." + "<br />" + "<br />" +
	"This overrides 'Balance On Players Count' from above"
	);
	[Attribute(defvalue: "0", desc: hintRandomGroupSize, category: "Group Scaling")]
	bool m_bRandomGroupSize;

	/* Courtesy of JeTZz */
	[Attribute("1", UIWidgets.EditBox, "Multiply the group size by this number. (1 = single group, 2 = single group * 2, etc)", "1 inf 1", category: "Group Scaling")]
	protected int m_iGroupMultiplier;
	/* Courtesy of JeTZz */

	//--- PapaReap >>>
	[Attribute("0", UIWidgets.EditBox, "Maximum time the group will respawn. If greater than 'Respawn Period' it will pick a random time between the two. (seconds, 0 = no respawn)", "0 inf 1")]
	protected int m_iRespawnPeriodMaximum;
	
	/* Gramps added */
	[Attribute("0", UIWidgets.EditBox, "How many waves will the group respawn. (0 = no respawn, -1 = infinite respawn)", "-1 inf 1")]
	protected int m_iRespawnWaves;
	
	//--- PapaReap >>>
	[Attribute("0", UIWidgets.EditBox, "Maximum amount of waves the group respawn. If greater than 'Respawn Waves' it will pick a random amount between the two. (0 = no respawn, -1 = infinite respawn)", "-1 inf 1")]
	protected int m_iRespawnWavesMaximum;

	//! PR SPAWN PATROL: TELEPORT TO POSITION
	[Attribute("", desc: "Object name to respawn on. If more than one is used, a random spot will be used. Can be inside a collection also.")]
	ref array<string> m_aRespawnPosition;
	
	[Attribute("0")]
	protected bool m_bIsVehicleGroup;
	/* Gramps added */

	protected int m_iGroupSize;
	protected ref array<IEntity> m_aPlayers = {};

	bool IsVehicleGroup()
	{
		return m_bIsVehicleGroup;
	}

	int GetGroupMultiplier()
	{
		return m_iGroupMultiplier;
	}

	void SetGroupSize(int size)
	{
		m_iGroupSize = size;
	}

	//------------------------------------------------------------------------------------------------
	//--- Gramps
	protected void RefreshPlayerList()
	{
		m_aPlayers.Clear();
		array<int> playerIds = {};
		PlayerManager pc = GetGame().GetPlayerManager();
		int playersCount = pc.GetPlayers(playerIds);
		foreach (int playerId : playerIds)
		{
			IEntity player = pc.GetPlayerControlledEntity(playerId);
			if (!player)
				continue;
			CharacterControllerComponent comp = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
			if (!comp || comp.IsDead())
				continue;
			m_aPlayers.Insert(player);
		}
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (RefreshPlayerList) m_aPlayers.Count(): %1", m_aPlayers.Count()), LogLevel.WARNING);
	}

    override protected void EOnInit(IEntity owner)
    {
		super.EOnInit(owner);

		if (SCR_Global.IsEditMode())
			UnitCalculator();
    }

	protected void UnitCalculator()
	{
		int iUnitsCalc = Math.Map(m_iSimulateGamePlayerCountForScaling, 1, Math.Max(GetMaxPlayersForGameMode(), 2), m_iMinUnitsInGroup, m_iUnitScaleForOptimumPlayerCount);
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (UnitCalculator)"), LogLevel.WARNING);

		if (m_iMaxUnitsInGroup == -1)
			m_iMaxUnitsInGroup = iUnitsCalc;

		if (iUnitsCalc > m_iMaxUnitsInGroup)
			Print(string.Format("---- Maximum Unit Count has been reached [%1]. Raise 'Max Units In Group' to allow estimated spawn of [%2]! ----", m_iMaxUnitsInGroup, iUnitsCalc), LogLevel.ERROR);

		Print(string.Format("---- Simulated 'Unit Spawn Count' with [%1] players in the game = [%2] units spawned ----", m_iSimulateGamePlayerCountForScaling, (Math.Min(iUnitsCalc,m_iMaxUnitsInGroup))), LogLevel.WARNING);
	}
	
	//SCR_AIGroup m_Group;
	//--- PapaReap <<<

	//------------------------------------------------------------------------------------------------
	//--- PapaReap from SCR_ScenarioFrameworkLayerBase.c
	int GetPlayersCount(FactionKey factionName = "")
	{
		if (factionName.IsEmpty())
			//Print(("[PR] GetPlayersCount, factionName.IsEmpty(): " + GetGame().GetPlayerManager().GetPlayerCount()), LogLevel.NORMAL);
			return GetGame().GetPlayerManager().GetPlayerCount();

		FactionManager factionManager = GetGame().GetFactionManager();
		if (!factionManager)
			Print(("[PR] GetPlayersCount, factionManager: " + factionManager), LogLevel.ERROR);
			return -1;

		int iCnt = 0;
		array<int> aPlayerIDs = {};
		SCR_PlayerController playerController;
		GetGame().GetPlayerManager().GetPlayers(aPlayerIDs);
		foreach (int iPlayerID : aPlayerIDs)
		{
			playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(iPlayerID));
			if (!playerController)
				continue;

			if (playerController.GetLocalControlledEntityFaction() == factionManager.GetFactionByKey(factionName))
				iCnt++;
		}

		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (GetPlayersCount) iCnt: %1", iCnt), LogLevel.WARNING);
		return iCnt;
	}

	//------------------------------------------------------------------------------------------------
	int GetMaxPlayersForGameMode(FactionKey factionName = "")
	{
		//TODO: separate players by faction (attackers / defenders)
		SCR_MissionHeader header = SCR_MissionHeader.Cast(GetGame().GetMissionHeader());

	//	SCR_MissionHeaderCampaign myData = SCR_MissionHeaderCampaign.Cast(MissionHeader.ReadMissionHeader("Missions/NQDYCampaignGogland.conf"))\n
		if (!header || m_bIgnoreMissionHeader)
			return m_iOptimumPlayerCountForScaling;

		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (GetMaxPlayersForGameMode) Mission Header: %1", header), LogLevel.NORMAL);
		return header.m_iPlayerCount;
	}

	//--- PapaReap >>>
	//------------------------------------------------------------------------------------------------
	//! sets m_bFirstRun
	protected bool m_bFirstRun = 1;

	void SetFirstRun(bool firstRun)
	{
		m_bFirstRun = firstRun;
	}

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
	//! Get all children for waypoint collection
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
	//! sets m_vSpawnPosition
	protected vector m_vSpawnPosition;
	void SetSpawnPosition(vector spawnPosition)
	{
		m_vSpawnPosition = spawnPosition;
	}

	vector GetSpawnPosition()
	{
		return m_vSpawnPosition;
	}
	
	//--- PapaReap <<<
	
	//------------------------------------------------------------------------------------------------
	override void SpawnPatrol()
	{
		SCR_FactionAffiliationComponent comp = SCR_FactionAffiliationComponent.Cast(GetOwner().FindComponent(SCR_FactionAffiliationComponent));

		if (!comp)
			return;

		if (GetSpawnPosition())
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) GetSpawnPosition(): %1", GetSpawnPosition()), LogLevel.WARNING);
		
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_iMembersAlive: %1", m_iMembersAlive), LogLevel.WARNING);
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_iRespawnWaves: %1", m_iRespawnWaves), LogLevel.WARNING);
		if (m_iRespawnPeriodMaximum > m_iRespawnPeriod && m_iMembersAlive == -1)
		{
			m_iRespawnPeriod = Math.RandomIntInclusive(m_iRespawnPeriod, m_iRespawnPeriodMaximum);
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_iRespawnPeriod2: %1", m_iRespawnPeriod), LogLevel.WARNING);
		}

		// set respawn position		
		if (!GetFirstRun() && m_aRespawnPosition.Count() > 0 && m_iMembersAlive == -1)
		{
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) GetFirstRun()2: %1", GetFirstRun()), LogLevel.WARNING);
			array<IEntity> spawnArray = {};
			int spawnPositionCount = m_aRespawnPosition.Count();
			int _i = 0;
			while (spawnPositionCount > _i)
			{
				array<IEntity> spawnpointArray = {};
				IEntity spHolder = GetGame().GetWorld().FindEntityByName(m_aRespawnPosition.Get(_i));

				GetAllChildren(spHolder, spawnpointArray);
				foreach (IEntity x : spawnpointArray)
				{
					spawnArray.Insert(x);
				}
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) spawnpointArray: %1", spawnpointArray), LogLevel.WARNING);
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) spawnArray: %1", spawnArray), LogLevel.WARNING);
				_i++;
			}

			array<IEntity> filteredSpawnArray = {};
			if (spawnArray.Count() > 0)
			{
				foreach (IEntity x : spawnArray)
				{
					IEntity player = GetClosestPlayerEntity(x, 300);
					if (!player)
					{
						filteredSpawnArray.Insert(x);
					} else
						Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) Spawn position too close to player for spawn! Checking other positions. : %1", x), LogLevel.WARNING);
				}
			}

			if (filteredSpawnArray.Count() > 0)
			{
				int randomIndex = filteredSpawnArray.GetRandomIndex();
				IEntity spawnObject = filteredSpawnArray.Get(randomIndex);
				SetSpawnPosition(spawnObject.GetOrigin()); //= spawnObject.GetOrigin();
			}
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) filteredSpawnArray: %1", filteredSpawnArray), LogLevel.WARNING);
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_aRespawnPosition: %1", m_aRespawnPosition), LogLevel.WARNING);
			
			if (GetSpawnPosition())
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) spawnPosition2(): %1", GetSpawnPosition()), LogLevel.WARNING);
		}
		
		
		if (GetFirstRun())
		{
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) GetFirstRun()1: %1", GetFirstRun()), LogLevel.WARNING);
			SetFirstRun(false);
		
			if (m_iRespawnWavesMaximum > m_iRespawnWaves)
			{
				m_iRespawnWaves = Math.RandomIntInclusive(m_iRespawnWaves, m_iRespawnWavesMaximum);
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_iRespawnWaves2: %1", m_iRespawnWaves), LogLevel.WARNING);
			}
		}
		
Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) spawnPosition3(): %1", GetSpawnPosition()), LogLevel.WARNING);
		SCR_Faction faction = SCR_Faction.Cast(comp.GetAffiliatedFaction());

		if (!faction)
			faction = SCR_Faction.Cast(comp.GetDefaultAffiliatedFaction());

		if (faction != m_SavedFaction || m_iRespawnPeriod > 0)
			Update(faction);

		m_bSpawned = true;
		m_bActive = true;

		if (m_sPrefab.IsEmpty())
			return;

		Resource prefab = Resource.Load(m_sPrefab);
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_sPrefab: %1", m_sPrefab), LogLevel.WARNING);

		if (!prefab || !prefab.IsValid())
			return;

		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = GetOwner().GetOrigin();
		Math.Randomize(-1);

		/* Gramps added - Counter attack randomizer >> */
		if (m_Waypoint && m_iRespawnPeriod > 0 && m_iRespawnWaves > 0)
		{
			RefreshPlayerList();

			// Get a random spawn point for counter attack
			float dist = vector.Distance(GetOwner().GetOrigin(), m_Waypoint.GetOrigin());
			vector waypointVec = m_Waypoint.GetOrigin();
			vector randomDir = Vector(Math.RandomFloat(0, 360), 0, 0).AnglesToVector();
			vector randomVec = waypointVec + dist * randomDir;
			bool spawnEmpty = SCR_WorldTools.FindEmptyTerrainPosition(randomVec, randomVec, 5, 5);
			randomVec[1] = this.GetOwner().GetWorld().GetSurfaceY(randomVec[0], randomVec[2]);

			// Define if any player is too close to spawn counter attack group
			bool playersVeryNear;
			int distance;
			foreach (IEntity player : m_aPlayers)
			{
				if (!player)
					continue;
				distance = vector.DistanceSq(player.GetOrigin(), randomVec);
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) counter attack distance: %1", distance), LogLevel.WARNING);
				if (distance > 55000)
					continue;
				playersVeryNear = true;
				break;
			}

			// Define if selected random spawn point is in the water
			BaseWorld world = this.GetOwner().GetWorld();
			vector outWaterSurfacePoint;
			EWaterSurfaceType outType;
			vector transformWS[4];
			vector obbExtents;
			bool isUnderwater = ChimeraWorldUtils.TryGetWaterSurface(world, randomVec, outWaterSurfacePoint, outType, transformWS, obbExtents);

			// If random spawn point is in the water or players are too near, pick a new point
			int limiter = 0;	// Fix for infinite loops
			while (isUnderwater && limiter < 30 || playersVeryNear && limiter < 30 || !spawnEmpty && limiter < 30)
			{
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) limiter check: %1", limiter), LogLevel.WARNING);
				randomDir = Vector(Math.RandomFloat(0, 360), 0, 0).AnglesToVector();
				randomVec = waypointVec + dist * randomDir;
				spawnEmpty = SCR_WorldTools.FindEmptyTerrainPosition(randomVec, randomVec, 5, 5, 5);
				randomVec[1] = this.GetOwner().GetWorld().GetSurfaceY(randomVec[0], randomVec[2]);
				isUnderwater = ChimeraWorldUtils.TryGetWaterSurface(world, randomVec, outWaterSurfacePoint, outType, transformWS, obbExtents);
				RefreshPlayerList();
				playersVeryNear = false;
				foreach (IEntity player : m_aPlayers)
				{
					if (!player)
						continue;
					distance = vector.DistanceSq(player.GetOrigin(), randomVec);
					if (distance > 55000)
						continue;
					playersVeryNear = true;
					break;
				}
				limiter++;
			}
			if (limiter >= 30 && (m_aRespawnPosition.Count() == 0)/*PapaReap*/)
			{
				Print(string.Format("[CPR] Randomizer limit reached, counter attack skipped!"), LogLevel.WARNING);
				return;
			}
			// Once all checks are passed, set random spawn point
			if (!limiter >= 30)
				params.Transform[3] = randomVec;
		}
		/* << Gramps added */

		if (m_iRespawnPeriod == 0 && m_Waypoint && Math.RandomFloat01() >= 0.5)
		{
			AIWaypointCycle cycleWP = AIWaypointCycle.Cast(m_Waypoint);

			if (cycleWP)
			{
				array<AIWaypoint> waypoints = {};
				cycleWP.GetWaypoints(waypoints);
				//params.Transform[3] = waypoints.GetRandomElement().GetOrigin();
				params.Transform[3] = waypoints[0].GetOrigin();	// < Gramps edit //waypoints.GetRandomElement().GetOrigin();
			}
		}

		m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(prefab, null, params));

		if (!m_Group)
			return;

		if (!m_Group.GetSpawnImmediately())
		{
			if (m_iMembersAlive > 0)
			{
				m_Group.SetMaxUnitsToSpawn(m_iMembersAlive);
/* Gramps added >>> */
				m_Group.SetSpawnPoint(this);

				if (this.IsVehicleGroup())
				{
//					Print("APSPC:: Is Vic: TRUE!  Group size: " + m_iGroupSize);
					m_Group.SetIsVehiclePatrol(IsVehicleGroup());
//					m_Group.SetGroupSize(m_iGroupSize);
//					m_Group.SetMaxUnitsToSpawn(m_iGroupSize);
//					m_Group.SetNumberOfMembersToSpawn(m_iGroupSize);
				}
//				else
//				{
//					Print("APSPC:: Is Vic: FALSE!  Group size: " + m_iGroupSize);
//
//				}
			}// /* <<< Gramps added */m_Group.SetMaxUnitsToSpawn(m_iMembersAlive);

			//--- PapaReap >>>
			if ((m_bRandomGroupSize || m_bBalanceOnPlayersCount) && m_iMembersAlive <= 0)
			{
				//RefreshPlayerList(); just for testing

				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) m_iMembersAlive: %1", m_iMembersAlive), LogLevel.NORMAL);
				int prefabUnitCount = m_Group.m_aUnitPrefabSlots.Count();
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) prefabUnitCount: %1", prefabUnitCount), LogLevel.NORMAL);
				int iUnitsToSpawn = prefabUnitCount;
				if (m_iMinUnitsInGroup <= 0)
					m_iMinUnitsInGroup = 1;

				if (m_bBalanceOnPlayersCount)
				{
					if (m_iUnitScaleForOptimumPlayerCount < m_iMinUnitsInGroup)
						m_iUnitScaleForOptimumPlayerCount = m_iMinUnitsInGroup;
	
					iUnitsToSpawn = Math.Map(GetPlayersCount(), 1, Math.Max(GetMaxPlayersForGameMode(), 2), m_iMinUnitsInGroup, m_iUnitScaleForOptimumPlayerCount);
					Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) GetPlayersCount(): %1", GetPlayersCount()), LogLevel.NORMAL);
						//int iUnitsToSpawn = Math.Map(GetPlayersCount(), 1, GetMaxPlayersForGameMode(), Math.RandomInt(1, 3), m_iMembersAlive);
				}

				if (m_bRandomGroupSize)
				{
					if (m_iMinUnitsInGroup <= 0)
						m_iMinUnitsInGroup = 1;
					if (m_iMaxUnitsInGroup < m_iMinUnitsInGroup)
						m_iMaxUnitsInGroup = m_iMinUnitsInGroup;

					if (m_iMaxUnitsInGroup >= m_iMinUnitsInGroup)
						iUnitsToSpawn = Math.RandomIntInclusive(m_iMinUnitsInGroup, m_iMaxUnitsInGroup);
				}

				if (iUnitsToSpawn < 1)
					iUnitsToSpawn = 1;

				if (m_iMaxUnitsInGroup > 0)
				{
					if (iUnitsToSpawn > m_iMaxUnitsInGroup)
						iUnitsToSpawn = m_iMaxUnitsInGroup;
				}

				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) iUnitsToSpawn: %1", iUnitsToSpawn), LogLevel.NORMAL);

				if (prefabUnitCount >= iUnitsToSpawn)
				{
					m_Group.SetMaxUnitsToSpawn(iUnitsToSpawn);

					/* Gramps added >>> edit by PapaReap*/
					if (m_iGroupMultiplier > 1)
						for (int k = m_iGroupMultiplier; k > 0; k--)
						{
							m_Group.SpawnUnits();
						}
					else /* <<< Gramps added */
						m_Group.SpawnUnits();
				}
				else
				{
					while (iUnitsToSpawn > 0)
					{
						m_Group.SetMaxUnitsToSpawn(Math.Min(prefabUnitCount, iUnitsToSpawn));

						/* Gramps added >>> edit by PapaReap*/
						if (m_iGroupMultiplier > 1)
							for (int k = m_iGroupMultiplier; k > 0; k--)
							{
								m_Group.SpawnUnits();
							}
						else /* <<< Gramps added */
							m_Group.SpawnUnits();

						iUnitsToSpawn = iUnitsToSpawn - prefabUnitCount;
					}
				}
			}
			else
			{
				/* Gramps added >>> edit by PapaReap*/
				if (m_iGroupMultiplier > 1)
					for (int k = m_iGroupMultiplier; k > 0; k--)
					{
						m_Group.SpawnUnits();
					}
				else /* <<< Gramps added */
					m_Group.SpawnUnits();
			}
			//--- PapaReap <<<
		}

		if (GetSpawnPosition())
		{
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (SpawnPatrol) setting group position GetSpawnPosition(): %1", GetSpawnPosition()), LogLevel.WARNING);
			m_Group.SetOrigin(GetSpawnPosition());
		}
		//SetSpawnPosition(m_Group.GetOrigin());
		
		m_Group.AddWaypoint(m_Waypoint);

		if ((m_iRespawnPeriod != 0 /* Gramps added >>> */&& m_iRespawnWaves != 0) || (m_iRespawnPeriod != 0 && m_iRespawnWaves == -1)/* <<< Gramps added */)
			m_Group.GetOnAgentRemoved().Insert(OnAgentRemoved);
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	override void DeactivateGroup()
	{
		if (m_Group)
		{
			SetSpawnPosition(m_Group.GetOrigin());
			Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (DeactivateGroup) spawnPosition(): %1", GetSpawnPosition()), LogLevel.WARNING);
			m_bActive = false;
			m_Group.DeactivateAllMembers();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnAgentRemoved()
	{
		if (!m_Group || m_Group.GetAgentsCount() > 0)
			return;

		ChimeraWorld world = GetOwner().GetWorld();
		if (m_fRespawnTimestamp.GreaterEqual(world.GetServerTimestamp()))
			return;

		// Set up respawn timestamp, convert s to ms, reset original group size
		m_fRespawnTimestamp = world.GetServerTimestamp().PlusSeconds(m_iRespawnPeriod);
		/* Gramps added >>> */
		if (m_iRespawnWaves != -1)
			m_iRespawnWaves--;
		/* <<< Gramps added */
		m_iMembersAlive = -1;
		m_bSpawned = false;
		
		vector clearPos;
		SetSpawnPosition(clearPos);
		Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] (OnAgentRemoved) GetSpawnPosition()2: %1", GetSpawnPosition()), LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	override void Update(SCR_Faction faction)
	{
		if (!m_Waypoint)
			PrepareWaypoints();

		m_SavedFaction = faction;

		if (!faction)
			return;

		//--- PapaReap >>>
		if (m_eGroupType > 4 && faction.GetFactionKey() == "CIV")
		{
			m_sPrefab = GetCivGroup(m_eGroupType);
			return;
		}
		//--- PapaReap <<<

		SCR_EntityCatalog entityCatalog = faction.GetFactionEntityCatalogOfType(EEntityCatalogType.GROUP);

		if (!entityCatalog)
			return;

		array<SCR_EntityCatalogEntry> data = {};
		entityCatalog.GetEntityListWithData(SCR_EntityCatalogAmbientPatrolData, data);

		if (m_bPickRandomGroupType)
		{
			m_sPrefab = GetRandomPrefabByProbability(entityCatalog, data);
			return;
		}

		SCR_EntityCatalogEntry catalogEntry;
		SCR_EntityCatalogAmbientPatrolData patrolData;

		for (int i = 0, count = data.Count(); i < count; i++)
		{
			int catalogIndex = data[i].GetCatalogIndex();
			catalogEntry = entityCatalog.GetCatalogEntry(catalogIndex);

			if (!catalogEntry)
				continue;

			patrolData = SCR_EntityCatalogAmbientPatrolData.Cast(catalogEntry.GetEntityDataOfType(SCR_EntityCatalogAmbientPatrolData));

			if (!patrolData)
				continue;

			if (patrolData.GetGroupType() != m_eGroupType)
				continue;

			m_sPrefab = catalogEntry.GetPrefab();
			break;
		}
	}

	//--- PapaReap >>>
	protected string GetCivGroup(int groupType)
	{
		switch (groupType)
		{
			case 5: // "CIV_Businessmen_3" modded
			{
				return "{1FBE212D07953463}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_Businessmen_NotSpawned_3.et";
			};
			case 6: // "CIV_ConstructionWorkers_5" modded
			{
				return "{B4C4F7DF9438860B}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_ConstructionWorkers_NotSpawned_5.et";
			};
			case 7: // "CIV_Dockworkers_6" modded
			{
				return "{7232C826DC7C6C42}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_DockWorkers_NotSpawned_6.et";
			};
			case 8: // "CIV_CottonShirt_6" modded
			{
				return "{B43F216FB40EF8AF}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_GenericCiviliansCottonShirt_NotSpawned_6.et";
			};
			case 9: // "CIV_DenimJacket_2" modded
			{
				return "{9A585DADAA5737D4}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_GenericCiviliansDenimJacket_NotSpawned_2.et";
			};
			case 10: // "CIV_Turtleneck_3" modded
			{
				return "{7BA6F6CEA68B5066}Prefabs/Groups/CIV/AmbientPatrols/Group_CIV_GenericCiviliansTurtleneck_NotSpawned_3.et";
			};
		}
		return "<ERROR>";
	}
	//--- PapaReap <<<
}
