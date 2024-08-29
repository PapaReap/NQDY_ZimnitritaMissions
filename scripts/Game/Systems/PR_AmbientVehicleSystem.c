modded class SCR_AmbientVehicleSystem : GameSystem
{
	//------------------------------------------------------------------------------------------------
	override protected void ProcessSpawnpoint(int spawnpointIndex)
	{
		SCR_AmbientVehicleSpawnPointComponent spawnpoint = m_aSpawnpoints[spawnpointIndex];

		if (!spawnpoint || spawnpoint.GetIsDepleted())
			return;

		Vehicle spawnedVeh = spawnpoint.GetSpawnedVehicle();
		ChimeraWorld world = GetWorld();
		WorldTimestamp currentTime = world.GetServerTimestamp();
		int respawnPeriod = spawnpoint.GetRespawnPeriod();
		
		// Non-respawning vehicle has beed deleted by other means
		// Ignore this spawnpoint from now on
		if (!spawnedVeh && spawnpoint.GetIsSpawnProcessed() && respawnPeriod <= 0)
		{
			spawnpoint.SetIsDepleted(true);
			return;
		}

		if (!spawnedVeh)
		{
			WorldTimestamp respawnTimestamp = spawnpoint.GetRespawnTimestamp();

			// Respawn timer is ticking
			if (respawnTimestamp.Greater(currentTime))
				return;

			if (spawnpoint.GetIsFirstSpawnDone())
			{
				// Vehicle has been deleted, setup respawn timer if enabled
				if (respawnTimestamp == 0 && respawnPeriod > 0)
				{
					spawnpoint.SetRespawnTimestamp(currentTime.PlusSeconds(respawnPeriod));
					return;
				}
			}
		}

		vector location = spawnpoint.GetOwner().GetOrigin();

		// Only handle vehicles which are still on their spawnpoints
		if (spawnedVeh && vector.DistanceXZ(spawnedVeh.GetOrigin(), location) > PARKED_THRESHOLD)
		{
			// Non-respawning spawnpoints get depleted once the vehicle leaves the spawnpoint
			if (spawnpoint.GetRespawnPeriod() <= 0)
				spawnpoint.SetIsDepleted(true);

			return;
		}

		bool playersNear = false;
		bool playersFar = true;
		int distance;

		// Define if any player is close enough to spawn or if all players are far enough to despawn
		foreach (IEntity player : m_aPlayers)
		{
			if (!player)
				continue;

			distance = vector.DistanceSq(player.GetOrigin(), location);

			if (distance < m_iDespawnDistanceSq)
			{
				playersFar = false;

				if (distance < m_iSpawnDistanceSq)
				{
					playersNear = true;
					break;
				}
			}
		}

		if (!spawnedVeh && playersNear /* Gramps added >>*/	|| !spawnedVeh && spawnpoint.IsVehiclePatrol()/*<< Gramps added */	)
		//if (!spawnedVeh && playersNear)
		{
			Vehicle vehicle = spawnpoint.SpawnVehicle();

			if (vehicle && m_OnVehicleSpawned)
				m_OnVehicleSpawned.Invoke(spawnpoint, vehicle);
			
			return;
		}

		// Delay is used so dying players don't see the despawn happen
		if (spawnedVeh && playersFar)
		{
			WorldTimestamp despawnT = spawnpoint.GetDespawnTimer();

			if (despawnT == 0)
				spawnpoint.SetDespawnTimer(currentTime.PlusMilliseconds(DESPAWN_TIMEOUT));
			else if (currentTime.Greater(despawnT) /* Gramps added >>*/	&& !spawnpoint.IsVehiclePatrol()/*<< Gramps added */	)
			//else if (currentTime.Greater(despawnT))

				spawnpoint.DespawnVehicle();
		}
		else
		{
			spawnpoint.SetDespawnTimer(null);
		}
	}
}
