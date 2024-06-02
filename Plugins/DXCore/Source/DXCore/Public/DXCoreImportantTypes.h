// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/Object.h"
#include "DXCoreImportantTypes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUserReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveControllerConnectionChanged);

UENUM(BlueprintType)
enum class ESaveStateStoreOperation : uint8
{
	SSSO_SaveState	UMETA(DisplayName = "SaveState"),
	SSSO_SaveInitialState	UMETA(DisplayName = "SaveInitialState")
};

UENUM(BlueprintType)
enum class ESaveStateSetupOperation : uint8
{
	SSSO_Loading	UMETA(DisplayName = "Loading"),
	SSSO_Restarting	UMETA(DisplayName = "Restarting")
};

UENUM(BlueprintType)
enum class EReactorStateSource : uint8
{
	RSS_Activating	UMETA(DisplayName = "Activating"),
    RSS_Deactivating	UMETA(DisplayName = "Deactivating"),
    RSS_Restarting	UMETA(DisplayName = "Restarting"),
    RSS_Loading	UMETA(DisplayName = "Loading")
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMM_Roll	UMETA(DisplayName = "CUSTOM Roll"),
	CMM_Prone	UMETA(DisplayName = "CUSTOM Prone")
};

UENUM(BlueprintType)
enum class ELevelPlayState : uint8
{
	LPS_Final	UMETA(DisplayName = "Final"),
	LPS_Blocking	UMETA(DisplayName = "Blocking")
};

UENUM(BlueprintType)
enum class EPlayConfig : uint8
{
	PC_Distribution	UMETA(DisplayName = "Distribution"),
	PC_Testing	UMETA(DisplayName = "Testing"),
	PC_DevTesting UMETA(DisplayName = "Development Testing"),
	PC_BlockingTesting UMETA(DisplayName = "Blocking Testing"),
	PC_Developing UMETA(DisplayName = "Developing")
};

UENUM(BlueprintType)
enum class EGetObject : uint8
{
	Then,
	GetFailed
};

UENUM(BlueprintType)
enum class EGetSave : uint8
{
	Then,
	NewSave,
	NotLoaded,
	UserInvalid,
	DiskOperationInProgress,
	Corruption
};

USTRUCT(BlueprintType)
struct DXCORE_API FParameterKeeperData
{
	GENERATED_BODY()
	FParameterKeeperData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, bool> Bools;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, int> Ints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, float> Floats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, FString> Strings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, FVector> Vectors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TMap<FString, FRotator> Rotators;

