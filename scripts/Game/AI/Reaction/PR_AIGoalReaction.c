  /* ------------------------------------------------------------------------------------------------ /
 / CRX-EAI Modded Class ( #CRX-EAIModdedClass )                                                      /
/ ------------------------------------------------------------------------------------------------ */

[BaseContainerProps()]
modded class SCR_AIGoalReaction_GetOutVehicle : SCR_AIGoalReaction
{
	//------------------------------------------------------------------------------------------------
	override void PerformReaction(notnull SCR_AIUtilityComponent utility, SCR_AIMessageBase message)
	{
		super.PerformReaction(utility, message);
		
		auto behavior = utility.GetCurrentBehavior();
		
		CompartmentAccessComponent compartmentAccessComponent = CompartmentAccessComponent.Cast(utility.m_OwnerEntity.FindComponent(CompartmentAccessComponent));
		
		if (compartmentAccessComponent && compartmentAccessComponent.IsInCompartment())
			WrapBehaviorOutsideOfVehicle(utility, behavior, compartmentAccessComponent);
	}
	
	//------------------------------------------------------------------------------------------------
	void WrapBehaviorOutsideOfVehicle(SCR_AIUtilityComponent utility, SCR_AIActionBase action, CompartmentAccessComponent compartmentAccessComponent)
	{
		bool unitStateTurret;
		
		SCR_AIActivityBase relatedActivity;
		
		AIAgent agent = utility.GetAIAgent();
		
		if (agent)
		{
			AIGroup parentGroup = agent.GetParentGroup();
			
			if (parentGroup)
			{
				array<AIAgent> agents = {};
				
				parentGroup.GetAgents(agents);
				
				SCR_AIUtilityComponent aiUtility;
				
				foreach (AIAgent aiAgent : agents)
				{
					aiUtility = SCR_AIUtilityComponent.Cast(aiAgent.FindComponent(SCR_AIUtilityComponent));
					
					if (aiUtility && aiUtility.m_AIInfo.HasUnitState(EUnitState.IN_TURRET))
						unitStateTurret = true;
				}
			}
		}
		
		if (unitStateTurret)
		{
			SCR_AIBehaviorBase behavior = SCR_AIBehaviorBase.Cast(action);
			
			if (behavior)
				relatedActivity = SCR_AIActivityBase.Cast(behavior.GetRelatedGroupActivity());
			
			float score = action.Evaluate();
			
			score = 57;
			
			float priorityLevel = action.EvaluatePriorityLevel();
			
			priorityLevel = 0;
			
			IEntity vehicle = compartmentAccessComponent.GetCompartment().GetOwner();
			
			ECompartmentType compartmentType = SCR_AICompartmentHandling.CompartmentClassToType(compartmentAccessComponent.GetCompartment().Type());
			
			utility.AddAction(new SCR_AIGetInVehicle(utility, relatedActivity, vehicle, compartmentAccessComponent.GetCompartment(), roleInVehicle: compartmentType, priority: score, priorityLevel: priorityLevel));
		}
	}
}
