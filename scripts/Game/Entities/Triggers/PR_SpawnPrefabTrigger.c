/*
Author: PapaReap
*/

[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class PR_SpawnPrefabTriggerClass : PR_CoreTriggerClass
{
	// prefab properties here
}

class PR_SpawnPrefabTrigger : PR_CoreTrigger
{
	//------------------------------------------------------------------------------------------------
	void PR_SpawnPrefabTrigger(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
	
	//private string spawnPosition = ""; // make a default spawn position to this trigger if none found
	
	//! PR Spawn Prefab Details: Individual Prefabs to spawn.
	[Attribute(desc: "Individual Prefabs to spawn.  ", category: "PR Spawn Prefab Details")]
	protected ref array<ref PR_PrefabDetails> m_aPrefabDetails;

	//protected bool isTriggerActivated = false; //--- Flag to track activation status

//	override protected event void EOnInit(IEntity owner)
//	{
//		super.EOnInit(owner);

//		Deactivate();*/
//	}

	override protected event void OnActivate(IEntity ent)
	{
		super.OnActivate(ent);

		if (!Replication.IsServer())
			return;

		if (m_bIsTriggerActivated)
			return;

		m_bIsTriggerActivated = true; //--- Set the activation flag
		m_sLogMode = "(OnActivate)";
		
		//--- Delay after activation
		int delay = m_iDelayTimerMin * 1000;

		if (m_bUseRandomDelayTimer)
			delay = Math.RandomInt(m_iDelayTimerMin * 1000, m_iDelayTimerMax * 1000);

		Print(string.Format("[PR_SpawnPrefabTrigger] %1 : Trigger: %2 : delay: %3", m_sLogMode, m_sTriggerName, delay), LogLevel.WARNING);

		IEntity persistentObject;

		if (m_bUsePersistence)
		{
			if (m_bIsTestingMode)
				persistentObject = GetWorld().FindEntityByName(m_sPersistentObject);
			else
				persistentObject = GetGame().GetWorld().FindEntityByName(m_sPersistentObject);

			if (!persistentObject)
			{
				Print(string.Format("[PR_SpawnPrefabTrigger] %1 : No persistentObject: %2. Exiting!", m_sLogMode, m_sPersistentObject), LogLevel.WARNING);
				return;
			}
		}

		string specificPrefabName;
		array<string> objectLocationsToSpawnOn;
		bool useRandomPositions;
		float randomPercentage;

		if (m_aPrefabDetails.Count() > 0)
		{
			foreach (PR_PrefabDetails prefabDetails : m_aPrefabDetails)
			{
				specificPrefabName = prefabDetails.m_aSpecificPrefabName;
				objectLocationsToSpawnOn = prefabDetails.m_aObjectLocationsToSpawnOn;
				useRandomPositions = prefabDetails.m_bUseRandomPositions;
				randomPercentage = prefabDetails.m_fRandomPercentage;

				array<string> combinedArray = {};

				if (objectLocationsToSpawnOn.Count() > 0)
				{
					int objectLocationsCount = 0;
					objectLocationsCount = objectLocationsToSpawnOn.Count();
					IEntity holder;
					array<string> objectLocationsCountArray = {};
					int _i = 0;
					while (objectLocationsCount > _i)
					{
						holder = GetGame().GetWorld().FindEntityByName(objectLocationsToSpawnOn.Get(_i));
						if (holder)
							GetAllChildrenNames(holder, objectLocationsCountArray, m_bDebugLogs);

						_i++;
					}

					combinedArray = objectLocationsCountArray;
					if (useRandomPositions)
					{
						if (combinedArray.Count() > 1)
						{
							int arrayCount = Math.Round(combinedArray.Count() * (randomPercentage * 0.01));
							Print(string.Format("[PR_SpawnPrefabTrigger] %1 : arrayCount: %2", m_sLogMode, arrayCount), LogLevel.WARNING);
							array<string> tempArray = {};
							int randomIndex;
							_i = 0;
							while (arrayCount > _i)
							{
								randomIndex = combinedArray.GetRandomIndex();
								tempArray.Insert(combinedArray.Get(randomIndex));
								combinedArray.Remove(randomIndex);
								_i++;
							}
							combinedArray = tempArray;
						}
					}
				} else
					Print(string.Format("[PR_SpawnPrefabTrigger] %1 : Trigger: %2 : No positions to spawn on objectLocationsToSpawnOn: %3", m_sLogMode, m_sTriggerName, objectLocationsToSpawnOn), LogLevel.ERROR);

				foreach (string sObjectName : combinedArray)
				{
					IEntity object = GetWorld().FindEntityByName(sObjectName);
					if (!object)
					{
						Print(string.Format("[PR_SpawnPrefabTrigger] %1 : Trigger: %2 : No object for sObjectName: %3", m_sLogMode, m_sTriggerName, sObjectName), LogLevel.ERROR);
						continue;
					}

					vector spawnPos = object.GetOrigin();

					//--- Generate the resource
					Resource resource = GenerateAndValidateResource(specificPrefabName);

					if (!resource)
					{
						Print(string.Format("[PR_SpawnPrefabTrigger] %1 : Trigger: %2 : Unable to load resource for the specificPrefabName: %3", m_sLogMode, m_sTriggerName, specificPrefabName), LogLevel.ERROR);
						continue;
					}

					//--- Generate spawn parameters and spawn the prefab
					if (!m_bIsTestingMode)
					//{
						GetGame().GetCallqueue().CallLater(SpawnPrefab, delay, false, resource, spawnPos);
						//IEntity returnPrefab = SpawnPrefab(resource, spawnPos);
						//Print(string.Format("[PR_SpawnPrefabTrigger] %1 : Trigger: %2 : returnPrefab: %3", m_sLogMode, m_sTriggerName, returnPrefab), LogLevel.WARNING);
					//}
				}
			}
		}

		//--- Persistence cleanup
		PersistenceCleanup();

		Deactivate();
	}
/*
	//------------------------------------------------------------------------------------------------
	//! Generate Spawn Parameters
	protected EntitySpawnParams GenerateSpawnParameters(vector position)
	{
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = position; //--- Assign the position to those parameters
		return params;
	}

	//------------------------------------------------------------------------------------------------
	//! Generate and Validate Resources
	protected Resource GenerateAndValidateResource(string resourceToLoad)
	{
		Resource resource = Resource.Load(resourceToLoad);

		if (!resource.IsValid())
		{
			Print(("[PR GenerateAndValidateResource] Resource is invalid: " + resourceToLoad), LogLevel.ERROR);
			return null;
		}

		return resource;
	}

	//------------------------------------------------------------------------------------------------
	//! Spawn Prefab
	protected void SpawnPrefab(Resource resource, vector spawnPos)
	{
		StaticModelEntity spawnedPrefab = StaticModelEntity.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPos)));
	}
	*/
}

[BaseContainerProps()]
class PR_PrefabDetails
{
	//--- PR Spawn Prefab Details: Prefab type to spawn
	[Attribute("{44DCCCB56D42C43B}PrefabsEditable/Mines/E_Mine_TM62M.et", params: "et", desc: "If SPECIFIC_PREFAB_NAME is selected, fill the class name here.", category: "PR Spawn Prefab Details")]
	ResourceName m_aSpecificPrefabName;

	static string m_sHintObjectLocationsToSpawnOn = "Object location names to spawn prefab on. Can be a collection name. Can combine individual & collections.	";
	//--- PR Spawn Prefab Details: Object location names to spawn prefab on. Can be a collection name.
	[Attribute(desc: m_sHintObjectLocationsToSpawnOn/*"Object location names to spawn prefab on. Can be a collection name. Can combine individual & collections.  "*/, category: "PR Spawn Prefab Details")]
	ref array<string> m_aObjectLocationsToSpawnOn;

	//! PR Spawn Prefab Details: Pick random one position from all avaliable positions. Overrides 'Use Random Positions' below.
	[Attribute("false", UIWidgets.CheckBox,"Pick random one position from all avaliable positions. Overrides 'Use Random Positions' below.  ", category: "PR Spawn Prefab Details")]
	bool m_bPickRandomOne;
	
	//! PR Spawn Prefab Details: Pick random positions from all avaliable locations.
	[Attribute("false", UIWidgets.CheckBox,"Pick random positions from all avaliable locations.  ", category: "PR Spawn Prefab Details")]
	bool m_bUseRandomPositions;

	//! PR Spawn Prefab Details: Random percentage of locations to use.
	[Attribute(defvalue: "100", uiwidget: UIWidgets.EditBox, desc: "Random percentage of locations to use.", params: "0 100 1", category: "PR Spawn Prefab Details")]
	float m_fRandomPercentage;
}
