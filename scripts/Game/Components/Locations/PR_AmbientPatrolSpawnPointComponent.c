modded class SCR_AmbientPatrolSpawnPointComponent : ScriptComponent
{
	/* Gramps added */
	[Attribute("0", UIWidgets.EditBox, "How many waves will the group respawn. (0 = no respawn, -1 = infinite respawn)", "-1 inf 1")]
	protected int m_iRespawnWaves;
	/* Gramps added */

	//--- PapaReap >>>
	[Attribute(defvalue: "0", desc: "Randomize group size based on a random min and max values below.  ", category: "Group Scaling")]
	bool m_bRandomGroupSize;

	[Attribute(defvalue: "1", desc: "Minimum amount of AIs in the group.  ", category: "Group Scaling")]
	int m_iMinUnitsInGroup;

	[Attribute(defvalue: "-1", desc: "Maximum amount of AIs in the group. -1 has no effect, will use min value. If value is higher than actual group size, units will be added to group until max amount reached.  ", category: "Group Scaling")]
	int m_iMaxUnitsInGroup;

	/* Courtesy of JeTZz */
	[Attribute("1", UIWidgets.EditBox, "Multiply the group size by this number. (1 = single group, 2 = single group * 2, etc)", "1 inf 1", category: "Group Scaling")]
	protected int m_iGroupMultiplier;
	/* Courtesy of JeTZz */

	SCR_AIGroup m_Group;
	//--- PapaReap <<<

	//------------------------------------------------------------------------------------------------
	override void SpawnPatrol()
	{
		SCR_FactionAffiliationComponent comp = SCR_FactionAffiliationComponent.Cast(GetOwner().FindComponent(SCR_FactionAffiliationComponent));

		if (!comp)
			return;

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

		if (!prefab || !prefab.IsValid())
			return;

		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = GetOwner().GetOrigin();
		Math.Randomize(-1);

		if (m_iRespawnPeriod == 0 && m_Waypoint && Math.RandomFloat01() >= 0.5)
		{
			AIWaypointCycle cycleWP = AIWaypointCycle.Cast(m_Waypoint);

			if (cycleWP)
			{
				array<AIWaypoint> waypoints = {};
				cycleWP.GetWaypoints(waypoints);
				params.Transform[3] = waypoints.GetRandomElement().GetOrigin();
			}
		}

		m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(prefab, null, params));

		if (!m_Group)
			return;

		if (!m_Group.GetSpawnImmediately())
		{
			if (m_iMembersAlive > 0)
				m_Group.SetMaxUnitsToSpawn(m_iMembersAlive);

			//--- PapaReap >>>
			if (m_bRandomGroupSize)
			{
				int prefabUnitCount = m_Group.m_aUnitPrefabSlots.Count();
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] SpawnPatrol/m_bRandomGroupSize: numberToSpawn: %1", prefabUnitCount), LogLevel.NORMAL);

				int iUnitsToSpawn = prefabUnitCount;
				if (iUnitsToSpawn > m_iMinUnitsInGroup)
					iUnitsToSpawn = m_iMinUnitsInGroup;
		
				if (m_iMaxUnitsInGroup > m_iMinUnitsInGroup)
					iUnitsToSpawn = Math.RandomIntInclusive(m_iMinUnitsInGroup, m_iMaxUnitsInGroup);
		
				if (iUnitsToSpawn < 1)
					iUnitsToSpawn = 1;
				
				Print(string.Format("[PR_AmbientPatrolSpawnPointComponent] SpawnPatrol/m_bRandomGroupSize: iUnitsToSpawn1: %1", iUnitsToSpawn), LogLevel.NORMAL);

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
				} else
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

						iUnitsToSpawn = iUnitsToSpawn - prefabUnitCount; // 4
					}
				}
			} else
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

		m_Group.AddWaypoint(m_Waypoint);

		if ((m_iRespawnPeriod != 0 /* Gramps added >>> */&& m_iRespawnWaves != 0) || (m_iRespawnPeriod != 0 && m_iRespawnWaves == -1)/* <<< Gramps added */)
			m_Group.GetOnAgentRemoved().Insert(OnAgentRemoved);
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
	}
}