	bool IsEmpty() const
	{
		return Bools.Num() == 0 && Ints.Num() == 0 && Floats.Num() == 0 && Strings.Num() == 0 && Vectors.Num() == 0 && Rotators.Num() == 0;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DXCORE_API UParameterKeeper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParameterKeeper();

private:

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FParameterKeeperData _parameterData;

	uint8 bOverwritten:1;
	UPROPERTY(VisibleAnywhere, Category = "Parameter")
	FParameterKeeperData _overrideData;

	FParameterKeeperData& getParameterData();
	
	//Kept separate from saveable data for now (maybe even remove it later)
	UPROPERTY(EditAnywhere, Category="Parameter")
	TMap<FString, UObject*> _objects;

public:
	FParameterKeeperData ExportParameterData() const;
	void ImportData(FParameterKeeperData dataToImport);
	void OverrideData(FParameterKeeperData dataToOverrideWith);
	
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void ConsumeOverrideData();
	
	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasParameter(FString name);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool IsEmpty(bool bIncludeNonSaveable = false);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasBool(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool GetBool(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetBool(FString name, bool value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasInt(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllIntNames(TArray<FString>& names);
	UFUNCTION(BlueprintPure, Category="Parameter")
	int GetInt(FString name);
	UFUNCTION(BlueprintCallable, Category="Parameter")
	void SetInt(FString name, int value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasFloat(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllFloatNames(TArray<FString>& names);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	float GetFloat(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetFloat(FString name, float value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasString(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllStringNames(TArray<FString>& names);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	FString GetString(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetString(FString name, FString value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasVector(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllVectorNames(TArray<FString>& names);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	FVector GetVector(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetVector(FString name, FVector value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasRotator(FString name);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllRotatorNames(TArray<FString>& names);
	UFUNCTION(BlueprintPure, Category = "Parameter")
	FRotator GetRotator(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetRotator(FString name, FRotator value);

	UFUNCTION(BlueprintPure, Category = "Parameter")
	bool HasObject(FString name) const;
	UFUNCTION(BlueprintPure, Category = "Parameter")
	int GetAllObjectNames(TArray<FString>& names) const;
	UFUNCTION(BlueprintPure, Category = "Parameter")
	UObject* GetObject(FString name);
	UFUNCTION(BlueprintCallable, Category = "Parameter")
	void SetObject(FString name, UObject* value);
};
// ------------ Requirements

UENUM(BlueprintType)
enum class ERequirementType : uint8
{
	AT_Numeric	UMETA(DisplayName = "Numeric"),
	AT_String	UMETA(DisplayName = "Text"),
	AT_Bool UMETA(DisplayName = "Bool"),
	AT_Null	UMETA(DisplayeName = "Null")
};

USTRUCT(BlueprintInternalUseOnly)
struct DXCORE_API FRequirement
{
	GENERATED_BODY()

		FRequirement()
	{
		RequiredDataName = FName();
		Type = ERequirementType::AT_Null;
		Operand = FString();
	}
	virtual ~FRequirement() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
		FName RequiredDataName;
	UPROPERTY(BlueprintReadOnly, Category = "Requirement")
		ERequirementType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement")
		FString Operand;

	uint8 Operator = 0;

	virtual bool CheckRequirement(const AActor* reference, FString optionalValue = FString(""));


	FORCEINLINE bool operator==(const FRequirement& Other) const
	{
		return RequiredDataName == Other.RequiredDataName && Type == Other.Type && Operator == Operator && Operand == Other.Operand;
	}
};

UENUM(BlueprintType)
enum class ENumericComparer : uint8
{
	NC_Equals	UMETA(DisplayName = "=="),
	NC_GreaterThan	UMETA(DisplayName = ">"),
	NC_LessThan	UMETA(DisplayName = "<"),
	NC_GreaterOrEqual UMETA(DisplayName = ">="),
	NC_LessOrEqual	UMETA(DisplayName = "<="),
	NC_Different	UMETA(DisplayName = "!=")
};

USTRUCT(BlueprintType)
struct DXCORE_API FRequirementNumeric : public FRequirement
{
	GENERATED_BODY()

		FRequirementNumeric()
	{
		Type = ERequirementType::AT_Numeric;
		Operator = ENumericComparer::NC_Equals;
	}

	FRequirementNumeric(FRequirement requirement)
	{
		Type = requirement.Type;
		RequiredDataName = requirement.RequiredDataName;
		Operand = requirement.Operand;
		Operator = static_cast<ENumericComparer>(requirement.Operator);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement", meta = (DisplayAfter = "RequiredDataName"))
		ENumericComparer Operator;

	bool CheckRequirement(const AActor* reference, FString optionalValue = FString("")) override;

	FRequirement ToRequirement()
	{
		FRequirement req = *this;
		req.Operator = (uint8)Operator;
		return req;
	}
};

UENUM(BlueprintType)
enum class EStringComparer : uint8
{
	SC_Equals	UMETA(DisplayName = "Equals"),
	SC_Contains	UMETA(DisplayName = "Contains"),
	SC_NotContains	UMETA(DisplayName = "NotContains")
};

USTRUCT(BlueprintType)
struct DXCORE_API FRequirementText : public FRequirement
{
	GENERATED_BODY()

		FRequirementText()
	{
		Type = ERequirementType::AT_String;
		Operator = EStringComparer::SC_Equals;
	}

	FRequirementText(FRequirement requirement)
	{
		Type = requirement.Type;
		RequiredDataName = requirement.RequiredDataName;
		Operand = requirement.Operand;
		Operator = static_cast<EStringComparer>(requirement.Operator);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement", meta = (DisplayAfter = "RequiredDataName"))
		EStringComparer Operator;

	bool CheckRequirement(const AActor* reference, FString optionalValue = FString("")) override;

	FRequirement ToRequirement()
	{
		FRequirement req = *this;
		req.Operator = (uint8)Operator;
		return req;
	}
};

UENUM(BlueprintType)
enum class EBoolComparer : uint8
{
	BC_Equals	UMETA(DisplayName = "Equals"),
	BC_NotEquals UMETA(DisplayName = "Not Equals"),
	BC_And	UMETA(DisplayName = "And"),
	BC_Or	UMETA(DisplayName = "Or")
};

USTRUCT(BlueprintType)
struct DXCORE_API FRequirementBool : public FRequirement
{
	GENERATED_BODY()

		FRequirementBool()
	{
		Type = ERequirementType::AT_Bool;
		Operator = EBoolComparer::BC_Equals;
	}

	FRequirementBool(FRequirement requirement)
	{
		Type = requirement.Type;
		RequiredDataName = requirement.RequiredDataName;
		Operand = requirement.Operand;
		Operator = static_cast<EBoolComparer>(requirement.Operator);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement", meta = (DisplayAfter = "RequiredDataName"))
		EBoolComparer Operator;

	bool CheckRequirement(const AActor* reference, FString optionalValue = FString("")) override;

	FRequirement ToRequirement()
	{
		FRequirement req = *this;
		req.Operator = (uint8)Operator;
		return req;
	}
};

// ------------ Global Required Data

UENUM(BlueprintType)
enum class EGlobalRequiredDataPersistance : uint8
{
	GRDP_Instant UMETA(DisplayName = "Instant Only"),
	GRDP_LevelPersist UMETA(DisplayName = "Persist Level"),
	GRDP_Persist UMETA(DisplayName = "Persist")
};

USTRUCT(BlueprintType)
struct DXCORE_API FGlobalDataKeeper
{
	GENERATED_BODY()

		FGlobalDataKeeper()
	{

	}

private:
	UPROPERTY()
		TMap<FName, int> _intData;
	UPROPERTY()
		TMap<FName, float> _floatData;
	UPROPERTY()
		TMap<FName, FString> _stringData;
	UPROPERTY()
		TMap<FName, bool> _boolData;



public:

	bool CheckRequirement(FRequirement requirement);

	bool HasData(const FName dataName);

	int AddIntData(const FName dataName, const int value, const bool bIncremental);
	float AddFloatData(const FName dataName, const float value, const bool bIncremental);
	FString AddStringData(const FName dataName, const FString value, const bool bIncremental);
	bool AddBoolData(const FName dataName, const bool value, const bool bIncremental);

	void RemoveData(FName dataName);
	void ClearData();
	void PopAllIntData(TMap<FName, int>& data);
	void PopAllFloatData(TMap<FName, float>& data);
	void PopAllStringData(TMap<FName, FString>& data);
	void PopAllBoolData(TMap<FName, bool>& data);
};

USTRUCT()
struct DXCORE_API FRequirementRegister
{
	GENERATED_USTRUCT_BODY()
		/** Stuff */
	UPROPERTY()
	UObject* RegisteredObject;
	UPROPERTY()
	FRequirement Requirement;

	UPROPERTY()
	uint8 Continuous:1;

	FORCEINLINE bool operator==(const FRequirementRegister &Other) const
	{
		return RegisteredObject == Other.RegisteredObject && Requirement.Type == Other.Requirement.Type && Requirement.RequiredDataName == Other.Requirement.RequiredDataName && Requirement.Operator == Other.Requirement.Operator && Requirement.Operand == Other.Requirement.Operand;
	}
};

// ----------------- Requirement Update
USTRUCT(BlueprintInternalUseOnly)
struct  DXCORE_API FGlobalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RequiredName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGlobalRequiredDataPersistance DataPersistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Incremental;

	FGlobalData()
	{
		RequiredName = FName();
		DataPersistance = EGlobalRequiredDataPersistance::GRDP_Persist;
		Incremental = false;
	}
	virtual ~FGlobalData() {}
};

USTRUCT(BlueprintType)
struct  DXCORE_API FIntGlobalData : public FGlobalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NewValue;

	FIntGlobalData() : FGlobalData()
	{
		NewValue = 1;
	}
};

USTRUCT(BlueprintType)
struct  DXCORE_API FBoolGlobalData : public FGlobalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NewValue;

	FBoolGlobalData() : FGlobalData()
	{
		NewValue = true;
	}
};

USTRUCT(BlueprintType)
struct DXCORE_API FTextGlobalData : public FGlobalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NewValue;

	FTextGlobalData() : FGlobalData()
	{
		NewValue = FString();
	}
};

USTRUCT(BlueprintType)
struct DXCORE_API FFloatGlobalData : public FGlobalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NewValue;

	FFloatGlobalData() : FGlobalData()
	{
		NewValue = 1;
	}
};

// ==================== Requirement System - End ====================

// ==================== Cosmetic Events - Start ====================

USTRUCT(BlueprintInternalUseOnly)
struct FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventParameter(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	FString ParameterName;
};

USTRUCT(BlueprintType)
struct FCosmeticEventBoolParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventBoolParameter(){ BoolValue = false; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	bool BoolValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventIntParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventIntParameter(){ IntValue = 0; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	int IntValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventFloatParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventFloatParameter(){ FloatValue = 0; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	float FloatValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventStringParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventStringParameter(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	FString StringValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventVectorParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventVectorParameter(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	FVector VectorValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventRotatorParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventRotatorParameter(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	FRotator RotatorValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventTransformParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventTransformParameter(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	FTransform TransformValue;
};

USTRUCT(BlueprintType)
struct FCosmeticEventObjectParameter : public FCosmeticEventParameter
{
	GENERATED_BODY()
	FCosmeticEventObjectParameter(){ ObjectValue = nullptr; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Event Parameter")
	UObject* ObjectValue;
};

USTRUCT(BlueprintType)
struct DXCORE_API FCosmeticEventData
{
	GENERATED_BODY()
	FCosmeticEventData() {}	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventBoolParameter> BoolParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventIntParameter> IntParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventFloatParameter> FloatParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventStringParameter> StringParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventVectorParameter> VectorParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventRotatorParameter> RotatorParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventTransformParameter> TransformParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	TArray<FCosmeticEventObjectParameter> ObjectParameters;

	//A separate finder function for each of the value types in FCosmeticEventParameters, returning the value directly and also a bool if it was found or not, using FString as key
	bool FindBool(const FString& ParameterName, bool& OutBoolValue)
	{
		for (auto& Parameter : BoolParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutBoolValue = Parameter.BoolValue;
				return true;
			}
		}
		return false;
	}
	bool FindInt(const FString& ParameterName, int& OutIntValue)
	{
		for (auto& Parameter : IntParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutIntValue = Parameter.IntValue;
				return true;
			}
		}
		return false;
	}
	bool FindFloat(const FString& ParameterName, float& OutFloatValue)
	{
		for (auto& Parameter : FloatParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutFloatValue = Parameter.FloatValue;
				return true;
			}
		}
		return false;
	}
	bool FindString(const FString& ParameterName, FString& OutStringValue)
	{
		for (auto& Parameter : StringParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutStringValue = Parameter.StringValue;
				return true;
			}
		}
		return false;
	}
	bool FindVector(const FString& ParameterName, FVector& OutVectorValue)
	{
		for (auto& Parameter : VectorParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutVectorValue = Parameter.VectorValue;
				return true;
			}
		}
		return false;
	}
	bool FindRotator(const FString& ParameterName, FRotator& OutRotatorValue)
	{
		for (auto& Parameter : RotatorParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutRotatorValue = Parameter.RotatorValue;
				return true;
			}
		}
		return false;
	}
	bool FindTransform(const FString& ParameterName, FTransform& OutTransformValue)
	{
		for (auto& Parameter : TransformParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutTransformValue = Parameter.TransformValue;
				return true;
			}
		}
		return false;
	}
	bool FindObject(const FString& ParameterName, UObject*& OutObjectValue)
	{
		for (auto& Parameter : ObjectParameters)
		{
			if (Parameter.ParameterName == ParameterName)
			{
				OutObjectValue = Parameter.ObjectValue;
				return true;
			}
		}
		return false;
	}

	//Add functions for each type, receiving the ParameterName and the value to add
	void AddBool(const FString& ParameterName, bool BoolValue)
	{
		FCosmeticEventBoolParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.BoolValue = BoolValue;
		BoolParameters.Add(NewParameter);
	}
	void AddInt(const FString& ParameterName, int IntValue)
	{
		FCosmeticEventIntParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.IntValue = IntValue;
		IntParameters.Add(NewParameter);
	}
	void AddFloat(const FString& ParameterName, float FloatValue)
	{
		FCosmeticEventFloatParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.FloatValue = FloatValue;
		FloatParameters.Add(NewParameter);
	}
	void AddString(const FString& ParameterName, FString StringValue)
	{
		FCosmeticEventStringParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.StringValue = StringValue;
		StringParameters.Add(NewParameter);
	}
	void AddVector(const FString& ParameterName, FVector VectorValue)
	{
		FCosmeticEventVectorParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.VectorValue = VectorValue;
		VectorParameters.Add(NewParameter);
	}
	void AddRotator(const FString& ParameterName, FRotator RotatorValue)
	{
		FCosmeticEventRotatorParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.RotatorValue = RotatorValue;
		RotatorParameters.Add(NewParameter);
	}
	void AddTransform(const FString& ParameterName, FTransform TransformValue)
	{
		FCosmeticEventTransformParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.TransformValue = TransformValue;
		TransformParameters.Add(NewParameter);
	}
	void AddObject(const FString& ParameterName, UObject* ObjectValue)
	{
		FCosmeticEventObjectParameter NewParameter;
		NewParameter.ParameterName = ParameterName;
		NewParameter.ObjectValue = ObjectValue;
		ObjectParameters.Add(NewParameter);
	}
};

// ==================== Cosmetic Events - End ====================