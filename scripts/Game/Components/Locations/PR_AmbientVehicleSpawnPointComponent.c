modded class SCR_AmbientVehicleSpawnPointComponent : ScriptComponent
{
	//--- PapaReap >>>
	[Attribute("0", desc: "Overrides 'Entity Labels' from above if a prefab is choosen below using 'Specific Prefab Names'.")]
	protected bool m_bOverrideEntityLabels;

	[Attribute(params: "et", desc: "If SPECIFIC_PREFAB_NAME is selected, fill the class name here. More than one will be randomized.")]
	ref array<ResourceName> m_aSpecificPrefabNames;
	//--- PapaReap <<<
	
	[Attribute("0")]
	protected bool m_bIsVehiclePatrol;
	
	bool IsVehiclePatrol()
	{
		return m_bIsVehiclePatrol;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Update(SCR_Faction faction)
	{
		m_SavedFaction = faction;
		
		//--- PapaReap >>>
		if (m_bOverrideEntityLabels && m_aSpecificPrefabNames.Count() > 0)
		{
			m_sPrefab = (m_aSpecificPrefabNames.GetRandomElement());
			return;
		}
		//--- PapaReap <<<
		
		SCR_EntityCatalog entityCatalog;

		if (faction)
		{
			entityCatalog = faction.GetFactionEntityCatalogOfType(EEntityCatalogType.VEHICLE);
		}
		else
		{
			SCR_EntityCatalogManagerComponent comp = SCR_EntityCatalogManagerComponent.GetInstance();

			if (!comp)
				return;

			entityCatalog = comp.GetEntityCatalogOfType(EEntityCatalogType.VEHICLE);
		}

		if (!entityCatalog)
			return;

		array<SCR_EntityCatalogEntry> data = {};
		entityCatalog.GetFullFilteredEntityListWithLabels(data, m_aIncludedEditableEntityLabels, m_aExcludedEditableEntityLabels, m_bRequireAllIncludedLabels);

		if (data.IsEmpty())
			return;

		Math.Randomize(-1);
		m_sPrefab = (data.GetRandomElement().GetPrefab());
	}
}
