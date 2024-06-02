// Fill out your copyright notice in the Description page of Project Settings.


#include "DXCoreImportantTypes.h"

#include "DXCoreFunctionLibrary.h"


UParameterKeeper::UParameterKeeper() : bOverwritten(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

FParameterKeeperData& UParameterKeeper::getParameterData()
{
	return bOverwritten ? _overrideData : _parameterData;
}

FParameterKeeperData UParameterKeeper::ExportParameterData() const
{
	return _parameterData;
}

void UParameterKeeper::ImportData(FParameterKeeperData dataToImport)
{
	_parameterData = dataToImport;
}

void UParameterKeeper::OverrideData(FParameterKeeperData dataToOverrideWith)
{
	bOverwritten = true;
	_overrideData = dataToOverrideWith;
}

void UParameterKeeper::ConsumeOverrideData()
{
	_overrideData = FParameterKeeperData();
	bOverwritten = false;
}

bool UParameterKeeper::HasParameter(FString name)
{
	return HasBool(name) || HasInt(name) || HasFloat(name) || HasString(name) || HasVector(name) || HasRotator(name) || HasObject(name);
}

bool UParameterKeeper::IsEmpty(bool bIncludeNonSaveable)
{
	return bIncludeNonSaveable ? getParameterData().IsEmpty() && _objects.Num() == 0 : getParameterData().IsEmpty();
}

bool UParameterKeeper::HasBool(FString name)
{
	return getParameterData().Bools.Contains(name);
}

bool UParameterKeeper::GetBool(FString name)
{
	bool result = false;
	if (getParameterData().Bools.Contains(name))
	{
		result = getParameterData().Bools[name];
	}
	return result;
}

void UParameterKeeper::SetBool(FString name, bool value)
{
	getParameterData().Bools.Add(name, value);
}

bool UParameterKeeper::HasInt(FString name)
{
	return getParameterData().Ints.Contains(name);
}

int UParameterKeeper::GetAllIntNames(TArray<FString>& names)
{
	return getParameterData().Ints.GetKeys(names);
}

int UParameterKeeper::GetInt(FString name)
{
	int result = 0;
	if (getParameterData().Ints.Contains(name))
	{
		result = getParameterData().Ints[name];
	}
	return result;
}

void UParameterKeeper::SetInt(FString name, int value)
{
	getParameterData().Ints.Add(name, value);
}

bool UParameterKeeper::HasFloat(FString name)
{
	return getParameterData().Floats.Contains(name);
}

int UParameterKeeper::GetAllFloatNames(TArray<FString>& names)
{
	return getParameterData().Floats.GetKeys(names);
}

float UParameterKeeper::GetFloat(FString name)
{
	float result = 0;
	if (getParameterData().Floats.Contains(name))
	{
		result = getParameterData().Floats[name];
	}
	return result;
}

void UParameterKeeper::SetFloat(FString name, float value)
{
	getParameterData().Floats.Add(name, value);
}

bool UParameterKeeper::HasString(FString name)
{
	return getParameterData().Strings.Contains(name);
}

int UParameterKeeper::GetAllStringNames(TArray<FString>& names)
{
	return getParameterData().Strings.GetKeys(names);
}

FString UParameterKeeper::GetString(FString name)
{
	FString result = FString();
	if (getParameterData().Strings.Contains(name))
	{
		result = getParameterData().Strings[name];
	}
	return result;
}

void UParameterKeeper::SetString(FString name, FString value)
{
	getParameterData().Strings.Add(name, value);
}

bool UParameterKeeper::HasVector(FString name)
{
	return getParameterData().Vectors.Contains(name);
}

int UParameterKeeper::GetAllVectorNames(TArray<FString>& names)
{
	return getParameterData().Vectors.GetKeys(names);
}

FVector UParameterKeeper::GetVector(FString name)
{
	FVector result = FVector();
	if (getParameterData().Vectors.Contains(name))
	{
		result = getParameterData().Vectors[name];
	}
	return result;
}

void UParameterKeeper::SetVector(FString name, FVector value)
{
	getParameterData().Vectors.Add(name, value);
}

bool UParameterKeeper::HasRotator(FString name)
{
	return getParameterData().Rotators.Contains(name);
}

int UParameterKeeper::GetAllRotatorNames(TArray<FString>& names)
{
	return getParameterData().Rotators.GetKeys(names);
}

FRotator UParameterKeeper::GetRotator(FString name)
{
	FRotator result = FRotator();
	if (getParameterData().Rotators.Contains(name))
	{
		result = getParameterData().Rotators[name];
	}
	return result;
}

void UParameterKeeper::SetRotator(FString name, FRotator value)
{
	getParameterData().Rotators.Add(name, value);
}

bool UParameterKeeper::HasObject(FString name) const
{
	return _objects.Contains(name);
}

int UParameterKeeper::GetAllObjectNames(TArray<FString>& names) const
{
	return _objects.GetKeys(names);
}

UObject* UParameterKeeper::GetObject(FString name)
{
	UObject* result = nullptr;
	if (_objects.Contains(name))
	{
		result = _objects[name];
	}
	return result;
}

void UParameterKeeper::SetObject(FString name, UObject* value)
{
	_objects.Add(name, value);
}

bool FRequirement::CheckRequirement(const AActor* reference, FString optionalValue)
{
	switch (Type)
	{
	case ERequirementType::AT_Numeric:
		return FRequirementNumeric(*this).CheckRequirement(reference, optionalValue);
		break;
	case ERequirementType::AT_String:
		return FRequirementText(*this).CheckRequirement(reference, optionalValue);
		break;
	case ERequirementType::AT_Bool:
		return FRequirementBool(*this).CheckRequirement(reference, optionalValue);
		break;
	case ERequirementType::AT_Null:
		return false;
		break;
	}
	return false;
}

bool FRequirementNumeric::CheckRequirement(const AActor* reference, FString optionalValue)
{
	bool result = false;
	float left = 0;
	if (optionalValue.IsEmpty())
	{
		FString RequiredDataString = RequiredDataName.ToString();
		UDXCoreFunctionLibrary::TryReplaceWordsForNumbers(RequiredDataString, reference, RequiredDataString);
		UDXCoreFunctionLibrary::TryEvaluateExpression(RequiredDataString, left);
	}
	else
	{
		UDXCoreFunctionLibrary::TryEvaluateExpression(optionalValue, left);
	}


	FString OperandTemp = FString(Operand);
	UDXCoreFunctionLibrary::TryReplaceWordsForNumbers(OperandTemp, reference, OperandTemp);
	float right;
	UDXCoreFunctionLibrary::TryEvaluateExpression(OperandTemp, right);

	switch (Operator)
	{
	case ENumericComparer::NC_Equals:
		result = left == right;
		break;
	case ENumericComparer::NC_GreaterThan:
		result = left > right;
		break;
	case ENumericComparer::NC_LessThan:
		result = left < right;
		break;
	case ENumericComparer::NC_GreaterOrEqual:
		result = left >= right;
		break;
	case ENumericComparer::NC_LessOrEqual:
		result = left <= right;
		break;
	case ENumericComparer::NC_Different:
		result = left != right;
		break;
	}

	return result;
}

bool FRequirementText::CheckRequirement(const AActor* reference, FString optionalValue)
{
	bool result = false;
	FString left;
	if (optionalValue.IsEmpty())
	{
		//Try to get through parameterkeeper the string with RequiredDataName
		//set left
		if (reference)
		{
			UParameterKeeper* parameterKeeper = Cast<UParameterKeeper>(reference->GetComponentByClass(UParameterKeeper::StaticClass()));
			if (parameterKeeper)
			{
				left = parameterKeeper->GetString(RequiredDataName.ToString());
			}
		}
	}
	else
	{
		left = optionalValue;
	}

	switch (Operator)
	{
	case EStringComparer::SC_Equals:
		result = left == Operand;
		break;
	case EStringComparer::SC_Contains:
		result = left.Contains(Operand);
		break;
	case EStringComparer::SC_NotContains:
		result = !left.Contains(Operand);
		break;
	}

	return result;
}

bool FRequirementBool::CheckRequirement(const AActor* reference, FString optionalValue)
{
	bool result = false;
	bool left = false;
	if (optionalValue.IsEmpty())
	{
		if (reference)
		{
			UParameterKeeper* parameterKeeper = Cast<UParameterKeeper>(reference->GetComponentByClass(UParameterKeeper::StaticClass()));
			if (parameterKeeper)
			{
				left = parameterKeeper->GetBool(RequiredDataName.ToString());
			}
		}
	}
	else
	{
		left = optionalValue.Equals(FString("1"), ESearchCase::Type::IgnoreCase) || optionalValue.Equals(FString("T"), ESearchCase::Type::IgnoreCase) || optionalValue.Equals(FString("True"), ESearchCase::Type::IgnoreCase);
	}

	bool right = Operand.Equals(FString("1"), ESearchCase::Type::IgnoreCase) || Operand.Equals(FString("T"), ESearchCase::Type::IgnoreCase) || Operand.Equals(FString("True"), ESearchCase::Type::IgnoreCase);

	switch (Operator)
	{
	case EBoolComparer::BC_Equals:
		result = left == right;
		break;
	case EBoolComparer::BC_NotEquals:
		result = left != right;
		break;
	case EBoolComparer::BC_And:
		result = left && right;
		break;
	case EBoolComparer::BC_Or:
		result = left || right;
		break;
	}

	return result;
}

bool FGlobalDataKeeper::CheckRequirement(FRequirement requirement)
{
	FName& dataName = requirement.RequiredDataName;
	switch (requirement.Type)
	{
	case ERequirementType::AT_Numeric:
		if (_intData.Contains(dataName))
		{
			return requirement.CheckRequirement(nullptr, FString::FromInt(_intData[dataName]));
		}
		else if (_floatData.Contains(dataName))
		{
			return requirement.CheckRequirement(nullptr, FString::SanitizeFloat(_floatData[dataName]));
		}
		else 
		{
			return requirement.CheckRequirement(nullptr, FString::FromInt(0));
		}
		break;
	case ERequirementType::AT_String:
		if (_stringData.Contains(dataName))
		{
			return requirement.CheckRequirement(nullptr, _stringData[dataName]);
		}
		else 
		{
			return requirement.CheckRequirement(nullptr);
		}
		break;
	case ERequirementType::AT_Bool:
		if (_boolData.Contains(dataName))
		{
			return requirement.CheckRequirement(nullptr, _boolData[dataName] ? FString("1") : FString("0"));
		}
		else 
		{
			return requirement.CheckRequirement(nullptr, FString("0"));
		}
		break;
	default:
		return false;
	}

	return false;
}

bool FGlobalDataKeeper::HasData(const FName dataName)
{
	return _boolData.Contains(dataName) || _intData.Contains(dataName) || _floatData.Contains(dataName) || _stringData.Contains(dataName);
}

int FGlobalDataKeeper::AddIntData(const FName dataName, const int value, const bool bIncremental)
{
	int result = value;
	if (_intData.Contains(dataName))
	{
		result = bIncremental ? _intData[dataName] + value : value;
		_intData[dataName] = result;
	}
	else
	{
		RemoveData(dataName);
		_intData.Add(dataName, value);
	}
	return result;
}

float FGlobalDataKeeper::AddFloatData(const FName dataName, const float value, const bool bIncremental)
{
	float result = value;
	if (_floatData.Contains(dataName))
	{
		result = bIncremental ? _floatData[dataName] + value : value;
		_floatData[dataName] = result;
	}
	else
	{
		RemoveData(dataName);
		_floatData.Add(dataName, value);
	}
	return result;
}

FString FGlobalDataKeeper::AddStringData(const FName dataName, const FString value, const bool bIncremental)
{
	FString result;
	if (_stringData.Contains(dataName))
	{
		result = bIncremental ? _stringData[dataName] + value : value;
		_stringData[dataName] = result;
	}
	else
	{
		RemoveData(dataName);
		result = value;
		_stringData.Add(dataName, value);
	}
	return result;
}

bool FGlobalDataKeeper::AddBoolData(const FName dataName, const bool value, const bool bIncremental)
{
	bool result = value;
	if (_boolData.Contains(dataName))
	{
		result = bIncremental ? _boolData[dataName] || value : value;
		_boolData[dataName] = result;
	}
	else
	{
		RemoveData(dataName);
		_boolData.Add(dataName, value);
	}
	return result;
}

void FGlobalDataKeeper::RemoveData(FName dataName)
{
	_intData.Remove(dataName);
	_floatData.Remove(dataName);
	_stringData.Remove(dataName);
	_boolData.Remove(dataName);
}

void FGlobalDataKeeper::ClearData()
{
	_intData.Empty();
	_floatData.Empty();
	_stringData.Empty();
	_boolData.Empty();
}

void FGlobalDataKeeper::PopAllIntData(TMap<FName, int>& data)
{
	data = _intData;
	_intData.Empty();
}

void FGlobalDataKeeper::PopAllFloatData(TMap<FName, float>& data)
{
	data = _floatData;
	_floatData.Empty();
}

void FGlobalDataKeeper::PopAllStringData(TMap<FName, FString>& data)
{
	data = _stringData;
	_stringData.Empty();
}

void FGlobalDataKeeper::PopAllBoolData(TMap<FName, bool>& data)
{
	data = _boolData;
	_boolData.Empty();
}
