modded class SCR_DefenderSpawnerComponent : SCR_SlotServiceComponent
{
	//------------------------------------------------------------------------------------------------
	//! Manages automatic group spawning
	override protected void HandleGroup()
	{
		ChimeraWorld world = GetOwner().GetWorld();
		WorldTimestamp replicationTime = world.GetServerTimestamp();
		
		AIWorld aiWorld = GetGame().GetAIWorld();
		if (!aiWorld)
			return;
		
		aiWorld.SetAILimit(5000);
		
		//If another Ai would be over limit, stop requesting and prevent players from enabling it again.
		if ((aiWorld.GetCurrentAmountOfLimitedAIs() + 1) >= aiWorld.GetAILimit())
		{
			if (m_bEnableSpawning)
			{
				m_bEnableSpawning = false; //disable spawner
				Replication.BumpMe();
				
				m_GroupSpawningManager.SetIsAtAILimit(true); //block requesting action 
			}
			
			return;
		}
		
		SCR_EDefenderSpawnerState distanceState = GetPlayerDistanceState();
		
		if (m_bEnableSpawning && (distanceState == SCR_EDefenderSpawnerState.DEFENDERS_ENABLED))
		{
			// Add any stray or stuck units to group
			if (m_aUnitsOnMove)
			{
				int count = m_aUnitsOnMove.Count()-1;
				for (int i = count; i >= 0; i--)
				{
					if (!m_aUnitsOnMove[i].param2)
					{
						m_aUnitsOnMove.Remove(i);
						continue;
					}
					
					if (!m_aUnitsOnMove[i].param3.PlusSeconds(SPAWN_GROUP_JOIN_TIMEOUT).GreaterEqual(replicationTime))
						OnMoveFinished(m_aUnitsOnMove[i].param1);
				}
			}
			
			// Reinforce existing or create new defender group. Also handles respawning of despawned defenders
			if (m_iDespawnedGroupMembers > 0 || replicationTime.Greater(m_fNextRespawnTime))
			{
				m_fNextRespawnTime = replicationTime.PlusSeconds(m_fRespawnDelay);
				if (m_AIgroup && (m_AIgroup.m_aUnitPrefabSlots.Count() != m_AIgroup.GetAgentsCount()))
					ReinforceGroup();

				if (!m_AIgroup)
					SpawnGroup();
			}
			
			//Spawn queued units
			if (m_aRefillQueue && !m_aRefillQueue.IsEmpty())
			{
				if (m_iDespawnedGroupMembers > 0)
				{
					SpawnUnit(m_aRefillQueue[0], false);
					m_iDespawnedGroupMembers--;
				}
				else
				{
					SpawnUnit(m_aRefillQueue[0]);
				}
			}
		}
		
		//Despawn units that are too far away from players
		if (distanceState == SCR_EDefenderSpawnerState.DEFENDERS_DESPAWN)
		{
			if (m_AIgroup)
			{
				m_iDespawnedGroupMembers = m_AIgroup.GetAgentsCount();
				SCR_EntityHelper.DeleteEntityAndChildren(m_AIgroup);
			}
			
			//If leftover groups from previous owners remains alive, they will be despawned
			if (m_aPreviousGroups)
			{
				foreach (SCR_AIGroup aiGroup : m_aPreviousGroups)
				{
					SCR_EntityHelper.DeleteEntityAndChildren(aiGroup);
				}
				
				m_aPreviousGroups = null;
			}
			
			//Delete units from previous owners that were spawning in process of faction change. (edge case, but might happen)
			if (m_aOldUnits)
			{
				int index = m_aOldUnits.Count()-1;
				for (index; index >= 0; index--)
				{
					SCR_EntityHelper.DeleteEntityAndChildren(m_aOldUnits[index]);
				}
				
				m_aOldUnits = null;
			}
			
			//Despawn any units that are currently walking to RP
			if (m_aUnitsOnMove)
			{
				foreach (Tuple3<SCR_AIActionBase, AIAgent, WorldTimestamp> groupWaypoint : m_aUnitsOnMove)
				{
					SCR_EntityHelper.DeleteEntityAndChildren(groupWaypoint.param2.GetControlledEntity());
					m_iDespawnedGroupMembers++;
				}
				
				m_aUnitsOnMove = null;
			}
		}
	}
}
