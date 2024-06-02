#include "DXTweakingManagerComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/SCS_Node.h"
#include "Misc/MessageDialog.h"

// Sets default values for this component's properties
UDXTweakingManagerComponent::UDXTweakingManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UDXTweakingManagerComponent::PlayTweakingManagerFromDisk(FString TableName)
{
	// String for message dialog
	FString MessageString = TEXT("");
	FString InitTable = (TEXT("Data/"));
	FString EndName = (TEXT(".csv"));
	TArray<FString> AllName;
	AllName.Add(InitTable + TableName + EndName);
	const TArray<FString> FilesToLoad = AllName;
	const bool bAppendProjectPath = true;
	
	// Checks if the file loading is all good
	bIsAllSuccess = true;
	
	if (FilesToLoad.Num() > 0)
	{
		for (FString It : FilesToLoad)
		{
			UDXTweakingDataObject* SourceDataObject = NewObject<UDXTweakingDataObject>();
			if (SourceDataObject &&
				SourceDataObject->ParseCsv(bAppendProjectPath ? AppendProjectPath(It) : It))
			{
				ParseDataObjectAndUpdate(SourceDataObject);
			}
			// else
			// {
			// 	bIsAllSuccess = false;
			// }
		}
	}
	else
	{
		MessageString = TEXT("No filepaths added. Add the files to load in the Project Settings.");
	}
}

void UDXTweakingManagerComponent::PlayTweakingManagerAsCSV(FString CSV)
{
	UDXTweakingDataObject* SourceDataObject = NewObject<UDXTweakingDataObject>();
	
	if (SourceDataObject &&
		SourceDataObject->ParseCsv(CSV))
	{
		ParseDataObjectAndUpdate(SourceDataObject);
	}
}

void UDXTweakingManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDXTweakingManagerComponent::SpawnMessageDialog() const
{
	FString MessageString = TEXT("");
	MessageString = TEXT("Update successful!");
				//TEXT("Error, check logs for more details.");
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(MessageString));
}

UClass* UDXTweakingManagerComponent::FindClassByName(const FName& ClassName)
{
	if (ClassName.IsNone())
		return nullptr;

	// Get asset registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetsList;
	AssetRegistry.GetAssetsByPath("/Game/", AssetsList, true);

	for (FAssetData It : AssetsList)
	{
		if (!It.IsValid())
		{
			continue;
		}

		// Get with matching name
		if (It.AssetName == ClassName)
		{
			// If the object is a BP class, get the BP generated class instead
			const UBlueprint* BlueprintClass = Cast<UBlueprint>(It.GetAsset());
			if (BlueprintClass)
			{
				return BlueprintClass->GeneratedClass;
			}
			return It.GetAsset()->GetClass();
		}
	}
	return nullptr;
}

bool UDXTweakingManagerComponent::FindAssetSoftObjectPath(const FName& ClassName, FSoftObjectPath& ObjectPath)
{
	if (ClassName.IsNone())
		return false;

	// Get asset registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetsList;
	AssetRegistry.GetAssetsByPath("/Game/", AssetsList, true);

	for (FAssetData It : AssetsList)
	{
		if (!It.IsValid())
		{
			continue;
		}

		// Get with matching name
		if (It.AssetName == ClassName)
		{
			ObjectPath = It.ToSoftObjectPath();
			return true;
		}
	}

	return false;
}

FString UDXTweakingManagerComponent::AppendProjectPath(const FString& FilePath)
{
	// Don't do anything if the specified file path is empty
	if (FilePath.IsEmpty())
	{
		return "";
	}
		
	// Project path to append
	const FString ProjectPath = FPaths::ProjectDir();
				
	// If specified file path already has the project path, return as is; else return with the project path appended
	return (FilePath.StartsWith(ProjectPath)) ? FilePath : (ProjectPath + FilePath);
}

