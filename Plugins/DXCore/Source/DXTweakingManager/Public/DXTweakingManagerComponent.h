#pragma once
#include "CoreMinimal.h"
#include "DXTweakingDataObject.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DXTweakingManagerComponent.generated.h"


//Forward declaration
class UDXTweakingManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateValuesInContainer);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DXTWEAKINGMANAGER_API UDXTweakingManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDXTweakingManagerComponent();
	
	UFUNCTION(BlueprintCallable)
	void PlayTweakingManagerFromDisk(FString TableName);

	void PlayTweakingManagerAsCSV(FString CSV);

	UPROPERTY(BlueprintAssignable)
	FUpdateValuesInContainer OnUpdateValuesInContainer;

	void SpawnMessageDialog() const;
	
protected:
	virtual void BeginPlay() override;
	
	UClass* FindClassByName(const FName& ClassName);

	bool FindAssetSoftObjectPath(const FName& ClassName, FSoftObjectPath& ObjectPath);

	FString AppendProjectPath(const FString& FilePath);

	bool ParseDataObjectAndUpdate(UDXTweakingDataObject* DataObject);
		
	bool FindPropertyFromClass(UClass* ObjectClass, const FName& PropertyName, FProperty*& OutProperty, UObject*& OutDefaultObject);

	FProperty* GetProperty(UClass* ObjectClass, const FName& PropertyName);

	UPROPERTY()
	bool bIsAllSuccess;

	template <typename PropertyType, typename ValueType>
	void TemplatedSetValue(FProperty* P, ValueType V, void* ConainerMemoryAddress);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
