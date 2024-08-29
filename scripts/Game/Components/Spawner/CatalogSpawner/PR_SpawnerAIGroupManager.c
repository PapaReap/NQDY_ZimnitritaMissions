modded class SCR_SpawnerAIGroupManagerComponent : SCR_BaseGameModeComponent
{
	//------------------------------------------------------------------------------------------------
	override protected void OnAgentsUpdated(AIAgent agent)
	{
		AIWorld aiWorld = GetGame().GetAIWorld();
		aiWorld.SetAILimit(5000);
		if (!aiWorld && m_bIsAtAILimit)
		{
			SetIsAtAILimit(false);
			return;
		}
		
		bool change = (aiWorld.GetCurrentAmountOfLimitedAIs() + 1) >= aiWorld.GetAILimit();
			
		//No need to replicate something that didn't change
		if (change == m_bIsAtAILimit)
			return;
		
		SetIsAtAILimit(change);
	}
}
