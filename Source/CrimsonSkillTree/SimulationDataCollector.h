#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Misc/ScopeRWLock.h"
#include "UObject/StructOnScope.h"
#include "SimulationDataCollector.generated.h"

/**
 * @struct FSimulationDataCollector
 * @brief Collects various types of simulation data (as USTRUCTs) from NodeEvents.
 * @details ActivationConditions can query this collector for data types they understand.
 * The data is stored in a map where the key is the UStruct type and the value is an array of instances.
 */
USTRUCT(BlueprintType)
struct FSimulationDataCollector
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	FSimulationDataCollector() = default;

	/**
	 * @brief Adds a piece of simulation data (any USTRUCT) to the collector.
	 * @details The data is copied into a new FStructOnScope for proper memory management.
	 * @tparam FStructType The type of the USTRUCT to add.
	 * @param DataPayload The actual data to add.
	 */
	template <typename FStructType>
	void AddData(const FStructType& DataPayload)
	{
		const UScriptStruct* StructType = FStructType::StaticStruct();
		if (!StructType) return;

		TSharedPtr<FStructOnScope> NewDataInstance = MakeShared<FStructOnScope>(StructType);
		StructType->CopyScriptStruct(NewDataInstance->GetStructMemory(), &DataPayload);

		TArray<TSharedPtr<FStructOnScope>>& DataList = CollectedData.FindOrAdd(StructType);
		DataList.Add(NewDataInstance);
	}

	/**
	 * @brief Retrieves the first piece of simulation data of the specified USTRUCT type.
	 * @tparam FStructType The type of the USTRUCT to retrieve.
	 * @return A const pointer to the data if found, otherwise nullptr. The pointer is valid for the lifetime of the collector.
	 */
	template <typename FStructType>
	const FStructType* GetData() const
	{
		const UScriptStruct* StructType = FStructType::StaticStruct();
		if (!StructType) return nullptr;

		const TArray<TSharedPtr<FStructOnScope>>* DataListPtr = CollectedData.Find(StructType);
		if (DataListPtr && DataListPtr->Num() > 0 && (*DataListPtr)[0].IsValid())
		{
			return reinterpret_cast<const FStructType*>((*DataListPtr)[0]->GetStructMemory());
		}
		return nullptr;
	}

	/**
	 * @brief Retrieves all pieces of simulation data of a specified USTRUCT type.
	 * @tparam FStructType The type of the USTRUCT to retrieve.
	 * @param OutDataArray An array that will be populated with const pointers to the data. Pointers are valid for the lifetime of the collector.
	 */
	template <typename FStructType>
	void GetAllDataOfType(TArray<const FStructType*>& OutDataArray) const
	{
		OutDataArray.Empty();
		const UScriptStruct* StructType = FStructType::StaticStruct();
		if (!StructType) return;

		const TArray<TSharedPtr<FStructOnScope>>* DataListPtr = CollectedData.Find(StructType);
		if (DataListPtr)
		{
			for (const TSharedPtr<FStructOnScope>& DataInstance : *DataListPtr)
			{
				if (DataInstance.IsValid())
				{
					OutDataArray.Add(reinterpret_cast<const FStructType*>(DataInstance->GetStructMemory()));
				}
			}
		}
	}

	/**
	 * @brief Clears all data currently stored in the collector.
	 */
	void Clear()
	{
		CollectedData.Empty();
	}

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	/** @brief Stores UStruct type -> array of instances of that struct (as FStructOnScope for proper UStruct handling). */
	TMap<const UScriptStruct*, TArray<TSharedPtr<FStructOnScope>>> CollectedData;
};
