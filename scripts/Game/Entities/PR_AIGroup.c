modded class SCR_AIGroup : ChimeraAIGroup
{
	protected int groupSize;
	protected static bool isVehiclePatrol = false;
	protected static SCR_AmbientPatrolSpawnPointComponent spawnpoint;
	protected static vector counterPoint;
	
	void SetGroupVehicles(array<string> vehicles)
	{
		m_aStaticVehicles = vehicles;
		AddVehiclesStatic(m_aStaticVehicles);
	}
	
	vector GetCounterPoint()
	{
		return counterPoint;
	}
	
	void SetCounterPoint(vector point)
	{
		counterPoint = point;
	}
	
	void SetSpawnPoint(SCR_AmbientPatrolSpawnPointComponent point)
	{
		spawnpoint = point;
	}
	
	SCR_AmbientPatrolSpawnPointComponent GetSpawnpoint()
	{
		return spawnpoint;
	}
	
	void SetGroupSize(int size)
	{
		groupSize = size;
	}
	
	void SetIsVehiclePatrol(bool value)
	{
		isVehiclePatrol = value;
	}
	
	bool IsVehiclePatrol()
	{
		return isVehiclePatrol;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void CreateUnitEntities(bool editMode, array<ResourceName> entityResourceNames)
	{
		if (!GetGame().GetAIWorld())
		{
			Print(string.Format("Cannot spawn team members of group %1, AIWorld is missing in the world!", this), LogLevel.WARNING);
			return;
		}
		
#ifdef WORKBENCH
		if (!editMode)
		{
			//--- Are AI components valid?
			AIFormationComponent AIFormation = AIFormationComponent.Cast(FindComponent(AIFormationComponent));
			
			if (!AIFormation)
				Print(string.Format("Group %1 does not have AIFormationComponent! Team members will not be spawned.", this), LogLevel.WARNING);
			else
			{
				AIFormationDefinition formationDefinition = AIFormation.GetFormation();
				if (!formationDefinition)
					Print(string.Format("Formation of group %1 not found in SCR_AIWorld! Team members will not be spawned.", this), LogLevel.WARNING);
			}
		}
#endif
		//--- Apply global override
		bool snapToTerrain = m_bSnapToTerrain;
		if (s_bIgnoreSnapToTerrain)
		{
			snapToTerrain = false;
			s_bIgnoreSnapToTerrain = false;
		}
		if (Replication.IsClient())
			return;
		
		//--- We are in WB, prepare array so previews can be deleted later
		if (editMode && !m_aSceneGroupUnitInstances)
			m_aSceneGroupUnitInstances = new array<IEntity>;
		
		m_iNumOfMembersToSpawn = Math.Min(entityResourceNames.Count(), m_iMaxUnitsToSpawn);
		
		//--- Create group members
		for (int i = m_iNumOfMembersToSpawn-1; i >= 0; i--)
		{
			// Spawn group across multiple frames
			SCR_AIGroup_DelayedSpawn delaySpawn = new SCR_AIGroup_DelayedSpawn();
			delaySpawn.snapToTerrain	= snapToTerrain;
			delaySpawn.index			= i;
			delaySpawn.resourceName		= entityResourceNames[i];
			delaySpawn.editMode			= editMode;
			
			m_delayedSpawnList.Insert(delaySpawn);
		}

		if (editMode)
		{
			//--- Edit mode has no game world, spawn immediately
			SpawnAllImmediately();
		}
		else
		{
			//--- Enable the frame event and frames when paused
			BeginDelayedSpawn();
		}

		//--- Call group init if it cannot be called by the last spawned entity
		if (m_iNumOfMembersToSpawn == 0)
			Event_OnInit.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	/*
	Returns false when member couldn't be spawned but we should try again
	True otherwise
	*/
	override bool SpawnDelayedGroupMember(int spawnIndex)
	{
		return SpawnGroupMember(
			m_delayedSpawnList.Get(spawnIndex).snapToTerrain,
			m_delayedSpawnList.Get(spawnIndex).index,
			m_delayedSpawnList.Get(spawnIndex).resourceName,
			m_delayedSpawnList.Get(spawnIndex).editMode,
			spawnIndex == -1 // isLast - Gramps changed to -1 from 0
		);
	}
};
