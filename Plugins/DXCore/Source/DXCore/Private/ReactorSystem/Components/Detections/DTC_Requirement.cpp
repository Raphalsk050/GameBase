#include "ReactorSystem/Components/Detections/DTC_Requirement.h"
#include "Core/DXCoreGISubsystem.h"

UDTC_Requirement::UDTC_Requirement()
{
	bResetRequirementsWhenRestart = true;
}


void UDTC_Requirement::BeginPlay()
{
	Super::BeginPlay();

	NumReqs = NumericRequirements;
	TextReqs = TextRequirements;
	BoolReqs = BoolRequirements;
	
	if(bUseContinuousRequirements)
	{
		ResetContinuousRequirements( NumReqs, TextReqs, BoolReqs);
	}

	RegisterAllRequirementsForGlobalRequirements(NumericRequirements, TextRequirements, BoolRequirements);
}

void UDTC_Requirement::ResetRequirements()
{
	if(!bResetRequirementsWhenRestart)
		return;

	NumReqs = NumericRequirements;
	TextReqs = TextRequirements;
	BoolReqs = BoolRequirements;

	if(bUseContinuousRequirements)
	{
		ContinuousReqs.Reset();
		ResetContinuousRequirements( NumReqs, TextReqs, BoolReqs);
	}
	RegisterAllRequirementsForGlobalRequirements(NumericRequirements, TextRequirements, BoolRequirements);
}

bool UDTC_Requirement::DetectInternal_Implementation(AController* controller)
{
	bool superDetect = Super::DetectInternal_Implementation(controller);

	if(bUseContinuousRequirements)
	{
		TArray<bool> contReqsCompleted;
		ContinuousReqs.GenerateValueArray(contReqsCompleted);
		return !contReqsCompleted.Contains(false);
	}

	return NumReqs.Num() == 0 && TextReqs.Num() == 0 && BoolReqs.Num() == 0;
}

void UDTC_Requirement::ResetContinuousRequirements(TArray<FRequirementNumeric> _numReqs,TArray<FRequirementText> _textReqs, TArray<FRequirementBool>_boolReqs)
{
	for(auto& req : _numReqs)
	{
		ContinuousReqs.Add(req.RequiredDataName, false);
	}
	for(auto& req : _textReqs)
	{
		ContinuousReqs.Add(req.RequiredDataName, false);
	}
	for(auto& req : _boolReqs)
	{
		ContinuousReqs.Add(req.RequiredDataName, false);
	}
}

void UDTC_Requirement::RegisterAllRequirementsForGlobalRequirements(TArray<FRequirementNumeric> _numericRequirements,
	TArray<FRequirementText> _textRequirements, TArray<FRequirementBool> _boolRequirements)
{
	for(auto& req: _numericRequirements)
	{
		GetOwner()->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>()->RegisterForGlobalRequirement(req, this, bUseContinuousRequirements);
	}
	for(auto& req: _textRequirements)
	{
		GetOwner()->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>()->RegisterForGlobalRequirement(req, this, bUseContinuousRequirements);
	}
	for(auto& req: _boolRequirements)
	{
		GetOwner()->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>()->RegisterForGlobalRequirement(req, this, bUseContinuousRequirements);
	}
}


void UDTC_Requirement::ReceiveGlobalRequirementConcluded_Implementation(FRequirement RequirementValue)
{
		switch(RequirementValue.Type)
		{
		case ERequirementType::AT_Numeric:
			NumReqs.Remove(RequirementValue);
			break;
		case ERequirementType::AT_String:
			TextReqs.Remove(RequirementValue);
			break;
		case ERequirementType::AT_Bool:
			BoolReqs.Remove(RequirementValue);
			break;
		default:
			break;
		}
}

void UDTC_Requirement::ReceiveGlobalRequirementChanged_Implementation(FRequirement RequirementValue,
	bool isRequirementMet)
{
	ContinuousReqs.Add(RequirementValue.RequiredDataName, isRequirementMet);
}

void UDTC_Requirement::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(UWorld* world = GetWorld())
	{
		if(UDXCoreGISubsystem* gi = world->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>())
		{
			gi->RemoveAllRegistersFrom(this);
		}
	}
}