bool UDXTweakingManagerComponent::ParseDataObjectAndUpdate(UDXTweakingDataObject* DataObject)
{
	if (!DataObject)
		return false;
		
	bool bHasError = false;
		
	// Iterate through the data object by rows of data
	for (int32 i = 0; i < DataObject->GetNumberOfRows(); i++)
	{
		TArray<UDXTweakingSelectNode*> CurrentNodes;
		if (DataObject->GetRow(i, CurrentNodes) && CurrentNodes.Num() > 0)
		{
			// Assume first node as the object name
			UClass* AssetClass = FindClassByName(CurrentNodes[0]->GetValueAsName());
			if (!AssetClass)
			{
				bHasError = true;
				continue;
			}
			
			// Iterate through the remaining nodes and try to find the variable as property and update its value
			for (int32 j = 1; j < CurrentNodes.Num(); j++)
			{
				// Property name
				const FName PropertyName = *DataObject->DataHeader[j];
				
				FProperty* PropertyToChange = nullptr;
				UObject* DefaultObject = nullptr;
				//AssetClass = Cast<AssetClass>(DefaultObject->GetClass());
				
				if (FindPropertyFromClass(AssetClass, PropertyName, PropertyToChange, DefaultObject))
				{
					// This is the hacky part, since we can't convert string into variable type, we just have to check one by one
					const FString VariableType = PropertyToChange->GetCPPType();
					
					if (VariableType == TEXT("float"))
					{
						TemplatedSetValue<FFloatProperty, float>(PropertyToChange, CurrentNodes[j]->GetValueAsFloat(), this->GetOwner());
					}
					else if (VariableType == TEXT("int32"))
					{
						TemplatedSetValue<FIntProperty, int>(PropertyToChange, CurrentNodes[j]->GetValueAsInteger(), this->GetOwner());
					}
					else if (VariableType == TEXT("FName"))
					{
						TemplatedSetValue<FNameProperty, FName>(PropertyToChange, CurrentNodes[j]->GetValueAsName(), this->GetOwner());
					}
					else if (VariableType == TEXT("bool"))
					{
						TemplatedSetValue<FBoolProperty, bool>(PropertyToChange, CurrentNodes[j]->GetValueAsBool(), this->GetOwner());
					}
					
					else
					{
						bHasError = true;
					}
				}
				else
				{
					bHasError = true;
				}
			}
			// Mark package dirty so save prompt can appear
			bool Dirty = AssetClass->MarkPackageDirty();
			//FFloatProperty* as;
			//as->InitializePropertyValue()
		}
	}
	return !bHasError;
}

bool UDXTweakingManagerComponent::FindPropertyFromClass(UClass* ObjectClass, const FName& PropertyName,
	FProperty*& OutProperty, UObject*& OutDefaultObject)
{
	if (!ObjectClass || PropertyName.IsNone())
		return false;

	// Find the property on the class
	OutProperty = GetProperty(ObjectClass, PropertyName);

	if (OutProperty)
	{
		OutDefaultObject = ObjectClass->GetDefaultObject<UObject>();
		return true;
	}
	else
	{
		// If property is not found, iterate through components in Blueprint
		UBlueprintGeneratedClass* BpClass = Cast<UBlueprintGeneratedClass>(ObjectClass);
		while (BpClass)
		{
			for (USCS_Node* NodeIt : BpClass->SimpleConstructionScript->GetAllNodes())
			{
				// Look for property in the component class
				OutProperty = GetProperty(NodeIt->ComponentClass, PropertyName);
				if (OutProperty)
				{
					// Set default object as the component from the original class, not the super
					OutDefaultObject = NodeIt->GetActualComponentTemplate(Cast<UBlueprintGeneratedClass>(ObjectClass));
					return true;
				}
			}
			// Get super class since the SCS Node can only get immediate components
			BpClass = Cast<UBlueprintGeneratedClass>(BpClass->GetSuperClass());
		}
	}
	return false;
}

FProperty* UDXTweakingManagerComponent::GetProperty(UClass* ObjectClass, const FName& PropertyName)
{
	if (!ObjectClass || PropertyName.IsNone())
		return nullptr;

	for (TFieldIterator<FProperty> PropertyItr(ObjectClass); PropertyItr; ++PropertyItr)
	{
		FProperty* Property = *PropertyItr;
		if (Property && Property->GetFName() == PropertyName)
		{
			return Property;
		}
	}
	return nullptr;
}

template <typename PropertyType, typename ValueType>
void UDXTweakingManagerComponent::TemplatedSetValue(FProperty* P, ValueType V, void* ConainerMemoryAddress)
{
	if (P->IsA(PropertyType::StaticClass()))
	{
		SpawnMessageDialog();
		ValueType* PropertyValuePtr = P->ContainerPtrToValuePtr<ValueType>(ConainerMemoryAddress);
		if (PropertyValuePtr)
		{
			*PropertyValuePtr = V;
			OnUpdateValuesInContainer.Broadcast();
		}
	}
}

// Called when the game starts
void UDXTweakingManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

